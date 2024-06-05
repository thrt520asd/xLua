#if IL2CPP_ENHANCED_LUA_DEVELOP
#define ENABLE_IL2CPP
#endif
#if IL2CPP_ENHANCED_LUA && ENABLE_IL2CPP
using RealStatePtr = System.IntPtr;
using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Reflection;
using XLua.LuaDLL;
using LuaAPI = XLua.LuaDLL.Lua;
using UnityEngine;
using LuaCSFunction = XLua.LuaDLL.lua_CSFunction;
using System.Text;
namespace XLua.IL2CPP
{
    public class TypeRegister
    {
        
        /// call from native
        public static int AddObj(RealStatePtr L, object obj){
            var translator = ObjectTranslatorPool.Instance.Find(L);
            if (translator != null)
            {
                return translator.objects.Add(obj);
            }
            return -1;
        }
        /// call from native
        public static object RemoveObj(RealStatePtr L, int idx){
            var translator = ObjectTranslatorPool.Instance.Find(L);
            if (translator != null)
            {
                return translator.objects.Remove(idx);
            }
            return null;
        }

        public static bool RefObj(RealStatePtr L, object obj){
            var translator = ObjectTranslatorPool.Instance.Find(L);
            if (translator != null)
            {
                return translator.objectCache.Add(obj);
            }
            return false;
        }
        /// call from native
        public static bool UnRefObj(RealStatePtr L, object obj){
            var translator = ObjectTranslatorPool.Instance.Find(L);
            if (translator != null)
            {
                return translator.objectCache.Remove(obj);
            }
            return false;
        }

        /// call from native
        public static Delegate GetDelegate(RealStatePtr L, int referenced, Type type, IntPtr funcptr){
            var translator = ObjectTranslatorPool.Instance.Find(L);
            if (translator != null)
            {
                return translator.GetDelegate(L, referenced, type, funcptr);
            }
            return null;
        }

        /// <summary>
        /// call from native
        /// </summary>
        /// <param name="L"></param>
        /// <param name="t"></param>
        /// <returns></returns>
        public static int StaticGetTypeId(RealStatePtr L, Type t)
        {
            var translator = ObjectTranslatorPool.Instance.Find(L);
            if (t != null)
            {
                return translator.GetTypeId(L, t);
            }
            return -1;
        }

        public static object FallBackLua2CSObj(RealStatePtr L,int index,  Type t){
            var translator = ObjectTranslatorPool.Instance.Find(L);
            if (t != null)
            {
                return translator.objectCasters.GetCaster(t)(L, index, null);
            }
            return null;
        }
        

        //#TODO@benp C++类型释放

        private static IntPtr GetFieldWrapper(string name, bool isStatic, string signature)
        {
            return NativeAPI.FindFieldWrap(signature);
        }

        private static IntPtr GetWrapperFunc(MemberInfo member, string signature)
        {
            return NativeAPI.FindWrapFunc(signature);
        }

        public static void SetTypeMetaId(Type type, int metaId)
        {
            var typeId = NativeAPI.GetTypeId(type);
            NativeAPI.SetClassMetaId(typeId, metaId);
        }

        public static string Bytes2String(byte[] bytes){
            return Encoding.UTF8.GetString(bytes);
        }

