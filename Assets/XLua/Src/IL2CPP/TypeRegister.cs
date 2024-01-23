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
namespace XLua.IL2CPP
{
    public class TypeRegister
    {
        //#TODO@benp C++类型释放
        private static IntPtr ObjGetCallBack = IntPtr.Zero;
        private static IntPtr ObjSetCallBack = IntPtr.Zero;
        private static IntPtr ClsConstructorCallBack = IntPtr.Zero;
        private static IntPtr ClsSetCallBack = IntPtr.Zero;
        private static IntPtr ClsGetCallBack = IntPtr.Zero;

        private static IntPtr GetFieldWrapper(string name, bool isStatic, string signature)
        {
            //todo@benp reflectionWrap
            return NativeAPI.FindFieldWrap(signature);
        }

        private static IntPtr GetWrapperFunc(MemberInfo member, string signature)
        {
            Debug.Log("GetWrapperFunc "+signature);
            //todo@benp reflectionWrap
            return NativeAPI.FindWrapFunc(signature);
        }

        public static void SetTypeMetaId(Type type, int metaId){
            var typeId = NativeAPI.GetTypeId(type);
            NativeAPI.SetClassMetaId(typeId, metaId);
        }

        public static void SetLuaCacheRef(int cacheRef){
            NativeAPI.SetLuaCacheRef(cacheRef);
        }


        public static void Register(RealStatePtr L, Type type, bool includeNonPublic, bool throwIfMemberFail = false)
        {
            UnityEngine.Debug.Log($"Register {L} {type} {includeNonPublic} {throwIfMemberFail}");
            var translator = ObjectTranslatorPool.Instance.Find(L);
            if(ClsConstructorCallBack == IntPtr.Zero){
                ClsConstructorCallBack = NativeAPI.GetClsConstructorCallBackPtr();
                ClsGetCallBack = NativeAPI.GetClsGetCallBackPtr();
                ObjGetCallBack = NativeAPI.GetObjGetCallBackPtr();
                NativeAPI.SetLuaCacheRef(translator.cacheRef);
            }
            BindingFlags flag = BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public;
            if (includeNonPublic)
            {
                flag = flag | BindingFlags.NonPublic;
            }
            
            var cppClsDef = Register2Cpp(type, type.GetConstructors(flag), type.GetMethods(flag), type.GetProperties(flag), type.GetFields(flag), throwIfMemberFail);
            UnityEngine.Debug.Log("cppClsDef"+cppClsDef);
            
            if(cppClsDef != IntPtr.Zero){
                //push to lua
                Register2Lua(type, 0, cppClsDef, translator, L);
            }
        }

        private static void Push_LuaCSfunctionWithCppDef(RealStatePtr L, string key, int tableIdx, IntPtr cppClsDef, lua_CSFunction function){
            LuaAPI.xlua_pushasciistring(L, key);
            LuaAPI.lua_pushlightuserdata(L, cppClsDef);
#if XLUA_GENERAL || (UNITY_WSA && !UNITY_EDITOR)
            GCHandle.Alloc(function);
#endif
            IntPtr fn = Marshal.GetFunctionPointerForDelegate(function);
            LuaAPI.lua_pushcclosure(L, fn, 1);
            LuaAPI.lua_rawset(L, tableIdx);
        }

        private static void Push_stdcallcfunctionWithCppDef(RealStatePtr L, string key, int tableIdx, IntPtr cppClsDef, IntPtr stdCall){
            LuaAPI.xlua_pushasciistring(L, key);
            LuaAPI.lua_pushlightuserdata(L, cppClsDef);
            LuaAPI.lua_pushcclosure(L, stdCall, 1);
            LuaAPI.lua_rawset(L, tableIdx);
        }

