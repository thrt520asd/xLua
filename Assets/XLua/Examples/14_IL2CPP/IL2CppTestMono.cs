using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using XLua;
public class IL2CppTestMono : MonoBehaviour
{
    public string StreamingFilePath = "/14_IL2CPP.lua";
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
        
    }

    // Update is called once per frame
    void Update()
    {
        if(GUILayout.Button("Start")){
            
        }
    }
}
