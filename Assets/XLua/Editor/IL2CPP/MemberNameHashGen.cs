using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text.RegularExpressions;
using UnityEngine;
using Debug = UnityEngine.Debug;

namespace XLua.IL2CPP.Editor.Generator{
    public class MemberNameHashGen {
        static string BinPath = Application.dataPath +"/../gperf.exe";
        static string OutputFolder = Application.dataPath + "/../MemberNameHash/";
        static string HeaderPath = Application.dataPath + "/Plugins/xlua_il2cpp/memberNameHash/memberNameHash.h";
        public static void Generate(IEnumerable<Type> typeList, bool includeNonPublic){
            if(Directory.Exists(OutputFolder)){
                Directory.Delete(OutputFolder, true);
            }
            if(!Directory.Exists(OutputFolder)){
                Directory.CreateDirectory(OutputFolder);
            }
            FileStream fs = new FileStream(HeaderPath, FileMode.OpenOrCreate);
            StreamWriter sw = new StreamWriter(fs);
            int count = typeList.Count();
            float n = 0f;
            foreach (var item in typeList)
            {
                n++;
                string typeName = item.FullName.Replace(".", "_");
                typeName = typeName.Replace("`", "");
                BindingFlags flag = BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public;
                if (includeNonPublic)
                {
                    flag = flag | BindingFlags.NonPublic;
                }
                var methods = item.GetMethods(flag); 
                var properties = item.GetProperties(flag); 
                var fields = item.GetFields(flag);
                var members = item.GetMembers(flag);
                string path = OutputFolder + typeName + ".gperf";
                string cPath = OutputFolder + typeName + ".c";
                var methodsName = from method in methods 
                        where ! method.IsSpecialName 
                        select method.Name;
                methodsName = methodsName.Distinct();
                var propertyName = from property in properties 
                        select property.Name;    

                var fieldName = from field in fields 
                        select field.Name;    

                string content = string.Join("\n", 
                methodsName.Concat(propertyName).Concat(fieldName).Distinct());
                
                
                File.WriteAllText(path, content);
                ProcessStartInfo processStartInfo = new ProcessStartInfo();
                processStartInfo.FileName = "cmd.exe";
                processStartInfo.CreateNoWindow = true;
                processStartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                processStartInfo.WorkingDirectory = new DirectoryInfo(Application.dataPath).Parent.FullName;
                processStartInfo.Arguments =   $"/c \"gperf.exe -H {typeName + "_hash"} {path}  > {cPath}\"";
                // UnityEngine.Debug.Log(processStartInfo.Arguments);
                processStartInfo.RedirectStandardError = true;
                processStartInfo.RedirectStandardOutput = true;
                processStartInfo.UseShellExecute = false;
                processStartInfo.StandardOutputEncoding = System.Text.Encoding.UTF8;
                processStartInfo.StandardErrorEncoding = System.Text.Encoding.UTF8;
                Process p = Process.Start(processStartInfo);
                
                string output = p.StandardOutput.ReadToEnd();   
                string error = p.StandardError.ReadToEnd();   
                p.WaitForExit();
                if(!string.IsNullOrEmpty(error)){
                    Debug.LogError(error);
                }
                //#TODO@benp 生成头文件
                if(File.Exists(cPath)){
                    //解析离线hash结果
                    var memberNameHashDic = PreProcessHashFile(cPath, cPath+".preprocess", typeName);
                    if(memberNameHashDic != null){
                        for (int i = 0; i < methods.Length; i++)
                        {
                            var method = methods[i];
                            bool hasVal = memberNameHashDic.TryGetValue(methods[i].Name, out int hash);
                            if(hasVal){
                                sw.WriteLine($@"int MethodHashWrap_{n}_{i}(lua_State* L){{
    auto wrapData = GetLuaClassRegister()->GetMemberWrapData({n}, {i});
    if(wrapData){{
        return MethodCallback(L, wrapData, 2);
    }}else{{
        //#TODO@benp raise exception
    }}
}}");
                            }else{
                                Debug.LogError($"member {methods[i].Name} not find int memberNameHash check hash file");
                            }
                        }
                    }
                    File.Copy(cPath+".preprocess", Application.dataPath +$"/Plugins/xlua_il2cpp/memberNameHash/{typeName}_hash.c", true);
                    
                    sw.WriteLine($@"unsigned int {typeName + "_hash"} (register const char *str, register unsigned int len);");
                }
                UnityEditor.EditorUtility.DisplayProgressBar("生成hash函数", typeName, n/(float)count);
            }
            WriteClassId(typeList, sw);
            sw.WriteLine($@"std::map<const char* , int> HashTypeIdDic = {{
     {string.Join("\n", typeList.Select((t, i)=>$"{{ \"{t.FullName}\", {i} }},"))}
}};
");
            sw.Close();
            fs.Close();
            UnityEditor.EditorUtility.ClearProgressBar();
        }

        private static void WriteClassId(IEnumerable<Type> types, StreamWriter sw){
            string content = $@"std::map<const char* , int, xlua::cmp_str> classMap = {{
    {String.Join("\n    ", types.Select((type, i) => $"{{ \"{type.Name}\", {i} }},"))}
}};";       
            sw.WriteLine(content);
        }

        private static Dictionary<string, int> PreProcessHashFile(string path , string newPath, string typeName){
            string content = File.ReadAllText(path);
            
            Match match = Regex.Match(content, @"static const char \* wordlist\[\] =\s*\{([^}]*)\};", RegexOptions.Singleline);
            Debug.Log(match.Success);
            Dictionary<string, int> dic = null;
            if (match.Success)
            {
                dic = new Dictionary<string, int>();
                var match2 = Regex.Matches(match.Groups[1].Value, @"""([^""]*)""", RegexOptions.Singleline);
                
                int n = 0 ;
                int emptyCnt = 0;
                foreach (var item in match2)
                {
                    dic[item.ToString()] = n++;
                    Debug.Log("insert dic "+ item.ToString()+" "+n);
                    if(item.ToString() == ""){
                        emptyCnt++;
                    }
                }
                Debug.LogWarning($"member hash cnt {match2.Count} emptyCnt {emptyCnt}");
            }else{
                
                Debug.LogError("memberNameHash parse failed ");
            }

            content = content.Replace("static unsigned int", "unsigned int");
            content = content.ReplaceFirst("return", "unsigned int __hVal =");
            content = content.Replace("register const char *str;", "");
            content = content.Replace("register unsigned int len;", "");
            content = content.Replace("#include <string.h>", "#include <string.h>\n#include \"memberNameHash.h\"");
            content = content.Replace("str, len", "register const char *str, register unsigned int len");
            content = content.ReplaceFirst(
@"}

#ifdef __GNUC__", 

$@"
  return __hVal - MIN_HASH_VALUE;
}}
unsigned int {typeName}_MaxLength() {{
    return MAX_HASH_VALUE - MIN_HASH_VALUE;
}}
/*
#ifdef __GNUC__");
            content += "*/";

            
            File.WriteAllText(newPath, content);
            return dic;
        }
    }

}