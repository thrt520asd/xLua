using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using XLua;
public class IL2CppTestMono : MonoBehaviour
{
    public string StreamingFilePath = "/14_IL2CPP.lua";
    // Start is called before the first frame update
    void Start()
    {
        LuaEnv l = new LuaEnv();
        var filePath = Application.streamingAssetsPath +StreamingFilePath;
        if(File.Exists(filePath)){
            var content = File.ReadAllText(filePath);
            l.DoString(content);
        }
        l.FullGc();
        GC.Collect();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