        private static HashSet<Type> loaded_type = new HashSet<Type>();
        private static bool setcache = false;
        //#TODO@benp 待整理
        public static void Register(RealStatePtr L, Type type, bool includeNonPublic, bool throwIfMemberFail = false)
        {
            var translator = ObjectTranslatorPool.Instance.Find(L);
            if(loaded_type.Contains(type)){
                var typeId = NativeAPI.GetTypeId(type);
                Register2Lua(type, 0, typeId, translator, L);
                return;
            }
            
            if (!setcache)
            {
                setcache = true;
                
                NativeAPI.SetXLuaRef(new int[]{translator.cacheRef, translator.luaEnv.errorFuncRef});
                var StaticGetTypeIdMethod = typeof(TypeRegister).GetMethod("StaticGetTypeId", BindingFlags.Static | BindingFlags.Public);

                var AddObjMethod = typeof(TypeRegister).GetMethod("AddObj", BindingFlags.Static | BindingFlags.Public);
                var RemoveObjMethod = typeof(TypeRegister).GetMethod("RemoveObj", BindingFlags.Static | BindingFlags.Public);

                var CombineDelegateMethod = typeof(Delegate).GetMethod("Combine",new Type[] {typeof(Delegate), typeof(Delegate)});
                var RemoveDelegateMethod = typeof(Delegate).GetMethod("Remove", BindingFlags.Static | BindingFlags.Public);
                
                var Bytes2StringMethod = typeof(TypeRegister).GetMethod("Bytes2String", BindingFlags.Static | BindingFlags.Public);

                var FallBackLua2CSObjMethod = typeof(TypeRegister).GetMethod("FallBackLua2CSObj", BindingFlags.Static | BindingFlags.Public);

                var GetDelegateMethod = typeof(TypeRegister).GetMethod("GetDelegate", BindingFlags.Static | BindingFlags.Public);

                var RefObjMethod = typeof(TypeRegister).GetMethod("RefObj", BindingFlags.Static | BindingFlags.Public);
                var UnRefObjMethod = typeof(TypeRegister).GetMethod("UnRefObj", BindingFlags.Static | BindingFlags.Public);

                NativeAPI.SetCSharpAPI(new MethodBase[]{StaticGetTypeIdMethod, AddObjMethod, RemoveObjMethod, CombineDelegateMethod, RemoveDelegateMethod,  Bytes2StringMethod, FallBackLua2CSObjMethod, GetDelegateMethod, RefObjMethod, UnRefObjMethod});
                
                NativeAPI.SetGlobalType_LuaObject(typeof(DelegateMiddleware));
            }
            BindingFlags flag = BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public;
            if (includeNonPublic)
            {
                flag = flag | BindingFlags.NonPublic;
            }

            var cppClsDef = Register2Cpp(L, type, type.GetConstructors(flag), type.GetMethods(flag), type.GetProperties(flag), type.GetFields(flag), throwIfMemberFail);

            if (cppClsDef != IntPtr.Zero)
            {
                //push to lua
                Register2Lua(type, 0, cppClsDef, translator, L);
                loaded_type.Add(type);
            }
        }

        private static void Register2Lua(Type type, int type_id, IntPtr typeId, ObjectTranslator translator, RealStatePtr L)
        {
            //todo hotfix 处理
            if (type == null)
            {
                if (type_id == -1) throw new Exception("Fatal: must provide a type of type_id");
                LuaAPI.xlua_rawgeti(L, LuaIndexes.LUA_REGISTRYINDEX, type_id);
            }
            else
            {
                LuaAPI.luaL_getmetatable(L, type.FullName);
                if (LuaAPI.lua_isnil(L, -1))
                {
                    LuaAPI.lua_pop(L, 1);
                    LuaAPI.luaL_newmetatable(L, type.FullName);
                }
            }

            //xlua 标记
            LuaAPI.lua_pushlightuserdata(L, LuaAPI.xlua_tag());
            LuaAPI.lua_pushnumber(L, 1);
            LuaAPI.lua_rawset(L, -3); // 1 objMeta 2 tag 3 1
            int objMeta = LuaAPI.lua_gettop(L);
            if(typeof(MulticastDelegate).IsAssignableFrom(type)){
                NativeAPI.HandleDeleagateMetatable(L, -3, typeId);
            }
            else{
                NativeAPI.HandleObjMetatable(L, -3, typeId);

                LuaAPI.lua_createtable(L, 0, 0); // 1 objMeta 2 clsTable

                int cls_table = LuaAPI.lua_gettop(L);

                Utils.SetCSTable(L, type, cls_table);
                LuaAPI.lua_createtable(L, 0, 3); // // 1 objMeta 2 clsTable 3 clsMeta
                int meta_table = LuaAPI.lua_gettop(L);

                NativeAPI.HandleClsMetaTable(L, meta_table, typeId);

                LuaAPI.lua_pushvalue(L, meta_table); // 1 objMeta 2 clsTable 3 clsMeta 4 clsMeta
                LuaAPI.lua_setmetatable(L, cls_table); //  setmetatable(clsTable, clsMeta[4])  1 objMeta 2 clsTable 3 clsMeta
                LuaAPI.lua_pop(L, 3);
            }

        }

