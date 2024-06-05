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
using System.Reflection;
namespace XLua
{
    public partial class ObjectTranslator
    {
        Dictionary<int, WeakReference> delegatMiddlewareCache = new Dictionary<int, WeakReference>();

        //#TODO@benp initsize
        internal HashSet<object> objectCache = new HashSet<object>();


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
            ObjectTranslator objectTranslator = ObjectTranslatorPool.Instance.Find(L);
            delegateMiddleware = new DelegateMiddleware() { L = L, reference = reference, FuncPtr = funcptr, luaLock = objectTranslator.luaEnv.luaEnvLock};
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

        public bool TryDelayWrapLoader_Il2Cpp(RealStatePtr L, Type type){
            if (loaded_types.ContainsKey(type)) return true;
            loaded_types.Add(type, true);
            LuaAPI.luaL_newmetatable(L, type.FullName); //先建一个metatable，因为加载过程可能会需要用到
            LuaAPI.lua_pop(L, 1);
            int top = LuaAPI.lua_gettop(L);
            TypeRegister.Register(L,type, privateAccessibleFlags.Contains(type));
            if (top != LuaAPI.lua_gettop(L))
            {
                throw new Exception("top change, before:" + top + ", after:" + LuaAPI.lua_gettop(L));
            }
            foreach (var nested_type in type.GetNestedTypes(BindingFlags.Public))
            {
                if (nested_type.IsGenericTypeDefinition())
                {
                    continue;
                }
                GetTypeId(L, nested_type);
            }
            
            return true;
        }

        internal object getLuaTable(RealStatePtr L, int idx)
        {
            if (LuaAPI.lua_type(L, idx) == LuaTypes.LUA_TUSERDATA)
            {
                object obj = SafeGetCSObj(L, idx);
                return (obj != null && obj is LuaTable) ? obj : null;
            }
            if (!LuaAPI.lua_istable(L, idx))
            {
                return null;
            }
            LuaAPI.lua_pushvalue(L, idx);
            return new LuaTable(LuaAPI.luaL_ref(L), luaEnv);
        }

        internal object getLuaFunction(RealStatePtr L, int idx)
        {
            if (LuaAPI.lua_type(L, idx) == LuaTypes.LUA_TUSERDATA)
            {
                object obj = SafeGetCSObj(L, idx);
                return (obj != null && obj is LuaFunction) ? obj : null;
            }
            if (!LuaAPI.lua_isfunction(L, idx))
            {
                return null;
            }
            LuaAPI.lua_pushvalue(L, idx);
            return new LuaFunction(LuaAPI.luaL_ref(L), luaEnv);
        }

    }
}

#endif