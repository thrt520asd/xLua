using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using XLua;
public class IL2CppTestMono : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        LuaEnv l = new LuaEnv();
        
        l.DoString(@"
            local cls = CS.IL2CPPTest
            print(cls)
            for k,v in pairs(cls) do
                print(k,v)
            end
            local meta = getmetatable(cls)
            for k,v in pairs(meta) do
                print(""metatable"",k,v)
            end
            
            local f = meta.__call
            local up1 = debug.getupvalue(f, 1)
            print(""up1"", up1)
            local res = cls()
            print(res)
        ");
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
