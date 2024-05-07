using System;
using System.Collections.Generic;
namespace XLua.IL2CPP{
    public class DelegateCache 
    {
        private Type firstKey = null;

        private Delegate firstValue = null;

        private Dictionary<Type, Delegate> bindTo = null;

        protected int errorFuncRef;



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