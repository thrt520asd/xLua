using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using UnityEngine;

namespace XLua.IL2CPP.Editor.Generator{
    public class MemberNameHashGen {
        static string BinPath = Application.dataPath +"/../gperf.exe";
        static string OutputFolder = Application.dataPath + "/../MemberNameHash/";
        static string HeaderPath = Application.dataPath + "/Plugins/xlua_il2cpp/memberNameHash/memberNameHash.h";
        public static void Generate(IEnumerable<Type> typeList){
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
                string typeName = item.Name;
                typeName = typeName.Replace("`", "");
                var members = item.GetMembers(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic);
                string path = OutputFolder + typeName + "_instance.gperf";
                string cPath = OutputFolder + typeName + "_instance.c";
                string content = string.Join("\n", members.Select(m=>{
                    //#TODO@benp 参数过滤
                    return m.Name;
                }));
                File.WriteAllText(path, content);
                ProcessStartInfo processStartInfo = new ProcessStartInfo();
                processStartInfo.FileName = "cmd.exe";
                processStartInfo.CreateNoWindow = true;
                processStartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                processStartInfo.WorkingDirectory = new DirectoryInfo(Application.dataPath).Parent.FullName;
                processStartInfo.Arguments =   $"/c \"gperf.exe -c -S1 -I -o -D -H {typeName + "_instance_hash"} {path}  > {cPath}\"";
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
                // UnityEngine.Debug.Log(output);
                if(!string.IsNullOrEmpty(error)){
                    UnityEngine.Debug.LogError(error);
                }
                //#TODO@benp 生成头文件
                if(File.Exists(cPath)){
                    //#TODO@benp 删除in_word_set
                    
                    File.Copy(cPath, Application.dataPath +$"/Plugins/xlua_il2cpp/memberNameHash/{typeName}_instance.c", true);
                    
                    sw.WriteLine($@"static unsigned int
{typeName + "_instance_hash"} (register const char *str, register unsigned int len);");
                }
                UnityEditor.EditorUtility.DisplayProgressBar("生成hash函数", typeName, n/(float)count);
            }
            sw.Close();
            fs.Close();
            UnityEditor.EditorUtility.ClearProgressBar();
        }
    }
}