using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Xml.Schema;
using Unity.Entities.UniversalDelegates;
using Unity.Mathematics;
using UnityEngine;
using Debug = UnityEngine.Debug;

namespace XLua.IL2CPP.Editor.Generator{
    public class MemberNameHashGen {
        static string BinPath = Application.dataPath +"/../gperf.exe";
        static string OutputFolder = Application.dataPath + "/../MemberNameHash/";
        static string HeaderPath = Application.dataPath + "/Plugins/xlua_il2cpp/memberNameHash/memberNameHash.h";
        static string HashWrapPath = Application.dataPath + "/Plugins/xlua_il2cpp/memberNameHash/HashWrap.h";
        static string OutPutFolder2 = Application.dataPath + "/Plugins/xlua_il2cpp/memberNameHash/";
        public static void Generate(IEnumerable<Type> typeList, bool includeNonPublic){
            if(Directory.Exists(OutputFolder)){
                Directory.Delete(OutputFolder, true);
            }
            if(!Directory.Exists(OutputFolder)){
                Directory.CreateDirectory(OutputFolder);
            }
            
            if(Directory.Exists(OutPutFolder2)){
                Directory.Delete(OutPutFolder2, true);
            }
            if(!Directory.Exists(OutPutFolder2)){
                Directory.CreateDirectory(OutPutFolder2);
            }
            FileStream fs = new FileStream(HeaderPath, FileMode.OpenOrCreate);
            StreamWriter sw = new StreamWriter(fs);
            FileStream hwFs = new FileStream(HashWrapPath, FileMode.OpenOrCreate);
            StreamWriter hwSw = new StreamWriter(hwFs);
            int count = typeList.Count();
            int n = 0;
            Dictionary<int, string> methodId2MethodName = new Dictionary<int, string>();
            foreach (var item in typeList)
            {
                
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
                var events = item.GetEvents(flag);
                string path = OutputFolder + typeName + ".gperf";
                string cPath = OutputFolder + typeName + ".h";
                var methodsName = from method in methods 
                        where ! method.IsSpecialName 
                        select method.Name;
                methodsName = methodsName.Distinct();
                var propertyName = from property in properties 
                        select property.Name;    

                var fieldName = from field in fields 
                        select field.Name;    
                var eventsName = from eventInfo in events 

                        select eventInfo.Name;    

                string content = string.Join("\n", 
                methodsName.Concat(propertyName).Concat(fieldName).Concat(eventsName).Distinct());
                
                
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
                if(File.Exists(cPath)){
                    //解析离线hash结果
                    var memberNameHashDic = PreProcessHashFile(cPath, cPath+".preprocess", typeName);
                    if(memberNameHashDic != null){
                        
                        foreach (var methodName in methodsName)
                        {
                            bool hasVal = memberNameHashDic.TryGetValue(methodName, out int hash);
                            if(hasVal){
                                hwSw.WriteLine($@"//{methodName}
int MethodHashWrap_{n}_{hash}(lua_State* L){{
    auto wrapData = GetLuaClassRegister()->GetMemberWrapData({n}, {hash});
    if(wrapData){{
        return MethodCallback(L, wrapData, 2);
    }}else{{
        //#TODO@benp raise exception
    }}
}}");
                            methodId2MethodName[n * 100000 + hash] = $"MethodHashWrap_{n}_{hash}";
                            }else{
                                Debug.LogError($"member {methodName} not find int memberNameHash check hash file");
                            }
                        }

                        foreach (var evtName in eventsName)
                        {
                            bool hasVal = memberNameHashDic.TryGetValue(evtName, out int hash);
                            if(hasVal){
                                hwSw.WriteLine($@"//{evtName}
int EventHashWrap_{n}_{hash}(lua_State* L){{
    auto wrapData = GetLuaClassRegister()->GetMemberEventWrapData({n}, {hash});
    if(wrapData){{
        return EventWrapCallBack_Hash(L, wrapData, 2);
    }}else{{
        //#TODO@benp raise exception
    }}
}}");
                            methodId2MethodName[n * 100000 + hash] = $"EventHashWrap_{n}_{hash}";
                            }else{
                                Debug.LogError($"member {evtName} not find int memberNameHash check hash file");
                            }
                        }
                    }

                    File.Copy(cPath+".preprocess", Application.dataPath +$"/Plugins/xlua_il2cpp/memberNameHash/{typeName}_hash.h", true);
                    
                    sw.WriteLine($@"unsigned int {typeName + "_hash"} (register const char *str, register unsigned int len);");
                    sw.WriteLine($@"#include ""{typeName}_hash.h""");
                }
                
                n++;
                UnityEditor.EditorUtility.DisplayProgressBar("生成hash函数", typeName, n/(float)count);
            }

            WriteClassId(typeList, hwSw);
            
            sw.WriteLine($@"static HashFuncs s_classMemberHashFunc [] = {{
    {string.Join("\n\t", typeList.Select(s=>$"{{(ClassMemberHashFunc){s}_hash,(HashLengthFunc){s}_MaxLength}},"))}
}};");

            hwSw.WriteLine($@"std::map<int, HashWrapFunc> s_HashWrapDic = {{
    {string.Join("\n\t", methodId2MethodName.Select((k)=>$"{{{k.Key}, (HashWrapFunc){k.Value}}},"))}
}};");
            hwSw.Close();
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
            int minHashValue = -1;
            Match match = Regex.Match(content, @"static const char \* wordlist\[\] =\s*\{([^}]*)\};", RegexOptions.Singleline);
            
            Match minValueMatch = Regex.Match(content, @"#define\s+MIN_HASH_VALUE\s+(\d+)", RegexOptions.Singleline);
            if(minValueMatch.Success){
                string value = minValueMatch.Groups[1].Value;
                int.TryParse(value, out minHashValue);
            }
            if(minHashValue == -1) return null;

            Dictionary<string, int> dic = null;
            if (match.Success)
            {
                dic = new Dictionary<string, int>();
                var match2 = Regex.Matches(match.Groups[1].Value, @"""([^""]*)""", RegexOptions.Singleline);
                
                int n = 0 ;
                int emptyCnt = 0;
                foreach (Match item in match2)
                {
                    string res = item.Groups[1].Value;;
                    dic[res] = n++ - minHashValue;
                    
                    if(res == ""){
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
    return MAX_HASH_VALUE - MIN_HASH_VALUE + 1;
}}
/*
#ifdef __GNUC__");
            content += "*/";
            File.WriteAllText(newPath, content);
            return dic;
        }
    }

}