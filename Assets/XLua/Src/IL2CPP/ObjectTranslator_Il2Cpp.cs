using RealStatePtr = System.IntPtr;
using LuaCSFunction = XLua.LuaDLL.lua_CSFunction;
using System.Collections.Generic;
using System;
using System.Diagnostics;
namespace XLua
{
    public partial class ObjectTranslator
    {
        Dictionary<int, WeakReference> delegate_cache = new Dictionary<int, WeakReference>();
        public object GetCacheDelegate(RealStatePtr L, int referenced){
            UnityEngine.Debug.Log("GetCacheDelegate" + referenced);
            WeakReference weakReference;
            if(delegate_cache.TryGetValue(referenced, out weakReference)){
                if (weakReference.IsAlive)
                {
                    UnityEngine.Debug.Log("GetCacheDelegate" + referenced + "success");
                    return weakReference.Target;
                }
            }
            UnityEngine.Debug.Log("GetCacheDelegate" + referenced + "failed");
            return null;
        }

        public void CacheDelegate(RealStatePtr L, int referenced , object cppDelegate){
            if(delegate_cache.ContainsKey(referenced)){
                UnityEngine.Debug.LogWarning("delegate_cache has already contains referenced" + referenced);
            }
            UnityEngine.Debug.Log("CacheDelegate" + referenced + cppDelegate);
            delegate_cache[referenced] = new WeakReference(cppDelegate);
        }


    }    
}
