using System;
using System.Runtime.CompilerServices;
namespace XLua.IL2CPP
{
    /// <summary>
    /// C++侧使用
    /// </summary>
    public class LuaObject
    {
        IntPtr L;
        int reference;

        [MethodImpl(MethodImplOptions.InternalCall)]
        void ReleaseLuaRef()
        {
            throw new NotImplementedException();
        }

        ~LuaObject()
        {
            ReleaseLuaRef();
        }
    }
}