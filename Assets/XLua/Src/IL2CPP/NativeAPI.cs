#if IL2CPP_ENHANCED_LUA_DEVELOP
#define ENABLE_IL2CPP
#endif
#if IL2CPP_ENHANCED_LUA && ENABLE_IL2CPP

using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Reflection;
using System.Collections.Generic;

namespace XLua.IL2CPP
{
#pragma warning disable 414
    public class MonoPInvokeCallbackAttribute : System.Attribute
    {
        private Type type;
        public MonoPInvokeCallbackAttribute(Type t)
        {
            type = t;
        }
    }
#pragma warning restore 414

    public class NativeAPI
    {
#if (UNITY_ANDROID || UNITY_IPHONE || UNITY_TVOS || UNITY_WEBGL || UNITY_SWITCH) && !UNITY_EDITOR
        const string DLLNAME = "__Internal";
        const string LUADLL = "__Internal";
#else
        const string LUADLL = "xlua";
        const string DLLNAME = "__Internal";
#endif

        [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitialXLua_IL2CPP(IntPtr apiImpl, IntPtr rawL);


        // [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        // public static extern IntPtr GetClsConstructorCallBackPtr();

        // [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        // public static extern int ClsConstructorCallBack(IntPtr L);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static IntPtr CreateCSharpTypeInfo(string name, IntPtr type_id, IntPtr super_type_id, IntPtr klass, bool isValueType, bool isDelegate, string delegateSignature){
            throw new NotImplementedException ();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void ReleaseCSharpTypeInfo(IntPtr classInfo);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static IntPtr FindWrapFunc(string signature){
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr FindFieldWrap(string signature);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr AddConstructor(IntPtr classInfo, string signature, IntPtr WrapFunc, IntPtr method, IntPtr methodPointer, int typeInfoNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr AddMethod(IntPtr classInfo, string signature, IntPtr WrapFunc, string name, bool isStatic, bool isExtensionethod, bool isGetter, bool isSetter, IntPtr method, IntPtr methodPointer, int typeInfoNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr AddProperty(IntPtr classInfo, string signature, IntPtr WrapFunc, string name, bool isStatic, bool isGetter, IntPtr method, IntPtr methodPointer, int typeInfoNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool AddField(IntPtr classInfo, IntPtr FieldWrap, string name, bool isStatic, IntPtr fieldInfo, int offset, IntPtr fieldTypeInfo);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetTypeInfo(IntPtr wrapData, int index, IntPtr typeId);


        [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool RegisterLuaClass(IntPtr classInfo, string fullName);

        [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void UnRegisterLuaClass(string name);

        [DllImport(LUADLL, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr xlua_getImpl();

        [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
		public static extern void SetClassMetaId(IntPtr p, int metaId);

        [MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void SetXLuaRef(int[] refArr);
        
        [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
		public static extern void HandleDeleagateMetatable(IntPtr L, int tableIdx, IntPtr typeId);

        [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
		public static extern void HandleObjMetatable(IntPtr L, int tableIdx, IntPtr typeId);

        [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
		public static extern void HandleClsMetaTable(IntPtr L, int tabldIdx, IntPtr typeId);

        [MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void SetCSharpAPI(MethodBase[] methodInfo);


        
        //[DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern void SetObjectPool(IntPtr jsEnv, IntPtr objectPoolAddMethodInfo, IntPtr objectPoolAdd, IntPtr objectPoolRemoveMethodInfo, IntPtr objectPoolRemove, IntPtr objectPoolInstance);

        // [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        // public static extern void SetTryLoadCallback(IntPtr tryLoadMethodInfo, IntPtr tryLoad);

        //[DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern void SetObjectToGlobal(IntPtr jsEnv, string key, IntPtr objPtr);

        //[DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern void ReleasePendingJsObjects(IntPtr jsEnv);

        //[DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern void CreateInspector(IntPtr jsEnv, int port);

        //[DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern void DestroyInspector(IntPtr jsEnv);

        //[DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern bool InspectorTick(IntPtr jsEnv);
        
        //[DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern bool LogicTick(IntPtr jsEnv);

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public static Func<string, Puerts.JSObject> GetModuleExecutor(IntPtr NativeJsEnvPtr, Type type)
        //{
        //    throw new NotImplementedException();
        //}

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static IntPtr GetMethodPointer(MethodBase methodInfo)
        {
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static IntPtr GetMethodInfoPointer(MethodBase methodInfo)
        {
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static int GetFieldOffset(FieldInfo fieldInfo, bool isInValueType)
        {
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static IntPtr GetFieldInfoPointer(FieldInfo fieldInfo)
        {
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static IntPtr GetObjectPointer(Object obj)
        {
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static IntPtr GetTypeId(Type type)
        {
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static Type TypeIdToType(IntPtr typeId)
        {
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static IntPtr GetUnityExports()
        {
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static void SetGlobalType_LuaObject(Type type)
        {
           throw new NotImplementedException();
        }

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public static void SetGlobalType_ArrayBuffer(Type type)
        //{
        //    throw new NotImplementedException();
        //}
        
        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public static void SetGlobalType_TypedValue(Type type)
        //{
        //    throw new NotImplementedException();
        //}

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public static void PesapiCallTest(Type type)
        //{
        //    throw new NotImplementedException();
        //}

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public static object EvalInternal(IntPtr envHolder, byte[] code, string path, Type type)
        //{
        //    throw new NotImplementedException();
        //}

#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN || XLua_GENERAL || (UNITY_WSA && !UNITY_EDITOR)
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
#endif
        public delegate void LogCallback(string content);

        [MonoPInvokeCallback(typeof(LogCallback))]
        public static void LogImpl(string msg)
        {
            UnityEngine.Debug.Log("debug msg: " + msg);
        }

        public static LogCallback Log = LogImpl;

        [DllImport(DLLNAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetLogCallback(IntPtr log);

        //[UnityEngine.Scripting.RequiredByNativeCodeAttribute()]
        public static void SetLogCallback(LogCallback log)
        {
#if XLUA_GENERAL || (UNITY_WSA && !UNITY_EDITOR) || UNITY_STANDALONE_WIN
            GCHandle.Alloc(log);
#endif
            IntPtr fn1 = log == null ? IntPtr.Zero : Marshal.GetFunctionPointerForDelegate(log);

            try 
            {
                SetLogCallback(fn1);                
            }
            catch(DllNotFoundException)
            {
                UnityEngine.Debug.LogError("[Puer001] PuerTS's Native Plugin(s) is missing. You can solve this problem following the FAQ.");
                throw;
            }
        }
    }
}

#endif
