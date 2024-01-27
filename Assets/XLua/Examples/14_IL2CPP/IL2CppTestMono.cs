using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using XLua;
public class IL2CppTestMono : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        // IL2CPPTestBase b = new IL2CPPTestBase();
        // b.TestIL2CPPGC();
        LuaEnv l = new LuaEnv();
        var filePath = Application.streamingAssetsPath +"/14_IL2CPP.lua";
        if(File.Exists(filePath)){
            var content = File.ReadAllText(filePath);
            l.DoString(content);
        }
        // l.DoString(@"
        //     local cls = CS.IL2CPPTest
        //     print(cls)
        //     for k,v in pairs(cls) do
        //         print(k,v)
        //     end
        //     local meta = getmetatable(cls)
        //     for k,v in pairs(meta) do
        //         print(""metatable"",k,v)
        //     end
            
        //     local f = meta.__call
        //     local res = cls()
        //     print(res)
        //     local metaTable = getmetatable(res)           
        //     print(metaTable)
        //     for k,v in pairs(metaTable) do
        //         print(""objMeta"",k,v)
        //     end
        // ");
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