        /// 注册类型信息到IL2CPP,
        private static IntPtr Register2Cpp(IntPtr L, Type type, MethodBase[] ctors = null, MethodBase[] methods = null, PropertyInfo[] properties = null, FieldInfo[] fields = null, bool throwIfMemberFail = false)
        {
            IntPtr typeInfo = IntPtr.Zero;
            try
            {
                bool isDelegate = typeof(MulticastDelegate).IsAssignableFrom(type) && type != typeof(MulticastDelegate);
                var superTypeId = (isDelegate || type == typeof(object) || type.BaseType == null) ? IntPtr.Zero : NativeAPI.GetTypeId(type.BaseType);
                var typeId = NativeAPI.GetTypeId(type);
                // UnityEngine.Debug.Log("typeId" + typeId);
                //create C++ struct 
                typeInfo = NativeAPI.CreateCSharpTypeInfo(type.ToString(), typeId, superTypeId, typeId, type.IsValueType, isDelegate, isDelegate ? TypeUtils.GetMethodSignature(type.GetMethod("Invoke"), true) : "");
                if (typeInfo == IntPtr.Zero)
                {
                    if (isDelegate) throw new Exception(string.Format("create TypeInfo for {0} fail. maybe the BridgeInfo is not found, try to regenerate the FunctionBridge.Gen.h", type));
                    throw new Exception(string.Format("create TypeInfo for {0} fail", type));
                }

                if (!isDelegate)
                {

                    //constructor
                    if (ctors != null && ctors.Length > 0 && (!type.IsArray || type == typeof(System.Array)))
                    {
                        foreach (var ctor in ctors)
                        {
                            if (ctor.IsGenericMethodDefinition) continue;
                            List<Type> usedTypes = TypeUtils.GetUsedTypes(ctor);
                            var signature = TypeUtils.GetMethodSignature(ctor);

                            var wrapper = GetWrapperFunc(ctor, signature);
                            if (wrapper == IntPtr.Zero)
                            {
                                UnityEngine.Debug.LogWarning(string.Format("wrapper{1} is null for {0}", type, signature));
                                continue;
                            }
                            // UnityEngine.Debug.Log(string.Format("add ctor {0}, usedTypes count: {1}", ctor, usedTypes.Count));

                            var methodInfoPointer = NativeAPI.GetMethodInfoPointer(ctor);
                            var methodPointer = NativeAPI.GetMethodPointer(ctor);
                            if (methodInfoPointer == IntPtr.Zero)
                            {
                                UnityEngine.Debug.LogWarning(string.Format("cannot get method info for {0}:{1}, signature:{2}", type, ctor, TypeUtils.GetMethodSignature(ctor)));
                                continue;
                            }
                            if (methodPointer == IntPtr.Zero)
                            {
                                UnityEngine.Debug.LogWarning(string.Format("cannot get method pointer for {0}:{1}, signature:{2}", type, ctor, TypeUtils.GetMethodSignature(ctor)));
                                continue;
                            }
                            var wrapData = NativeAPI.AddConstructor(
                                typeInfo,
                                signature,
                                wrapper,
                                methodInfoPointer,
                                methodPointer,
                                usedTypes.Count
                            );
                            if (wrapData == IntPtr.Zero)
                            {
                                if (!throwIfMemberFail)
                                {
#if WARNING_IF_MEMBERFAIL
                                    UnityEngine.Debug.LogWarning(string.Format("add constructor for {0} fail, signature:{1}", type, TypeUtils.GetMethodSignature(ctor)));
                                    
#endif
                                    continue;
                                }
                                throw new Exception(string.Format("add constructor for {0} fail, signature:{1}", type, TypeUtils.GetMethodSignature(ctor)));
                            }
                            for (int i = 0; i < usedTypes.Count; ++i)
                            {
                                var typeInfoPointer = NativeAPI.GetTypeId(usedTypes[i]);
                                // UnityEngine.Debug.Log(string.Format("set used type for ctor {0}: {1}={2}, typeId:{3}", ctor, i, usedTypes[i], typeInfoPointer));
                                NativeAPI.SetTypeInfo(wrapData, i, typeInfoPointer);
                            }
                        }
                    }
                    // add method delegate
                    Action<string, MethodInfo, bool, bool, bool> AddMethodToType = (string name, MethodInfo method, bool isGetter, bool isSetter, bool isExtensionMethod) =>
                    {
                        method = TypeUtils.HandleMaybeGenericMethod(method);
                        if (method == null) return;
                        List<Type> usedTypes = TypeUtils.GetUsedTypes(method, isExtensionMethod);
                        var signature = TypeUtils.GetMethodSignature(method, false, isExtensionMethod);
                        // UnityEngine.Debug.Log(string.Format("add method {0}, usedTypeCount:{1} name: {2} isGetter{3} isSetter{4}", method, usedTypes.Count, name, isGetter, isSetter));

                        var wrapper = GetWrapperFunc(method, signature);
                        if (wrapper == IntPtr.Zero)
                        {
                            UnityEngine.Debug.LogWarning(string.Format("wrapper is null for {0}:{1}, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, isExtensionMethod)));
                            return;
                        }

                        var methodInfoPointer = NativeAPI.GetMethodInfoPointer(method);
                        var methodPointer = NativeAPI.GetMethodPointer(method);
                        if (methodInfoPointer == IntPtr.Zero)
                        {
                            UnityEngine.Debug.LogWarning(string.Format("cannot get method info for {0}:{1}, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, isExtensionMethod)));
                            return;
                        }
                        if (methodPointer == IntPtr.Zero)
                        {
                            UnityEngine.Debug.LogWarning(string.Format("cannot get method pointer for {0}:{1}, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, isExtensionMethod)));
                            return;
                        }
                        var wrapData = NativeAPI.AddMethod(
                            typeInfo,
                            signature,
                            wrapper,
                            name,
                            !isExtensionMethod && method.IsStatic,
                            isExtensionMethod,
                            isGetter,
                            isSetter,
                            methodInfoPointer,
                            methodPointer,
                            usedTypes.Count
                        );
                        if (wrapData == IntPtr.Zero)
                        {
                            if (throwIfMemberFail)
                            {
                                throw new Exception(string.Format("add method for {0}:{1} fail, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, isExtensionMethod)));
                            }
                            else
                            {
#if WARNING_IF_MEMBERFAIL
                                UnityEngine.Debug.LogWarning(string.Format("add method for {0}:{1} fail, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, isExtensionMethod)));
#endif
                                return;
                            }
                        }
                        for (int i = 0; i < usedTypes.Count; ++i)
                        {
                            var usedTypeId = NativeAPI.GetTypeId(usedTypes[i]);
                            // UnityEngine.Debug.Log(string.Format("set used type for method {0}: {1}={2}, typeId:{3}", method, i, usedTypes[i], usedTypeId));
                            NativeAPI.SetTypeInfo(wrapData, i, usedTypeId);
                        }
                    };

                    if (methods != null && (!type.IsArray || type == typeof(System.Array)))
                    {
                        foreach (var method in methods)
                        {
                            if (method.IsAbstract) continue;
                            // if (method.IsSpecialName) continue;
                            AddMethodToType(method.Name, method as MethodInfo, false, false, false);
                        }
                    }

                    

                    // add property delegate
                    Action<string, MethodInfo, bool> AddPropertyToType = (string name, MethodInfo method, bool isGetter) =>
                    {
                        method = TypeUtils.HandleMaybeGenericMethod(method);
                        if (method == null) return;
                        List<Type> usedTypes = TypeUtils.GetUsedTypes(method, false);
                        var signature = TypeUtils.GetMethodSignature(method, false, false);
                        // UnityEngine.Debug.Log(string.Format("add property {0}, usedTypeCount:{1} name: {2} isGetter{3} ", name, usedTypes.Count, name, isGetter));

                        var wrapper = GetWrapperFunc(method, signature);
                        if (wrapper == IntPtr.Zero)
                        {
                            UnityEngine.Debug.LogWarning(string.Format("wrapper is null for {0}:{1}, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, false)));
                            return;
                        }

                        var methodInfoPointer = NativeAPI.GetMethodInfoPointer(method);
                        var methodPointer = NativeAPI.GetMethodPointer(method);
                        if (methodInfoPointer == IntPtr.Zero)
                        {
                            UnityEngine.Debug.LogWarning(string.Format("cannot get method info for {0}:{1}, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, false)));
                            return;
                        }
                        if (methodPointer == IntPtr.Zero)
                        {
                            UnityEngine.Debug.LogWarning(string.Format("cannot get method pointer for {0}:{1}, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, false)));
                            return;
                        }
                        var wrapData = NativeAPI.AddProperty(
                            typeInfo,
                            signature,
                            wrapper,
                            name,
                            method.IsStatic,
                            isGetter,
                            methodInfoPointer,
                            methodPointer,
                            usedTypes.Count
                        );
                        if (wrapData == IntPtr.Zero)
                        {
                            if (throwIfMemberFail)
                            {
                                throw new Exception(string.Format("AddProperty for {0}:{1} fail, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, false)));
                            }
                            else
                            {
#if WARNING_IF_MEMBERFAIL
                                UnityEngine.Debug.LogWarning(string.Format("AddProperty for {0}:{1} fail, signature:{2}", type, method, TypeUtils.GetMethodSignature(method, false, false)));
#endif
                                return;
                            }
                        }
                        for (int i = 0; i < usedTypes.Count; ++i)
                        {
                            var usedTypeId = NativeAPI.GetTypeId(usedTypes[i]);
                            // UnityEngine.Debug.Log(string.Format("set used type for method {0}: {1}={2}, typeId:{3}", method, i, usedTypes[i], usedTypeId));
                            NativeAPI.SetTypeInfo(wrapData, i, usedTypeId);
                        }
                    };


                    //extension method  
                    var extensionMethods = Utils.GetExtensionMethodsOf(type);
                    if (extensionMethods != null)
                    {
                        
                        foreach (var method in extensionMethods)
                        {
                            if(type.IsArray || type == typeof(System.Array) && (method.Name == ("get_Item") || method.Name == "set_Item" )){
                                // Debug.Log("TypeReigster Add Array indexer "+method.ToString());
                                AddPropertyToType("Item", method, method.Name == "get_Item");
                            }else{
                                AddMethodToType(method.Name, method as MethodInfo, false, false, true);
                            }
                        }
                    }

                    if (properties != null)
                    {
                        foreach (var prop in properties)
                        {
                            var getter = prop.GetGetMethod();
                            if (getter != null && !getter.IsGenericMethodDefinition && !getter.IsAbstract)
                            {
                                AddPropertyToType(prop.Name, getter, true);
                            }
                            var setter = prop.GetSetMethod();
                            if (setter != null && !setter.IsGenericMethodDefinition && !setter.IsAbstract)
                            {
                                AddPropertyToType(prop.Name, setter, false);
                            }
                        }
                    }

                    if (fields != null)
                    {
                        foreach (var field in fields)
                        {
                            // skip hotfix inject field
                            if (field.IsStatic && (field.Name.StartsWith("__Hotfix") || field.Name.StartsWith("_c__Hotfix")) && typeof(Delegate).IsAssignableFrom(field.FieldType))
                            {
                                continue;
                            }
                            string signature = (field.IsStatic ? "" : TypeUtils.GetThisSignature2(type)) + TypeUtils.GetTypeSignature(field.FieldType);
                            var name = field.Name;

                            var wrapper = GetFieldWrapper(name, field.IsStatic, signature);
                            if (wrapper == IntPtr.Zero)
                            {
                                UnityEngine.Debug.LogWarning(string.Format("wrapper is null for {0}:{1}, signature:{2}", type, name, signature));
                                continue;
                            }

                            if (!NativeAPI.AddField(
                                typeInfo,
                                wrapper,
                                name,
                                field.IsStatic,
                                NativeAPI.GetFieldInfoPointer(field),
                                NativeAPI.GetFieldOffset(field, type.IsValueType),
                                NativeAPI.GetTypeId(field.FieldType))
                            )
                            {
                                if (!throwIfMemberFail)
                                {
#if WARNING_IF_MEMBERFAIL
                                    UnityEngine.Debug.LogWarning(string.Format("add field for {0}:{1} fail, signature:{2}", type, field, signature));
#endif
                                    continue;
                                }
                                throw new Exception(string.Format("add field for {0}:{1} fail, signature:{2}", type, field, signature));
                            }
                            // UnityEngine.Debug.Log(string.Format("AddField {0} of {1} ok offset={2}", field, type, NativeAPI.GetFieldOffset(field, type.IsValueType)));
                        }
                    }
                }
                if (NativeAPI.RegisterLuaClass(typeInfo, type.FullName))
                {
                    return typeId;
                }
                else
                {
                    throw new Exception(string.Format("Register for {0} fail", type));
                }
            }
            catch (Exception e)
            {
                UnityEngine.Debug.Log("Register 2 CPP Exception " + e.ToString());
                NativeAPI.ReleaseCSharpTypeInfo(typeInfo);
                throw e;
            }
        }
    }
}
#endif