        private static void Register2Lua(Type type, int type_id, IntPtr cppClsDef, ObjectTranslator translator, RealStatePtr L){
            //todo hotfix 处理
            //obj meta register begin
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
            
            if ((type == null || !translator.HasCustomOp(type)) && type != typeof(decimal))
			{
                //#TODO@benp GC处理
				LuaAPI.xlua_pushasciistring(L, "__gc");
				LuaAPI.lua_pushstdcallcfunction(L, translator.metaFunctions.GcMeta);
				LuaAPI.lua_rawset(L, -3);
			}

            
            LuaAPI.xlua_pushasciistring(L, "__tostring");
			LuaAPI.lua_pushstdcallcfunction(L, translator.metaFunctions.ToStringMeta);
			LuaAPI.lua_rawset(L, -3);

            // get
            // LuaAPI.xlua_pushasciistring(L, "__index");
            // LuaAPI.lua_pushlightuserdata(L, cppClsDef);
            // LuaAPI.xlua_push_csharp_function(L, ObjGetCallBack, 1);
            // LuaAPI.lua_rawset(L, -3);
            Push_stdcallcfunctionWithCppDef(L, "__index", -3, cppClsDef, ObjGetCallBack);
            //set
            // LuaAPI.xlua_pushasciistring(L, "__newindex");
            // LuaAPI.lua_pushlightuserdata(L, cppClsDef);
            // LuaAPI.xlua_push_csharp_function(L, ObjSetCallBack, 0);
            // LuaAPI.lua_rawset(L, -3);
            Push_stdcallcfunctionWithCppDef(L, "__newindex", -3, cppClsDef, ObjSetCallBack);
            //obj meta register end

            //cls meta register begin
            //cls table
            LuaAPI.lua_createtable(L, 0, 0); // 1 objMeta 2 clsTable
            
            LuaAPI.xlua_pushasciistring(L, "UnderlyingSystemType");
            translator.PushAny(L, type);
            LuaAPI.lua_rawset(L, -3);

            int cls_table = LuaAPI.lua_gettop(L);

			Utils.SetCSTable(L, type, cls_table);
            LuaAPI.lua_createtable(L, 0, 3); // // 1 objMeta 2 clsTable 3 clsMeta
			int meta_table = LuaAPI.lua_gettop(L);
            
            Push_stdcallcfunctionWithCppDef(L, "__call", meta_table, cppClsDef, ClsConstructorCallBack);
            
            Push_stdcallcfunctionWithCppDef(L, "__index", -3, cppClsDef, ClsGetCallBack);

            Push_stdcallcfunctionWithCppDef(L, "__newindex", -3, cppClsDef, ClsSetCallBack);

            LuaAPI.lua_pushvalue(L, meta_table); // 1 objMeta 2 clsTable 3 clsMeta 4 clsMeta
			LuaAPI.lua_setmetatable(L, cls_table); //  setmetatable(clsTable, clsMeta[4])  1 objMeta 2 clsTable 3 clsMeta
            LuaAPI.lua_pop(L,3);
        }

        [MonoPInvokeCallback(typeof(LuaCSFunction))]
        static int ConstructorCallBack(RealStatePtr L)
        {
            try
            {
                return NativeAPI.ClsConstructorCallBack(L);
                // ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
                // LuaCSFunction func = (LuaCSFunction)translator.FastGetCSObj(L, LuaAPI.xlua_upvalueindex(1));
                // return func(L);
            }
            catch (Exception e)
            {
                return LuaAPI.luaL_error(L, "c# exception in StaticCSFunction:" + e);
            }
        }

