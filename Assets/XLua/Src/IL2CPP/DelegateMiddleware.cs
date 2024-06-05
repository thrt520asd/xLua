#if IL2CPP_ENHANCED_LUA_DEVELOP
#define ENABLE_IL2CPP
#endif
#if IL2CPP_ENHANCED_LUA && ENABLE_IL2CPP
using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace XLua.IL2CPP
{
    /// <summary>
    /// C++侧使用
    /// </summary>
    public class DelegateMiddleware
    {
        public IntPtr L;
        public IntPtr FuncPtr;
        public object luaLock;
        public int reference;

        Dictionary<Type, Delegate> bindTo = null;
        public Type firstKey = null;
        public Delegate firstValue = null;

        void ReleaseLuaRef()
        {
            if(reference == -1) 
            {
                return;
            }
            ObjectTranslatorPool.Instance.Find(L)?.luaEnv.equeueDelegateMiddlewareGCAction(new LuaEnv.GCAction(){Reference = reference});
            reference = -1;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public Delegate CreateDelegate(Type type){
            throw new NotImplementedException();
        }

        ~DelegateMiddleware()
        {
            ReleaseLuaRef();
        }

        /// <summary>
        /// 获取或创建一个委托实例。
        /// </summary>
        /// <param name="key">委托的类型。</param>
        /// <returns>返回一个对应于指定类型的委托实例。</returns>
        public Delegate GetOrCreateDelegate(Type key){
            if(TryGetDelegate(key, out var value)){
                return value;
            }
            Delegate d = CreateDelegate(key);
            AddDelegate(key, d);
            return d;
        }

        public bool TryGetDelegate(Type key, out Delegate value)
        {
            if(key == firstKey)
            {
                value = firstValue;
                return true;
            }
            if (bindTo != null)
            {
                return bindTo.TryGetValue(key, out value);
            }
            value = null;
            return false;
        }

        public void AddDelegate(Type key, Delegate value)
        {
            if (key == firstKey)
            {
                throw new ArgumentException("An element with the same key already exists in the dictionary.");
            }

            if (firstKey == null && bindTo == null) // nothing 
            {
                firstKey = key;
                firstValue = value;
            }
            else if (firstKey != null && bindTo == null) // one key existed
            {
                bindTo = new Dictionary<Type, Delegate>();
                bindTo.Add(firstKey, firstValue);
                firstKey = null;
                firstValue = null;
                bindTo.Add(key, value);
            }
            else
            {
                bindTo.Add(key, value);
            }
        }
    }
}
#endif