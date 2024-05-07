using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEngine.Profiling;
using UnityEngine.UI;
using XLua;
public class Perf_LuaCallCSharp_MethodInvoke : MonoBehaviour
{
    public string StreamingFilePath = "/14_IL2CPP.lua";
    public Button btn;
    private LuaEnv env;
    // Start is called before the first frame update
    void Start()
    {
        env = new LuaEnv();
        var filePath = Application.streamingAssetsPath +StreamingFilePath;
        if(File.Exists(filePath)){
            var content = File.ReadAllText(filePath);
            env.DoString(content);
        }
        btn.onClick.AddListener(()=>{
            var func = env.Global.Get<Action>("TestFunc");
            Profiler.BeginSample("Perf_LuaCallCSharp_MethodInvoke");
            func();
            Profiler.EndSample();
        });
    }
}