        /// <summary>
        /// 注册类型信息到IL2CPP,
        /// </summary>
        /// <param name="type"></param>
        /// <param name="ctors"></param>
        /// <param name="methods"></param>
        /// <param name="properties"></param>
        /// <param name="fields"></param>
        /// <param name="throwIfMemberFail"></param>
        private static IntPtr Register2Cpp(Type type, MethodBase[] ctors = null, MethodBase[] methods = null, PropertyInfo[] properties = null, FieldInfo[] fields = null, bool throwIfMemberFail = false)
        {
            IntPtr typeInfo = IntPtr.Zero;
            try
            {
                bool isDelegate = typeof(MulticastDelegate).IsAssignableFrom(type) && type != typeof(MulticastDelegate);
                var superTypeId = (isDelegate || type == typeof(object) || type.BaseType == null) ? IntPtr.Zero : NativeAPI.GetTypeId(type.BaseType);
                var typeId = NativeAPI.GetTypeId(type);
                UnityEngine.Debug.Log("typeId"+typeId);
                //create C++ struct 
                typeInfo = NativeAPI.CreateCSharpTypeInfo(type.ToString(), typeId, superTypeId, typeId, type.IsValueType, isDelegate, isDelegate ? TypeUtils.GetMethodSignature(type.GetMethod("Invoke"), true) : "");
                if (typeInfo == IntPtr.Zero)
                {
                    if (isDelegate) throw new Exception(string.Format("create TypeInfo for {0} fail. maybe the BridgeInfo is not found, try to regenerate the FunctionBridge.Gen.h", type));
                    throw new Exception(string.Format("create TypeInfo for {0} fail", type));
                }

                if(!isDelegate){

                    //constructor
                    if (ctors != null && ctors.Length > 0 && (!type.IsArray || type == typeof(System.Array)))
                    {
                        foreach (var ctor in ctors)
                        {
                            if (ctor.IsGenericMethodDefinition) continue;
                            List<Type> usedTypes = TypeUtils.GetUsedTypes(ctor);
                            var signature = TypeUtils.GetMethodSignature(ctor);

                            var wrapper = GetWrapperFunc(ctor,signature);
                            if (wrapper == IntPtr.Zero)
                            {
                                UnityEngine.Debug.LogWarning(string.Format("wrapper{1} is null for {0}", type, signature));
                                continue;
                            }
                            UnityEngine.Debug.Log(string.Format("add ctor {0}, usedTypes count: {1}", ctor, usedTypes.Count));

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
                                UnityEngine.Debug.Log(string.Format("set used type for ctor {0}: {1}={2}, typeId:{3}", ctor, i, usedTypes[i], typeInfoPointer));
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
                        UnityEngine.Debug.Log(string.Format("add method {0}, usedTypes count: {1}", method, usedTypes.Count));

                        var wrapper = GetWrapperFunc( method, signature);
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
                            UnityEngine.Debug.Log(string.Format("set used type for method {0}: {1}={2}, typeId:{3}", method, i, usedTypes[i], usedTypeId));
                            NativeAPI.SetTypeInfo(wrapData, i, usedTypeId);
                        }
                    };

                    if (methods != null && (!type.IsArray || type == typeof(System.Array)))
                    {
                        foreach (var method in methods)
                        {
                            if (method.IsAbstract) continue;
                            AddMethodToType(method.Name, method as MethodInfo, false, false, false);
                        }
                    }
					
                    //extension method
					var extensionMethods = Utils.GetExtensionMethodsOf(type);
					if (extensionMethods != null)
                    {
                        foreach (var method in extensionMethods)
                        {
                            AddMethodToType(method.Name, method as MethodInfo, false, false, true);
                        }
                    }

                    if (properties != null)
                    {
                        foreach (var prop in properties)
                        {
                            var getter = prop.GetGetMethod();
                            if (getter != null && !getter.IsGenericMethodDefinition && !getter.IsAbstract)
                            {
                                AddMethodToType(prop.Name, getter, true, false, false);
                            }
                            var setter = prop.GetSetMethod();
                            if (setter != null && !setter.IsGenericMethodDefinition && !setter.IsAbstract)
                            {
                                AddMethodToType(prop.Name, setter, false, true, false);
                            }
                        }
                    }

                    if (fields != null)
                    {
                        foreach (var field in fields)
                        {
                            string signature = (field.IsStatic ? "" : "t") + TypeUtils.GetTypeSignature(field.FieldType);
                            var name = field.Name;
                            
                            var wrapper = GetFieldWrapper( name, field.IsStatic, signature);
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
                            UnityEngine.Debug.Log(string.Format("AddField {0} of {1} ok offset={2}", field, type, NativeAPI.GetFieldOffset(field, type.IsValueType)));
                        }
                    }
                }
                var index =  NativeAPI.RegisterLuaClass(typeInfo);
                if(index == -1){
                    throw new Exception(string.Format("Register for {0} fail", type));
                }
                return typeId;

            }
            catch (Exception e){
                UnityEngine.Debug.Log("Register 2 CPP Excepitoni " + e.ToString());
                NativeAPI.ReleaseCSharpTypeInfo(typeInfo);
                throw e;
            }
        }
    }
}
#endif
