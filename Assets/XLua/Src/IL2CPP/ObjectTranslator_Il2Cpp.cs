#if IL2CPP_ENHANCED_LUA_DEVELOP
#define ENABLE_IL2CPP
#endif
#if IL2CPP_ENHANCED_LUA && ENABLE_IL2CPP
using RealStatePtr = System.IntPtr;
using LuaCSFunction = XLua.LuaDLL.lua_CSFunction;
using System.Collections.Generic;
using System;
using System.Diagnostics;
using XLua.IL2CPP;
using LuaAPI = XLua.LuaDLL.Lua;
namespace XLua
{
    public partial class ObjectTranslator
    {
        Dictionary<int, WeakReference> delegatMiddlewareCache = new Dictionary<int, WeakReference>();

        HashSet<object> objectCache = new HashSet<object>();


        public Delegate GetDelegate(RealStatePtr L, int idx, Type type, IntPtr funcptr)
        {
            LuaAPI.lua_pushvalue(L, idx);
            LuaAPI.lua_rawget(L, LuaIndexes.LUA_REGISTRYINDEX);

            DelegateMiddleware delegateMiddleware;
            if (!LuaAPI.lua_isnil(L, -1))
            {
                int referenced = LuaAPI.xlua_tointeger(L, -1);
                LuaAPI.lua_pop(L, 1);
                if(delegatMiddlewareCache.TryGetValue(referenced, out var weakReference)){
                    if(weakReference.IsAlive){
                        delegateMiddleware= weakReference.Target as DelegateMiddleware;
                        return delegateMiddleware.GetOrCreateDelegate(type);
                    }else{
                        // UnityEngine.Debug.Log("delegate in not alive" + referenced);    
                    }
                }else{
                    // UnityEngine.Debug.Log("GetDelegate failed lua funciont cache in registry but no valid DelegateMiddleware" + referenced);
                }
            }
            else
            {
                LuaAPI.lua_pop(L, 1);
            }

            LuaAPI.lua_pushvalue(L, idx);
            int reference = LuaAPI.luaL_ref(L);
            LuaAPI.lua_pushvalue(L, idx);
            LuaAPI.lua_pushnumber(L, reference);
            LuaAPI.lua_rawset(L, LuaIndexes.LUA_REGISTRYINDEX);

            delegateMiddleware = new DelegateMiddleware() { L = L, reference = reference, FuncPtr = funcptr};
            var value = delegateMiddleware.CreateDelegate(type);
            delegateMiddleware.AddDelegate(type,value );
            delegatMiddlewareCache[reference] = new WeakReference(delegateMiddleware);
            return value;
        }

        public void ReleaseDelegateMiddleWare(RealStatePtr L, int reference){
            LuaAPI.xlua_rawgeti(L, LuaIndexes.LUA_REGISTRYINDEX, reference); // -1 func
            if (LuaAPI.lua_isnil(L, -1))
            {
                LuaAPI.lua_pop(L, 1);
            }
            else
            {
                LuaAPI.lua_pushvalue(L, -1); // -1 func -2 func
                LuaAPI.lua_rawget(L, LuaIndexes.LUA_REGISTRYINDEX); // -1 result -2 func
                if (LuaAPI.lua_type(L, -1) == LuaTypes.LUA_TNUMBER && LuaAPI.xlua_tointeger(L, -1) == reference) //
                {
                    // UnityEngine.Debug.LogWarning("ReleaseDelegateMiddleWare from registry " + reference);
                    LuaAPI.lua_pop(L, 1);// pop LUA_REGISTRYINDEX[func] // -1 func
                    LuaAPI.lua_pushnil(L); // -1 nil -2 func
                    LuaAPI.lua_rawset(L, LuaIndexes.LUA_REGISTRYINDEX); // LUA_REGISTRYINDEX[func] = nil 
                }
                else //another Delegate ref the function before the GC tick
                {
                    LuaAPI.lua_pop(L, 2); // pop LUA_REGISTRYINDEX[func] & func
                }
            }

            LuaAPI.lua_unref(L, reference);
            delegatMiddlewareCache.Remove(reference);
        }
    }
}

#endif