#include "il2cpp-config.h"

#include "codegen/il2cpp-codegen.h"
#include "il2cpp-api.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
#include "vm/InternalCalls.h"
#include "vm/Object.h"
#include "vm/Array.h"
#include "vm/Runtime.h"
#include "vm/Reflection.h"
#include "vm/MetadataCache.h"
#include "vm/Field.h"
#include "vm/GenericClass.h"
#include "vm/Thread.h"
#include "vm/Method.h"
#include "vm/Parameter.h"
#include "vm/Image.h"
#include "utils/StringUtils.h"
#include "gc/WriteBarrier.h"
#include "lua_api_adapt.h"
#include "UnityExports4Lua.h"
#include "xlua_il2cpp_def.h"
#include "LuaClassRegister.h"
#include "CppObjMapper.h"
static_assert(IL2CPP_GC_BOEHM, "Only BOEHM GC supported!");
//#TODO@benp 资源释放 malloc new
//#TODO@benp gc delegate tostring 函数参数默认值
//#TODO@benp 继承实现
//#TODO@benp 静态变量初始化 IL2CPP_RUNTIME_CLASS_INIT
//#TODO@benp klass 2 runtimeType
using namespace il2cpp::vm;

namespace xlua
{

    
    static xlua::UnityExports g_unityExports;
    intptr_t GetMethodPointer(Il2CppReflectionMethod* method)
    {
        auto methodInfo = method->method;
        auto ret = MetadataCache::GetMethodPointer(methodInfo->klass->image, methodInfo->token);
        if (!ret)
        {
            ret = methodInfo->methodPointer;
        }
        return (intptr_t)ret;
    }

    std::string ilstring2stdstring(Il2CppString * result){
        const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(result);
        return il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
    }

    intptr_t GetMethodInfoPointer(Il2CppReflectionMethod* method)
    {
        return (intptr_t)method->method;
    }

    int32_t GetFieldOffset(Il2CppReflectionField* field, bool isInValueType)
    {
        return (int32_t)Field::GetOffset(field->field) - (Class::IsValuetype(Field::GetParent(field->field)) ? sizeof(RuntimeObject) : 0);
    }

    intptr_t GetFieldInfoPointer(Il2CppReflectionField* field)
    {
        return (intptr_t)field->field;
    }

    intptr_t GetObjectPointer(RuntimeObject *obj)
    {
        return (intptr_t)obj;
    }

    intptr_t GetTypeId(Il2CppReflectionType *type)
    {
        return (intptr_t)il2cpp_codegen_class_from_type(type->type);
    }

    const void* CSharpTypeToTypeId(Il2CppObject *type)
    {
        return (type && Class::IsAssignableFrom(il2cpp_defaults.systemtype_class, type->klass)) ? il2cpp_codegen_class_from_type(((Il2CppReflectionType *)type)->type) : nullptr;
    }

    const Il2CppReflectionType* TypeIdToType(Il2CppClass *klass)
    {
        if (!klass) return nullptr;
        return Reflection::GetTypeObject(Class::GetType(klass));
    }

    static void* ObjectAllocate(Il2CppClass *klass)
    {
        if (Class::IsValuetype(klass))
        {
            auto size = klass->native_size > 0 ? klass->native_size : (klass->instance_size - sizeof(Il2CppObject));
            auto buff = (void*)(new uint8_t[size]);
            memset(buff, 0, size);
            return buff;
        } else {
            auto obj = il2cpp::vm::Object::New(klass);
            return obj;
        }
    }



    static int throw_exception2lua(lua_State * L, const char* msg){
        return lapi_luaL_error(L, msg);
    }

    static void ValueTypeFree(void* ptr)
    {
        delete [] (uint8_t*)ptr;
    }

    static Il2CppClass *g_typeofPersistentObjectInfo;
    static Il2CppClass *g_typeofArrayBuffer;
    static Il2CppClass *g_typeofTypedValue;

    const Il2CppClass* GetReturnType(const MethodInfo* method) {
        if (kInvalidIl2CppMethodSlot != method->slot) {
            Class::Init(method->klass);
        }
        return Class::FromIl2CppType(Method::GetReturnType(method), false);
    }

    const Il2CppClass* GetParameterType(const MethodInfo* method, int index) {
        if (kInvalidIl2CppMethodSlot != method->slot) {
            Class::Init(method->klass);
        }
        const Il2CppType* type = Method::GetParam(method, index);
        if (type) {
            return Class::FromIl2CppType(type, false);
        } else {
            return nullptr;
        }
    }

    static std::map<const MethodInfo*, const MethodInfo*> WrapFuncPtrToMethodInfo;
    // static std::recursive_mutex WrapFuncPtrToMethodInfoMutex;

    Il2CppDelegate* FunctionPointerToDelegate(Il2CppMethodPointer functionPtr, Il2CppClass* delegateType, Il2CppObject* target)
    {
        Il2CppObject* delegate = il2cpp::vm::Object::New(delegateType);
        const MethodInfo* invoke = il2cpp::vm::Runtime::GetDelegateInvoke(delegateType);

        const MethodInfo* method = NULL;
        {
            // std::lock_guard<std::recursive_mutex> lock(WrapFuncPtrToMethodInfoMutex);
            //il2cpp::utils::NativeDelegateMethodCache::GetNativeDelegate((Il2CppMethodPointer)invoke);
            auto iter = WrapFuncPtrToMethodInfo.find(invoke);
            if (iter == WrapFuncPtrToMethodInfo.end())
            {
                MethodInfo* newMethod = (MethodInfo*)IL2CPP_CALLOC(1, sizeof(MethodInfo));
                newMethod->name = invoke->name;
                newMethod->klass = invoke->klass;
                newMethod->methodPointer = functionPtr;
                newMethod->invoker_method = invoke->invoker_method;
                newMethod->return_type = invoke->return_type;
                newMethod->parameters_count = invoke->parameters_count;
                newMethod->parameters = invoke->parameters;
                newMethod->slot = kInvalidIl2CppMethodSlot;
                //newMethod->is_marshaled_from_native = true;
                //il2cpp::utils::NativeDelegateMethodCache::AddNativeDelegate((Il2CppMethodPointer)invoke, newMethod);
                WrapFuncPtrToMethodInfo.insert(std::make_pair(invoke, newMethod));
                method = newMethod;
            }
            else
            {
                method = iter->second;
            }
        }

    #ifdef UNITY_2021_1_OR_NEWER
        Type::ConstructClosedDelegate((Il2CppDelegate*)delegate, target, functionPtr, method);
    #else
        Type::ConstructDelegate((Il2CppDelegate*)delegate, target, functionPtr, method);
    #endif

        return (Il2CppDelegate*)delegate;
    }

    static void* DelegateAllocate(Il2CppClass *klass, Il2CppMethodPointer functionPtr, void** outTargetData)
    {
        Il2CppClass *delegateInfoClass = g_typeofPersistentObjectInfo;
        if (!delegateInfoClass) return nullptr;
        
        auto target = il2cpp::vm::Object::New(delegateInfoClass);

        Il2CppDelegate* delegate = FunctionPointerToDelegate(functionPtr, klass, target);

        if (MethodIsStatic(delegate->method)) return nullptr;

    #ifndef UNITY_2021_1_OR_NEWER
        const MethodInfo* ctor = il2cpp_class_get_method_from_name(delegateInfoClass, ".ctor", 0);
        typedef void (*NativeCtorPtr)(Il2CppObject* ___this, const MethodInfo* method);
        ((NativeCtorPtr)ctor->methodPointer)(target, ctor);

        IL2CPP_OBJECT_SETREF(delegate, target, target);
    #endif

        *outTargetData = target + 1;

        delegate->method_ptr = functionPtr;

        return delegate;
    }

    void SetGlobalType_ArrayBuffer(Il2CppReflectionType *type)
    {
        if (!type)
        {
            Exception::Raise(Exception::GetInvalidOperationException("type of ArrayBuffer is null"));
        }
        g_typeofArrayBuffer =  il2cpp_codegen_class_from_type(type->type);
    }

    // void SetGlobalType_JSObject(Il2CppReflectionType *type)
    // {
    //     if (!type)
    //     {
    //         Exception::Raise(Exception::GetInvalidOperationException("type of JSObject is null"));
    //     }
    //     g_typeofPersistentObjectInfo = il2cpp_codegen_class_from_type(type->type);
    // }

    void SetGlobalType_TypedValue(Il2CppReflectionType *type)
    {
        if (!type)
        {
            Exception::Raise(Exception::GetInvalidOperationException("type of TypedValue is null"));
        }
        g_typeofTypedValue = il2cpp_codegen_class_from_type(type->type);
    }


    static int MethodCallback(lua_State *L, WrapData** wrapDatas, int index = 1) {
        try 
        {
            //#TODO@benp 

             bool checkArgument = *wrapDatas && *(wrapDatas + 1);
             while(*wrapDatas)
             {
                 if ((*wrapDatas)->Wrap((*wrapDatas)->Method, (*wrapDatas)->MethodPointer, L, checkArgument, *wrapDatas, index))
                 {
                     return 1;
                 }
                 ++wrapDatas;
             }
            return throw_exception2lua(L, "invalid arguments"); 
        } 
        catch (Il2CppExceptionWrapper& exception)
        {
            Il2CppClass* klass = il2cpp::vm::Object::GetClass(exception.ex);
            const MethodInfo* toStringMethod = il2cpp::vm::Class::GetMethodFromName(klass, "ToString", 0);

            Il2CppException* outException = NULL;
            Il2CppString* result = (Il2CppString*)il2cpp::vm::Runtime::Invoke(toStringMethod, exception.ex, NULL, &outException);
            if (outException != NULL)
            {
                throw_exception2lua(L, "unknow c# execption!");
            }
            else
            {
                const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(result);
                std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
                // throw_exception2lua(info, str.c_str());
                throw_exception2lua(L, str.c_str());
            }
        }
        return 0;
    }

    void GetFieldValue(void *ptr, FieldInfo *field, size_t offset, void *value)
    {
        void *src;

        if (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
        {
            IL2CPP_ASSERT(ptr);
            src = (char*)ptr + offset;
            Field::SetValueRaw(field->type, value, src, true);
        }
        else
        {
            Field::StaticGetValue(field, value);
        }
    }

    void* GetValueTypeFieldPtr(void *obj, FieldInfo *field, size_t offset)
    {
        if (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
        {
            IL2CPP_ASSERT(obj);
            return (char*)obj + offset;
        }
        else
        {
            Class::SetupFields(field->parent);

            void* threadStaticData = NULL;
            if (field->offset == THREAD_STATIC_FIELD_OFFSET)
    #ifdef UNITY_2021_1_OR_NEWER
                threadStaticData = Thread::GetThreadStaticDataForThread(field->parent->thread_static_fields_offset, il2cpp::vm::Thread::CurrentInternal());
    #else
                threadStaticData = Thread::GetThreadStaticDataForThread(field->parent->thread_static_fields_offset, il2cpp::vm::Thread::Current());
    #endif
            
            if (field->offset == THREAD_STATIC_FIELD_OFFSET)
            {
                IL2CPP_ASSERT(NULL != threadStaticData);
                int threadStaticFieldOffset = MetadataCache::GetThreadLocalStaticOffsetForField(field);
                return ((char*)threadStaticData) + threadStaticFieldOffset;
            }
            else
            {
                return ((char*)field->parent->static_fields) + field->offset;
            }
        }
    }

    void SetFieldValue(void *ptr, FieldInfo *field, size_t offset, void *value)
    {
        void *dest;

        if(!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
        {
            IL2CPP_ASSERT(ptr);
            dest = (char*)ptr + offset;
            Field::SetValueRaw(field->type, dest, value, true);
        }
        else
        {
            Field::StaticSetValue(field, value);
        }
    }

    void* GetDefaultValuePtr(const MethodInfo* method, uint32_t index)
    {
        bool isExplicitySetNullDefaultValue = false;
    #ifdef UNITY_2021_1_OR_NEWER
        Il2CppObject* defaultValue = Parameter::GetDefaultParameterValueObject(method, index, &isExplicitySetNullDefaultValue);
    #else
        Il2CppObject* defaultValue = Parameter::GetDefaultParameterValueObject(method, &method->parameters[index], &isExplicitySetNullDefaultValue);
    #endif
        return (defaultValue && Class::IsValuetype(Class::FromIl2CppType(Method::GetParam(method, index), false))) ? Object::Unbox(defaultValue) : defaultValue;
    }


    bool IsValueType(Il2CppClass *klass)
    {
        return Class::IsValuetype(klass);
    }

    bool IsDelegate(Il2CppClass *klass)
    {
        return Class::IsAssignableFrom(il2cpp_defaults.delegate_class, klass) && klass != il2cpp_defaults.delegate_class && klass != il2cpp_defaults.multicastdelegate_class;
    }

    int GetTID(Il2CppObject* obj)
    {
        if (obj)
        {
            const Il2CppType *type = Class::GetType(Object::GetClass(obj));
            return type->type;
        }
        return -1;
    }

    static FieldInfo* ArrayBufferCountField = nullptr;
    static FieldInfo* ArrayBufferBytesField = nullptr;
    // pesapi_value TryTranslateBuiltin(pesapi_env env, Il2CppObject* obj)
    // {
    //     if (obj)
    //     {
    //         if (obj->klass == g_typeofPersistentObjectInfo)
    //         {
    //             PersistentObjectInfo* objectInfo = reinterpret_cast<PersistentObjectInfo*>(obj + 1);
    //             return g_unityExports.GetPersistentObject(env, objectInfo);
    //         }
    //         if (obj->klass == g_typeofArrayBuffer)
    //         {
    //             if (ArrayBufferBytesField == nullptr || ArrayBufferCountField == nullptr) {
    //                 ArrayBufferCountField = il2cpp_class_get_field_from_name(g_typeofArrayBuffer, "Count");
    //                 ArrayBufferBytesField = il2cpp_class_get_field_from_name(g_typeofArrayBuffer, "Bytes");
    //             }

    //             int32_t length = 0;
    //             il2cpp_field_get_value(obj, ArrayBufferCountField, &length);

    //             Il2CppArray* buffer;
    //             il2cpp_field_get_value(obj, ArrayBufferBytesField, &buffer);

    //             return g_unityExports.CreateJSArrayBuffer(env, Array::GetFirstElementAddress(buffer), (size_t) length);
    //         }
    //     }
    //     return nullptr;
    // }

    static void TryTranslatePrimitiveWithClass(lua_State* L, Il2CppObject* obj, Il2CppClass *klass = nullptr)
    {
        if (obj)
        {
            const Il2CppType *type = Class::GetType(klass ? klass : obj->klass);
            int t = type->type;
            if (t == IL2CPP_TYPE_STRING)
            {
                const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars((Il2CppString*)obj);
                std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
                lapi_lua_pushstring(L, str.c_str());
            }
            void* ptr = Object::Unbox(obj);
            switch (t)
            {
                case IL2CPP_TYPE_I1:
                {
                    return lapi_lua_pushinteger(L, (int32_t)(*((int8_t*)ptr)));
                }
                case IL2CPP_TYPE_BOOLEAN:
                {
                    return lapi_lua_pushboolean(L, (int)(*((uint8_t*)ptr)));
                }
                case IL2CPP_TYPE_U1:
                {
                    return lapi_lua_pushinteger(L, (uint32_t)(*((uint8_t*)ptr)));
                }
                case IL2CPP_TYPE_I2:
                {
                    return lapi_lua_pushinteger(L, (int32_t)(*((int16_t*)ptr)));
                }
                case IL2CPP_TYPE_U2:
                {
                    return lapi_lua_pushinteger(L, (uint32_t)(*((uint16_t*)ptr)));
                }
                case IL2CPP_TYPE_CHAR:
                {
                    return lapi_lua_pushinteger(L, (int32_t)(*((Il2CppChar*)ptr)));
                }
        #if IL2CPP_SIZEOF_VOID_P == 4
                case IL2CPP_TYPE_I:
        #endif
                case IL2CPP_TYPE_I4:
                {
                    return lapi_lua_pushinteger(L, (int32_t)(*((int32_t*)ptr)));
                }
        #if IL2CPP_SIZEOF_VOID_P == 4
                case IL2CPP_TYPE_U:
        #endif
                case IL2CPP_TYPE_U4:
                {
                    return lapi_lua_pushinteger(L, (uint32_t)(*((uint32_t*)ptr)));
                }
        #if IL2CPP_SIZEOF_VOID_P == 8
                case IL2CPP_TYPE_I:
        #endif
                case IL2CPP_TYPE_I8:
                {
                    return lapi_lua_pushinteger(L, *((int64_t*)ptr));
                }
        #if IL2CPP_SIZEOF_VOID_P == 8
                case IL2CPP_TYPE_U:
        #endif
                case IL2CPP_TYPE_U8:
                {
                    return lapi_lua_pushinteger(L, *((uint64_t*)ptr));
                }
                case IL2CPP_TYPE_R4:
                {
                    return lapi_lua_pushnumber(L, (double)(*((float*)ptr)));
                }
                case IL2CPP_TYPE_R8:
                {
                    return lapi_lua_pushnumber(L, *((double*)ptr));
                }
                
                default:
                    return ;
            }
        }
    }

    void TryTranslatePrimitive(lua_State* L, Il2CppObject* obj)
    {
        return TryTranslatePrimitiveWithClass(L, obj);
    }

    // pesapi_value TranslateValueType(pesapi_env env, Il2CppClass* targetClass, Il2CppObject* obj)
    // {
    //     auto len = targetClass->native_size;
    //     if (len < 0)
    //     {
    //         len = targetClass->instance_size - sizeof(Il2CppObject);
    //     }

    //     auto buff = new uint8_t[len];
    //     memcpy(buff, Object::Unbox(obj), len);
    //     return pesapi_create_native_object(env, targetClass, buff, true);
    // }

    // pesapi_value TryTranslateValueType(pesapi_env env, Il2CppObject* obj)
    // {
    //     if (obj && obj->klass)
    //     {
    //         auto objClass = obj->klass;
    //         if (Class::IsValuetype(objClass))
    //         {
    //             return TranslateValueType(env, objClass, obj);
    //         }
    //     }
    //     return nullptr;
    // }


    union PrimitiveValueType
    {
        int8_t i1;
        uint8_t u1;
        int16_t i2;
        uint16_t u2;
        int32_t i4;
        uint32_t u4;
        int64_t i8;
        uint64_t u8;
        Il2CppChar c;
        float r4;
        double r8;
    };

    // Il2CppObject* JsValueToCSRef(Il2CppClass *klass, pesapi_env env, pesapi_value jsval)
    // {
    //     if (klass == il2cpp_defaults.void_class) return nullptr;
        
    //     if (!klass)
    //     {
    //         klass = il2cpp_defaults.object_class;
    //     }        
        
    //     const Il2CppType *type = Class::GetType(klass);
    //     int t = type->type;
        
    //     PrimitiveValueType data;
        
    //     void* toBox = &data;
        
    //     Il2CppObject* ret = nullptr;
        
    // handle_underlying:
    //     switch (t)
    //     {
    //         case IL2CPP_TYPE_I1:
    //         {
    //             data.i1 = (int8_t)pesapi_get_value_int32(env, jsval);
    //             break;
    //         }
    //         case IL2CPP_TYPE_BOOLEAN:
    //         {
    //             data.u1 = (uint8_t)pesapi_get_value_bool(env, jsval);
    //         }
    //         case IL2CPP_TYPE_U1:
    //         {
    //             data.u1 = (uint8_t)pesapi_get_value_uint32(env, jsval);
    //             break;
    //         }
    //         case IL2CPP_TYPE_I2:
    //         {
    //             data.i2 = (int16_t)pesapi_get_value_int32(env, jsval);
    //             break;
    //         }
    //         case IL2CPP_TYPE_U2:
    //         {
    //             data.u2 = (uint16_t)pesapi_get_value_uint32(env, jsval);
    //             break;
    //         }
    //         case IL2CPP_TYPE_CHAR:
    //         {
    //             data.c = (Il2CppChar)pesapi_get_value_uint32(env, jsval);
    //             break;
    //         }
    // #if IL2CPP_SIZEOF_VOID_P == 4
    //         case IL2CPP_TYPE_I:
    // #endif
    //         case IL2CPP_TYPE_I4:
    //         {
    //             data.i4 = (int32_t)pesapi_get_value_int32(env, jsval);
    //             break;
    //         }
    // #if IL2CPP_SIZEOF_VOID_P == 4
    //         case IL2CPP_TYPE_U:
    // #endif
    //         case IL2CPP_TYPE_U4:
    //         {
    //             data.u4 = (uint32_t)pesapi_get_value_uint32(env, jsval);
    //             break;
    //         }
    // #if IL2CPP_SIZEOF_VOID_P == 8
    //         case IL2CPP_TYPE_I:
    // #endif
    //         case IL2CPP_TYPE_I8:
    //         {
    //             data.i8 = pesapi_get_value_int64(env, jsval);
    //             break;
    //         }
    // #if IL2CPP_SIZEOF_VOID_P == 8
    //         case IL2CPP_TYPE_U:
    // #endif
    //         case IL2CPP_TYPE_U8:
    //         {
    //             data.u8 = pesapi_get_value_uint64(env, jsval);
    //             break;
    //         }
    //         case IL2CPP_TYPE_R4:
    //         {
    //             data.r4 = (float)pesapi_get_value_double(env, jsval);
    //             break;
    //         }
    //         case IL2CPP_TYPE_R8:
    //         {
    //             data.r8 = pesapi_get_value_double(env, jsval);
    //             break;
    //         }
    //         case IL2CPP_TYPE_STRING:
    //         {
    //             size_t bufsize = 0;
    //             auto str = pesapi_get_value_string_utf8(env, jsval, nullptr, &bufsize);
    //             if (str)
    //             {
    //                 return (Il2CppObject*)il2cpp::vm::String::NewWrapper(str);
    //             }
    //             std::vector<char> buff;
    //             buff.resize(bufsize + 1);
    //             str = pesapi_get_value_string_utf8(env, jsval, buff.data(), &bufsize);
    //             if (str)
    //             {
    //                 buff[bufsize] = '\0';
    //                 return (Il2CppObject*)il2cpp::vm::String::NewWrapper(str);
    //             }
    //             return nullptr;
    //         }
    //         case IL2CPP_TYPE_SZARRAY:
    //         case IL2CPP_TYPE_CLASS:
    //         case IL2CPP_TYPE_OBJECT:
    //         case IL2CPP_TYPE_ARRAY:
    //         case IL2CPP_TYPE_FNPTR:
    //         case IL2CPP_TYPE_PTR:
    //         {
    //             if (pesapi_is_function(env, jsval))
    //             {
    //                 if (IsDelegate(klass))
    //                 {
    //                     return (Il2CppObject*)g_unityExports.FunctionToDelegate(env, jsval, klass, true);
    //                 }
    //                 return nullptr;
    //             }
    //             auto ptr = pesapi_get_native_object_ptr(env, jsval);
    //             if (!ptr)
    //             {
    //                 if ((klass == g_typeofArrayBuffer || klass == il2cpp_defaults.object_class) && pesapi_is_binary(env, jsval)) 
    //                 {
    //                     RuntimeObject* ret = il2cpp::vm::Object::New(g_typeofArrayBuffer);

    //                     const MethodInfo* ctor = il2cpp_class_get_method_from_name(g_typeofArrayBuffer, ".ctor", 3);
    //                     typedef void (*NativeCtorPtr)(Il2CppObject* ___this, void*, int, int, const MethodInfo* method);
                        
    //                     void* data;
    //                     size_t length;
    //                     data = pesapi_get_value_binary(env, jsval, &length);
    //                     ((NativeCtorPtr)ctor->methodPointer)(ret, data, length, 0, ctor);   
    //                     return ret;
    //                 }
    //                 if ((klass == g_typeofPersistentObjectInfo || klass == il2cpp_defaults.object_class) && pesapi_is_object(env, jsval))
    //                 {
    //                     Il2CppClass* persistentObjectInfoClass = g_typeofPersistentObjectInfo;
                        
    //                     RuntimeObject* ret = (RuntimeObject*)g_unityExports.GetRuntimeObjectFromPersistentObject(env, jsval);
    //                     if (ret == nullptr) 
    //                     {
    //                         ret = il2cpp::vm::Object::New(persistentObjectInfoClass);

    //                         const MethodInfo* ctor = il2cpp_class_get_method_from_name(persistentObjectInfoClass, ".ctor", 0);
    //                         typedef void (*NativeCtorPtr)(Il2CppObject* ___this, const MethodInfo* method);
    //                         ((NativeCtorPtr)ctor->methodPointer)(ret, ctor);
                            
    //                         PersistentObjectInfo* objectInfo = reinterpret_cast<PersistentObjectInfo*>(ret + 1);
    //                         g_unityExports.SetPersistentObject(env, jsval, objectInfo);
    //                         g_unityExports.SetRuntimeObjectToPersistentObject(env, jsval, ret);
    //                     }
    //                     return ret;
    //                 }
    //                 if (klass == il2cpp_defaults.object_class)
    //                 {
    //                     if (pesapi_is_string(env, jsval))
    //                     {
    //                         t = IL2CPP_TYPE_STRING;
    //                         klass = il2cpp_defaults.string_class;
    //                     }
    //                     else if (pesapi_is_double(env, jsval))
    //                     {
    //                         t = IL2CPP_TYPE_R8;
    //                         klass = il2cpp_defaults.double_class;
    //                     }
    //                     else if (pesapi_is_int32(env, jsval))
    //                     {
    //                         t = IL2CPP_TYPE_I4;
    //                         klass = il2cpp_defaults.int32_class;
    //                     }
    //                     else if (pesapi_is_uint32(env, jsval))
    //                     {
    //                         t = IL2CPP_TYPE_U4;
    //                         klass = il2cpp_defaults.uint32_class;
    //                     }
    //                     else if (pesapi_is_int64(env, jsval))
    //                     {
    //                         t = IL2CPP_TYPE_I8;
    //                         klass = il2cpp_defaults.int64_class;
    //                     }
    //                     else if (pesapi_is_uint64(env, jsval))
    //                     {
    //                         t = IL2CPP_TYPE_U8;
    //                         klass = il2cpp_defaults.uint64_class;
    //                     }
    //                     else if (pesapi_is_boolean(env, jsval))
    //                     {
    //                         t = IL2CPP_TYPE_BOOLEAN;
    //                         klass = il2cpp_defaults.boolean_class;
    //                     }
    //                     else
    //                     {
    //                         goto return_nothing;
    //                     }
    //                     goto handle_underlying;
    //                 }
    //             return_nothing:
    //                 return nullptr;
    //             }
    //             auto objClass = (Il2CppClass *)pesapi_get_native_object_typeid(env, jsval);
    //             if (klass == il2cpp_defaults.object_class && g_typeofTypedValue && Class::IsAssignableFrom(g_typeofTypedValue, objClass))
    //             {
    //                 const MethodInfo* get_Target = il2cpp_class_get_method_from_name(objClass, "get_Target", 0);
    //                 if (get_Target)
    //                 {
    //                     typedef Il2CppObject* (*NativeFuncPtr)(void* ___this, const MethodInfo* method);
    //                     return ((NativeFuncPtr)get_Target->methodPointer)(ptr, get_Target);
    //                 }
    //             }
    //             if (Class::IsAssignableFrom(klass, objClass))
    //             {
    //                 return Class::IsValuetype(objClass) ? Object::Box(objClass, ptr) : (Il2CppObject*)ptr;
    //             }
    //             return nullptr;
    //         }
    //         case IL2CPP_TYPE_VALUETYPE:
    //             /* note that 't' and 'type->type' can be different */
    //             if (type->type == IL2CPP_TYPE_VALUETYPE && Type::IsEnum(type))
    //             {
    //                 t = Class::GetEnumBaseType(Type::GetClass(type))->type;
    //                 goto handle_underlying;
    //             }
    //             else
    //             {
    //                 auto objClass = (Il2CppClass *)pesapi_get_native_object_typeid(env, jsval);
    //                 if (!Class::IsAssignableFrom(klass, objClass))
    //                 {
    //                     return nullptr;
    //                 }
    //                 toBox = pesapi_get_native_object_ptr(env, jsval);
    //                 if (!toBox)
    //                 {
    //                     std::string message = "expect ValueType: ";
    //                     message += klass->name;
    //                     message += ", by got null";
    //                     Exception::Raise(Exception::GetInvalidOperationException(message.c_str()));
    //                     return nullptr;
    //                 }
    //             }
    //             break;
    //         case IL2CPP_TYPE_GENERICINST:
    //             t = GenericClass::GetTypeDefinition(type->data.generic_class)->byval_arg.type;
    //             goto handle_underlying;
    //         default:
    //             IL2CPP_ASSERT(0);
    //     }
    //     return Object::Box(klass, toBox);
    // }

    // pesapi_value CSRefToJsValue(pesapi_env env, Il2CppClass *targetClass, Il2CppObject* obj)
    // {
    //     if (targetClass == il2cpp_defaults.void_class ) return pesapi_create_undefined(env);
    //     if (!obj) return pesapi_create_null(env);

    //     if (!targetClass)
    //     {
    //         targetClass = il2cpp_defaults.object_class;
    //     }
        
    //     if (Class::IsEnum(targetClass))
    //     {
    //         targetClass = Class::GetElementClass(targetClass);
    //     }
        
    //     pesapi_value jsVal = TryTranslatePrimitiveWithClass(env, obj, targetClass != il2cpp_defaults.object_class ? targetClass : nullptr);
        
    //     if (jsVal) 
    //     {
    //         return jsVal;
    //     }

    //     if (Class::IsValuetype(targetClass))
    //     {
    //         jsVal = TranslateValueType(env, targetClass, obj);
    //         if (jsVal)
    //         {
    //             return jsVal;
    //         }    
    //     }
        
    //     jsVal = TryTranslateValueType(env, obj);
    //     if (jsVal) 
    //     {
    //         return jsVal;
    //     }

    //     jsVal = TryTranslateBuiltin(env, obj);

    //     if (jsVal)
    //     {
    //         return jsVal;
    //     }

    //     auto objClass = obj && obj->klass ? obj->klass : targetClass;
    //     return pesapi_create_native_object(env, objClass, obj, false);
    // }

    // static bool GetValueTypeFromJs(pesapi_env env, pesapi_value jsValue, Il2CppClass* klass, void* storage)
    // {
    //     bool hasValue = false;
    //     uint32_t valueSize = klass->instance_size - sizeof(Il2CppObject);
    //     if (!jsValue) return false;
    //     void* ptr;
    //     if (pesapi_is_object(env, jsValue) && (ptr = pesapi_get_native_object_ptr(env, jsValue)))
    //     {
    //         auto objClass = (Il2CppClass*) pesapi_get_native_object_typeid(env, jsValue);
    //         if (Class::IsAssignableFrom(klass, objClass))
    //         {
    //             hasValue = true;
    //             memcpy(storage, ptr, valueSize);
    //         }

    //     } else {
    //         const Il2CppType *type = Class::GetType(klass);
    //         PrimitiveValueType data;
    //         data.i8 = 0;
    //         int t = type->type;
    // handle_underlying:
    //         switch (t)
    //         {
    //             case IL2CPP_TYPE_I1:
    //             {
    //                 if (pesapi_is_int32(env, jsValue))
    //                 {
    //                     data.i1 = (int8_t)pesapi_get_value_int32(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //             case IL2CPP_TYPE_BOOLEAN:
    //             {
    //                 if (pesapi_is_boolean(env, jsValue))
    //                 {
    //                     data.u1 = (uint8_t)pesapi_get_value_bool(env, jsValue);
    //                     hasValue = true;
    //                 }
    //             }
    //             case IL2CPP_TYPE_U1:
    //             {
    //                 if (pesapi_is_uint32(env, jsValue))
    //                 {
    //                     data.u1 = (uint8_t)pesapi_get_value_uint32(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //             case IL2CPP_TYPE_I2:
    //             {
    //                 if (pesapi_is_int32(env, jsValue))
    //                 {
    //                     data.i2 = (int16_t)pesapi_get_value_int32(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //             case IL2CPP_TYPE_U2:
    //             {
    //                 if (pesapi_is_uint32(env, jsValue))
    //                 {
    //                     data.u2 = (uint16_t)pesapi_get_value_uint32(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //             case IL2CPP_TYPE_CHAR:
    //             {
    //                 if (pesapi_is_uint32(env, jsValue))
    //                 {
    //                     data.c = (Il2CppChar)pesapi_get_value_uint32(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //     #if IL2CPP_SIZEOF_VOID_P == 4
    //             case IL2CPP_TYPE_I:
    //     #endif
    //             case IL2CPP_TYPE_I4:
    //             {
    //                 if (pesapi_is_int32(env, jsValue))
    //                 {
    //                     data.i4 = (int32_t)pesapi_get_value_int32(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //     #if IL2CPP_SIZEOF_VOID_P == 4
    //             case IL2CPP_TYPE_U:
    //     #endif
    //             case IL2CPP_TYPE_U4:
    //             {
    //                 if (pesapi_is_uint32(env, jsValue))
    //                 {
    //                     data.u4 = (uint32_t)pesapi_get_value_uint32(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //     #if IL2CPP_SIZEOF_VOID_P == 8
    //             case IL2CPP_TYPE_I:
    //     #endif
    //             case IL2CPP_TYPE_I8:
    //             {
    //                 if (pesapi_is_int64(env, jsValue))
    //                 {
    //                     data.i8 = pesapi_get_value_int64(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }   
    //     #if IL2CPP_SIZEOF_VOID_P == 8
    //             case IL2CPP_TYPE_U:
    //     #endif
    //             case IL2CPP_TYPE_U8:
    //             {
    //                 if (pesapi_is_uint64(env, jsValue))
    //                 {
    //                     data.u8 = pesapi_get_value_uint64(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //             case IL2CPP_TYPE_R4:
    //             {
    //                 if (pesapi_is_double(env, jsValue))
    //                 {
    //                     data.r4 = (float)pesapi_get_value_double(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //             case IL2CPP_TYPE_R8:
    //             {
    //                 if (pesapi_is_double(env, jsValue))
    //                 {
    //                     data.r8 = pesapi_get_value_double(env, jsValue);
    //                     hasValue = true;
    //                 }
    //                 break;
    //             }
    //             case IL2CPP_TYPE_VALUETYPE:
    //             /* note that 't' and 'type->type' can be different */
    //             if (type->type == IL2CPP_TYPE_VALUETYPE && Type::IsEnum(type))
    //             {
    //                 t = Class::GetEnumBaseType(Type::GetClass(type))->type;
    //                 goto handle_underlying;
    //             }
    //         }
        
    //         if(hasValue)
    //         {
    //             memcpy(storage, &data, valueSize);
    //         }
    //     }
    //     return hasValue;
    // }

    // static pesapi_value JsObjectUnRef(pesapi_env env, pesapi_value jsValue)
    // {
    //     return (pesapi_is_object(env, jsValue)) ?  pesapi_get_property_uint32(env, jsValue, 0) : nullptr;
    // }

    // static void JsObjectSetRef(pesapi_env env, pesapi_value outer, pesapi_value val)
    // {
    //     if (outer && val && pesapi_is_object(env, outer))
    //     {
    //         pesapi_set_property_uint32(env, outer, 0, val);
    //     }
    // }

    // static bool ReflectionWrapper(MethodInfo* method, Il2CppMethodPointer methodPointer, pesapi_callback_info info, bool checkJSArgument, WrapData* wrapData)
    // {
    //     pesapi_env env = pesapi_get_env(info);
    //     int js_args_len = pesapi_get_args_len(info);
    //     bool hasParamArray = wrapData->HasParamArray;
    //     bool isExtensionMethod = wrapData->IsExtensionMethod;
    //     auto csArgStart = isExtensionMethod ? 1 : 0;
        
    //     if (checkJSArgument || wrapData->OptionalNum > 0)
    //     {
    //         if (!hasParamArray && wrapData->OptionalNum == 0)
    //         {
    //             if (js_args_len != method->parameters_count - csArgStart)
    //             {
    //                 return false;
    //             }
    //         }
    //         else
    //         {
    //             auto requireNum = method->parameters_count - csArgStart - wrapData->OptionalNum - (hasParamArray ? 1 : 0);
    //             if (js_args_len < requireNum)
    //             {
    //                 return false;
    //             }
    //         }
    //         for (int i = csArgStart; i < method->parameters_count; ++i)
    //         {
    //             auto parameterType = Method::GetParam(method, i);
    //             bool passedByReference = parameterType->byref;
    //             bool hasDefault = parameterType->attrs & PARAM_ATTRIBUTE_HAS_DEFAULT;
    //             bool isLastArgument = i == (method->parameters_count - 1);
    //             Il2CppClass* parameterKlass = Class::FromIl2CppType(parameterType);
    //             Class::Init(parameterKlass);
    //             pesapi_value jsValue = pesapi_get_arg(info, i - csArgStart);
                
    //             if ((hasDefault || (isLastArgument && hasParamArray)) && pesapi_is_undefined(env, jsValue))
    //             {
    //                 continue;
    //             }
    //             if (passedByReference)
    //             {
    //                 if (pesapi_is_object(env, jsValue))
    //                 {
    //                     continue;
    //                 }
    //                 else
    //                 {
    //                     return false;
    //                 }
    //             }
    //             int t;
    //             if (isLastArgument && hasParamArray)
    //                 t = (int) parameterKlass->element_class->byval_arg.type;
    //             else
    //                 t = parameterType->type; 
    // handle_underlying:
    //             switch (t)
    //             {
    //                 case IL2CPP_TYPE_I1:
    //                 case IL2CPP_TYPE_I2:
    // #if IL2CPP_SIZEOF_VOID_P == 4
    //                 case IL2CPP_TYPE_I:
    // #endif
    //                 case IL2CPP_TYPE_I4:
    //                 {
    //                     if (!pesapi_is_int32(env, jsValue))
    //                     {
    //                         return false;
    //                     }
    //                     break;
    //                 }
    //                 case IL2CPP_TYPE_BOOLEAN:
    //                 {
    //                     if (!pesapi_is_boolean(env, jsValue))
    //                     {
    //                         return false;
    //                     }
    //                     break;
    //                 }
    //                 case IL2CPP_TYPE_U1:
    //                 case IL2CPP_TYPE_U2:
    //                 case IL2CPP_TYPE_CHAR:
    // #if IL2CPP_SIZEOF_VOID_P == 4
    //                 case IL2CPP_TYPE_U:
    // #endif
    //                 case IL2CPP_TYPE_U4:
    //                 {
    //                     if (!pesapi_is_uint32(env, jsValue))
    //                     {
    //                         return false;
    //                     }
    //                     break;
    //                 }
    //         #if IL2CPP_SIZEOF_VOID_P == 8
    //                 case IL2CPP_TYPE_I:
    //         #endif
    //                 case IL2CPP_TYPE_I8:
    //                 {
    //                     if (!pesapi_is_int64(env, jsValue))
    //                     {
    //                         return false;
    //                     }
    //                     break;
    //                 }
    //         #if IL2CPP_SIZEOF_VOID_P == 8
    //                 case IL2CPP_TYPE_U:
    //         #endif
    //                 case IL2CPP_TYPE_U8:
    //                 {
    //                     if (!pesapi_is_uint64(env, jsValue))
    //                     {
    //                         return false;
    //                     }
    //                     break;
    //                 }
    //                 case IL2CPP_TYPE_R4:
    //                 case IL2CPP_TYPE_R8:
    //                 {
    //                     if (!pesapi_is_double(env, jsValue))
    //                     {
    //                         return false;
    //                     }
    //                     break;
    //                 }
    //                 case IL2CPP_TYPE_STRING:
    //                 {
    //                     if (!pesapi_is_string(env, jsValue))
    //                     {
    //                         return false;
    //                     }
    //                     break;
    //                 }
    //                 case IL2CPP_TYPE_SZARRAY:
    //                 case IL2CPP_TYPE_CLASS:
    //                 case IL2CPP_TYPE_OBJECT:
    //                 case IL2CPP_TYPE_ARRAY:
    //                 case IL2CPP_TYPE_FNPTR:
    //                 case IL2CPP_TYPE_PTR:
    //                 {
    //                     if (pesapi_is_function(env, jsValue) && (!Class::IsAssignableFrom(il2cpp_defaults.multicastdelegate_class, parameterKlass) || parameterKlass == il2cpp_defaults.multicastdelegate_class))
    //                     {
    //                         return false;
    //                     }
    //                     if (parameterKlass == il2cpp_defaults.object_class)
    //                     {
    //                         continue;
    //                     }
    //                     auto ptr = pesapi_get_native_object_ptr(env, jsValue);
    //                     if (ptr)
    //                     {
    //                         auto objClass = (Il2CppClass *)pesapi_get_native_object_typeid(env, jsValue);
    //                         if (!Class::IsAssignableFrom(parameterKlass, objClass))
    //                         {
    //                             return false;
    //                         }
    //                     }
    //                     //nullptr will match ref type
    //                     break;
    //                 }
    //                 case IL2CPP_TYPE_VALUETYPE:
    //                     /* note that 't' and 'type->type' can be different */
    //                     if (parameterType->type == IL2CPP_TYPE_VALUETYPE && Type::IsEnum(parameterType))
    //                     {
    //                         t = Class::GetEnumBaseType(Type::GetClass(parameterType))->type;
    //                         goto handle_underlying;
    //                     }
    //                     else
    //                     {
    //                         auto objClass = (Il2CppClass *)pesapi_get_native_object_typeid(env, jsValue);
    //                         if (!objClass || !Class::IsAssignableFrom(parameterKlass, objClass))
    //                         {
    //                             return false;
    //                         }
    //                     }
    //                     break;
    //                 case IL2CPP_TYPE_GENERICINST:
    //                     t = GenericClass::GetTypeDefinition(parameterType->data.generic_class)->byval_arg.type;
    //                     goto handle_underlying;
    //                 default:
    //                     IL2CPP_ASSERT(0);
    //             }
    //         }
    //     }
    //     void** args = method->parameters_count > 0 ? (void**)alloca(sizeof(void*) * method->parameters_count) : nullptr;
    //     pesapi_value jsThis = pesapi_get_holder(info);
    //     void* csThis = nullptr;
    //     if (Method::IsInstance(method))
    //     {
    //         csThis = pesapi_get_native_object_ptr(env, jsThis);
    //         Il2CppClass* thisType = method->klass;
    // #ifndef UNITY_2021_1_OR_NEWER
    //         if (Class::IsValuetype(thisType))
    //         {
    //             csThis = ((uint8_t*)csThis) - sizeof(Il2CppObject);
    //         }
    // #endif
    //         if (thisType == il2cpp_defaults.object_class)
    //         {
    //             auto ptrType = (Il2CppClass*) pesapi_get_native_object_typeid(env, jsThis);
    //             if (Class::IsValuetype(ptrType))
    //             {
    //                 csThis = Object::Box(ptrType, csThis);
    //             }
    //         }
    //     }
    //     if (isExtensionMethod)
    //     {
    //         args[0] = pesapi_get_native_object_ptr(env, jsThis);
    //     }
        
    //     for (int i = csArgStart; i < method->parameters_count; ++i) 
    //     {
    //         auto parameterType = Method::GetParam(method, i);
    //         bool passedByReference = parameterType->byref;
    //         bool hasDefault = parameterType->attrs & PARAM_ATTRIBUTE_HAS_DEFAULT;
    //         bool isLastArgument = i == (method->parameters_count - 1);
    //         Il2CppClass* parameterKlass = Class::FromIl2CppType(parameterType);
    //         Class::Init(parameterKlass);
            
    //         if (isLastArgument && hasParamArray)
    //         {
    //             int jsParamStart = i - csArgStart;
    //             auto elementType = Class::FromIl2CppType(&parameterKlass->element_class->byval_arg);
    //             auto arrayLen = js_args_len - jsParamStart > 0 ? js_args_len - jsParamStart : 0;
    //             auto array = Array::NewSpecific(parameterKlass, arrayLen);
    //             if (Class::IsValuetype(elementType))
    //             {
    //                 auto valueSize = elementType->instance_size - sizeof(Il2CppObject);
    //                 char* addr = Array::GetFirstElementAddress(array);
    //                 for(int j = jsParamStart; j < js_args_len; ++j)
    //                 {
    //                     GetValueTypeFromJs(env, pesapi_get_arg(info, j), elementType, addr + valueSize * (j - i + csArgStart));
    //                 }
    //             }
    //             else
    //             {
    //                 for(int j = jsParamStart; j < js_args_len; ++j)
    //                 {
    //                     il2cpp_array_setref(array, j - i + csArgStart, JsValueToCSRef(elementType, env, pesapi_get_arg(info, j)));
    //                 }
    //             }
    //             args[i] = array;
    //             continue;
    //         }
            
    //         pesapi_value jsValue = pesapi_get_arg(info, i - csArgStart);
            
    //         if (Class::IsValuetype(parameterKlass))
    //         {
    //             if (Class::IsNullable(parameterKlass))
    //             {
    //                 void* storage = alloca(parameterKlass->instance_size - sizeof(Il2CppObject));
    //                 auto underlyClass = Class::GetNullableArgument(parameterKlass);
    //                 uint32_t valueSize = underlyClass->instance_size - sizeof(Il2CppObject);
    //                 bool hasValue = GetValueTypeFromJs(env, jsValue, underlyClass, storage);
    // #ifndef UNITY_2021_1_OR_NEWER
    //                 *(static_cast<uint8_t*>(storage) + valueSize) = hasValue;
    // #else
    //                 *(static_cast<uint8_t*>(storage)) = hasValue;
    // #endif    // ! 

    //                 args[i] = storage;
    //             }
    //             else if (passedByReference)
    //             {
    //                 auto underlyClass = Class::FromIl2CppType(&parameterKlass->byval_arg);
    //                 void* storage = alloca(underlyClass->instance_size - sizeof(Il2CppObject));
    //                 jsValue = JsObjectUnRef(env, jsValue);
    //                 GetValueTypeFromJs(env, jsValue, underlyClass, storage);
    //                 args[i] = storage;
    //             }
    //             else if (hasDefault && pesapi_is_undefined(env, jsValue))
    //             {
    //                 void* storage = GetDefaultValuePtr(method, i);
    //                 if (!storage)
    //                 {
    //                     auto valueSize = parameterKlass->instance_size - sizeof(Il2CppObject);
    //                     storage = alloca(valueSize);
    //                     memset(storage, 0, valueSize);
    //                 }
    //                 args[i] = storage;
    //             }
    //             else
    //             {
    //                 auto valueSize = parameterKlass->instance_size - sizeof(Il2CppObject);
    //                 void* storage = alloca(valueSize);
    //                 bool hasValue = GetValueTypeFromJs(env, jsValue, parameterKlass, storage);
    //                 if (!hasValue)
    //                 {
    //                     memset(storage, 0, valueSize);
    //                 }
    //                 args[i] = storage;
    //             }
    //         }
    //         else if (passedByReference)
    //         {
    //             //convertedParameters[i] = &parameters[i]; // Reference type passed by reference
    //             void** arg = (void**)alloca(sizeof(void*));
    //             *arg = nullptr;
    //             auto underlyClass = Class::FromIl2CppType(&parameterKlass->byval_arg);
    //             jsValue = JsObjectUnRef(env, jsValue);
    //             if (jsValue)
    //             {
    //                 auto ptr = pesapi_get_native_object_ptr(env, jsValue);
    //                 if (ptr)
    //                 {
    //                     auto objClass = (Il2CppClass *)pesapi_get_native_object_typeid(env, jsValue);
    //                     if (Class::IsAssignableFrom(underlyClass, objClass))
    //                     {
    //                         *arg = ptr;
    //                     }
    //                 }
    //                 else if (underlyClass == il2cpp_defaults.object_class) // any type
    //                 {
    //                     *arg = JsValueToCSRef(underlyClass, env, jsValue);
    //                 }
    //             }
    //             args[i] = arg;
    //         }
    //         else if (parameterKlass->byval_arg.type == IL2CPP_TYPE_PTR)
    //         {
    //             auto underlyClass = Class::FromIl2CppType(&parameterKlass->element_class->byval_arg);
    //             void* storage = alloca(underlyClass->instance_size - sizeof(Il2CppObject));
    //             jsValue = JsObjectUnRef(env, jsValue);
    //             args[i] = GetValueTypeFromJs(env, jsValue, underlyClass, storage) ? storage : nullptr;
    //         }
    //         else
    //         {
    //             args[i] = (hasDefault && pesapi_is_undefined(env, jsValue)) ? GetDefaultValuePtr(method, i): JsValueToCSRef(parameterKlass, env, jsValue);
    //         }
    //     }
        
    //     Il2CppObject* ret = Runtime::InvokeWithThrow(method, csThis, args); //返回ValueType有boxing
        
    //     for (int i = csArgStart; i < method->parameters_count; ++i)
    //     {
    //         auto parameterType = Method::GetParam(method, i);
    //         bool passedByReference = parameterType->byref;
    //         Il2CppClass* parameterKlass = Class::FromIl2CppType(parameterType);
            
    //         pesapi_value jsValue = pesapi_get_arg(info, i - csArgStart);
            
    //         if (Class::IsValuetype(parameterKlass) && passedByReference)
    //         {
    //             if (Class::IsNullable(parameterKlass))
    //             {
    // #ifndef UNITY_2021_1_OR_NEWER
    //                 bool hasValue = !!*(static_cast<uint8_t*>(args[i]) + parameterKlass->instance_size - sizeof(Il2CppObject));
    // #else
    //                 bool hasValue = !!*(static_cast<uint8_t*>(args[i]));
    // #endif    // ! 
    //                 if (!hasValue)
    //                 {
    //                     JsObjectSetRef(env, jsValue, pesapi_create_null(env));
    //                     continue;
    //                 }
    //             }
    //             auto underlyClass = Class::FromIl2CppType(&parameterKlass->byval_arg);
    //             JsObjectSetRef(env, jsValue, CSRefToJsValue(env, underlyClass, (Il2CppObject*)(((uint8_t*)args[i]) - sizeof(Il2CppObject))));
    //         }
    //         else if (passedByReference)
    //         {
    //             Il2CppObject** arg = (Il2CppObject**)args[i];
    //             auto underlyClass = Class::FromIl2CppType(&parameterKlass->byval_arg);
    //             JsObjectSetRef(env, jsValue, CSRefToJsValue(env, underlyClass, *arg));
    //         }
    //         else if (parameterKlass->byval_arg.type == IL2CPP_TYPE_PTR)
    //         {
    //             auto underlyClass = Class::FromIl2CppType(&parameterKlass->element_class->byval_arg);
    //             JsObjectSetRef(env, jsValue, CSRefToJsValue(env, underlyClass, (Il2CppObject*)(((uint8_t*)args[i]) - sizeof(Il2CppObject))));
    //         }
    //     }
        
    //     auto returnType = Class::FromIl2CppType(method->return_type);
    //     if (returnType != il2cpp_defaults.void_class)
    //     {
    //         pesapi_add_return(info, CSRefToJsValue(env, returnType, ret));
    //     }
        
    //     return true;
    // }

    // static void ReflectionGetFieldWrapper(pesapi_callback_info info, FieldInfo* field, size_t offset, Il2CppClass* fieldType)
    // {
    //     pesapi_env env = pesapi_get_env(info);
    //     pesapi_value jsThis = pesapi_get_holder(info);
    //     void* csThis = nullptr;
    //     if (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
    //     {
    //         csThis = pesapi_get_native_object_ptr(env, jsThis);
    //     }
        
    //     if (Class::IsValuetype(fieldType))
    //     {
    //         void* storage = nullptr;
    //         bool isFieldPtr = true;
    //         auto expectType = fieldType;
    //         if (Class::IsNullable(fieldType))
    //         {
    //             expectType = Class::GetNullableArgument(fieldType);
    //         }
    //         if (Class::IsEnum(fieldType))
    //         {
    //             expectType = Class::GetElementClass(fieldType);
    //         }
            
    //         if ((field->type->attrs & FIELD_ATTRIBUTE_STATIC))
    //         {
    //             int t = Class::GetType(fieldType)->type;
    //             //il2cpp-blob.h
    //             if (t >= IL2CPP_TYPE_BOOLEAN && t <= IL2CPP_TYPE_R8 || t == IL2CPP_TYPE_I || t == IL2CPP_TYPE_U)
    //             {
    //                 storage = alloca(expectType->instance_size - sizeof(Il2CppObject));
    //                 GetFieldValue(csThis, field, offset, storage);
    //                 isFieldPtr = false;
    //             }
    //             else
    //             {
    //                 storage = GetValueTypeFieldPtr(csThis, field, offset);
    //             }
    //         }
    //         else if (csThis)
    //         {
    //             storage = (char*)csThis + offset;
    //         }
    //         if (!storage)
    //         {
    //             storage = alloca(expectType->instance_size - sizeof(Il2CppObject));
    //             GetFieldValue(csThis, field, offset, storage);
    //             isFieldPtr = false;
    //         }

    //         Il2CppObject* obj = (Il2CppObject*) storage - 1;
    //         pesapi_value jsVal = TryTranslatePrimitiveWithClass(env, obj, expectType);
        
    //         if (!jsVal) 
    //         {
    //             if (isFieldPtr)
    //             {
    //                 jsVal = pesapi_create_native_object(env, expectType, storage, false);
    //             }
    //             else
    //             {
    //                 auto valueSize = expectType->instance_size - sizeof(Il2CppObject);
    //                 auto buff = new uint8_t[valueSize];
    //                 memcpy(buff, storage, valueSize);
    //                 jsVal = pesapi_create_native_object(env, expectType, buff, true);
    //             }
    //         }
            
    //         if (jsVal)
    //         {
    //             pesapi_add_return(info, jsVal);
    //         }
    //     }
    //     else
    //     {
    //         void* storage = nullptr;
    //         GetFieldValue(csThis, field, offset, &storage);
    //         pesapi_add_return(info, CSRefToJsValue(env, fieldType, (Il2CppObject*)storage));
    //     }
    // }

    // static void ReflectionSetFieldWrapper(pesapi_callback_info info, FieldInfo* field, size_t offset, Il2CppClass* fieldType)
    // {
    //     pesapi_env env = pesapi_get_env(info);
    //     pesapi_value jsThis = pesapi_get_holder(info);
    //     void* csThis = nullptr;
    //     if (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
    //     {
    //         csThis = pesapi_get_native_object_ptr(env, jsThis);
    //     }
    //     pesapi_value jsValue = pesapi_get_arg(info, 0);
    //     if (Class::IsValuetype(fieldType))
    //     {
    //         if (Class::IsNullable(fieldType))
    //         {
    //             void* storage = alloca(fieldType->instance_size - sizeof(Il2CppObject));
    //             auto underlyClass = Class::GetNullableArgument(fieldType);
    //             uint32_t valueSize = underlyClass->instance_size - sizeof(Il2CppObject);
    //             bool hasValue = GetValueTypeFromJs(env, jsValue, underlyClass, storage);
    // #ifndef UNITY_2021_1_OR_NEWER
    //             *(static_cast<uint8_t*>(storage) + valueSize) = hasValue;
    // #else
    //             *(static_cast<uint8_t*>(storage)) = hasValue;
    // #endif    // ! 
    //             SetFieldValue(csThis, field, offset, storage);
    //         }
    //         else
    //         {
    //             auto valueSize = fieldType->instance_size - sizeof(Il2CppObject);
    //             void* storage = alloca(valueSize);
    //             bool hasValue = GetValueTypeFromJs(env, jsValue, fieldType, storage);
    //             if (!hasValue)
    //             {
    //                 memset(storage, 0, valueSize);
    //             }
    //             SetFieldValue(csThis, field, offset, storage);
    //         }
    //     }
    //     else
    //     {
    //         void* val = JsValueToCSRef(fieldType, env, jsValue);
            
    //         if(!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
    //         {
    //             SetFieldValue(csThis, field, offset, &val);
    //         }
    //         else
    //         {
    //             SetFieldValue(csThis, field, offset, val);
    //         }
    //     }
    // }


    static void ThrowInvalidOperationException(const char* msg)
    {
        //#TODO@benp throw error
    }

    Il2CppArray* NewArray(Il2CppClass *typeId, uint32_t length)
    {
        return Array::NewSpecific(typeId, length);
    }

    void ArraySetRef(Il2CppArray *array, uint32_t index, void* value)
    {
        il2cpp_array_setref(array, index, value);
    }

    // Il2CppObject* GetJSObjectValue(Il2CppObject* jsObject, Il2CppString* key, Il2CppReflectionType* rtype)
    // {
    //     return nullptr;
        // PersistentObjectInfo* objectInfo = reinterpret_cast<PersistentObjectInfo*>(jsObject + 1);

        // const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(key);
        // std::string key_std = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);

        // auto type = il2cpp_codegen_class_from_type(rtype->type);
        // return g_unityExports.GetJSObjectValue(objectInfo, key_std.c_str(), type);
    // }

    struct BridgeFuncInfo
    {
        const char* Signature;
        MethodPointer Method;
    };

    struct WrapFuncInfo
    {
        const char* Signature;
        WrapFuncPtr Method;
    };

    struct FieldWrapFuncInfo
    {
        const char* Signature;
        FieldWrapFuncPtr Getter;
        FieldWrapFuncPtr Setter;
    };

    LuaClassInfo* CreateCSharpTypeInfo(Il2CppString* name, const void* type_id, const void* super_type_id, void* klass, bool isValueType, bool isDelegate, Il2CppString delegateSignature)
    {
        const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(name);
        std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
        xlua::GLogFormatted(str.c_str());
        MethodPointer delegateBridge = nullptr;
        
        LuaClassInfo* ret = new LuaClassInfo();
        ret->Name = str;
        ret->TypeId = type_id;
        ret->SuperTypeId = super_type_id;
        ret->Class = static_cast<TypeIdType*>(klass);
        ret->IsValueType = isValueType;
        ret->DelegateBridge = delegateBridge;
        
        return ret;
    }


    void ReleaseCSharpTypeInfo(LuaClassInfo* classInfo)
    {
        //TODO: 有内存泄漏，需要释放里面的内容
        delete classInfo;
    }

    static void SetParamArrayFlagAndOptionalNum(WrapData* data, const char* signature)
    {
        data->HasParamArray = false;
        data->OptionalNum = 0;
        
        const char* p = signature;
        while(*p)
        {
            if (*p == 'V')
            {
                data->HasParamArray = true;
            }
            if (*p == 'D')
            {
                ++data->OptionalNum;
            }
            ++p;
        }
    }
    
    WrapData* AddConstructor(LuaClassInfo* classInfo, const char* signature, WrapFuncPtr WrapFunc, void* method, MethodPointer methodPointer, int typeInfoNum)
    {
        if (!WrapFunc) return nullptr;
        int allocSize = sizeof(WrapData) + sizeof(void*) * typeInfoNum;
        WrapData* data = (WrapData*)malloc(allocSize);
        memset(data, 0, allocSize);
        data->Method = static_cast<MethodType*>(method);
        data->MethodPointer = methodPointer;
        data->Wrap = WrapFunc;
        data->IsStatic = false;
        data->IsExtensionMethod = false;
        SetParamArrayFlagAndOptionalNum(data, signature);
        
        classInfo->Ctors.push_back(data);
        return data;
    }


    WrapData* AddMethod(LuaClassInfo* classInfo, Il2CppString *signatureIlstring, WrapFuncPtr WrapFunc,  Il2CppString *nameIlstring, bool isStatic, bool isExtensionMethod, bool isGetter, bool isSetter, void* method, MethodPointer methodPointer, int typeInfoNum)
    {
        if (!WrapFunc) return nullptr;
        int allocSize = sizeof(WrapData) + sizeof(void*) * typeInfoNum;
        WrapData* data = (WrapData*)malloc(allocSize);
        memset(data, 0, allocSize);
        data->Method = static_cast<MethodType*>(method);
        data->MethodPointer = methodPointer;
        data->Wrap = WrapFunc;
        data->IsStatic = isStatic;
        data->IsExtensionMethod = isExtensionMethod;
        std::string str = ilstring2stdstring(signatureIlstring);
        const char* signature = str.c_str();
        SetParamArrayFlagAndOptionalNum(data, signature);
        
        std::string nameStr = ilstring2stdstring(nameIlstring);
        const char* name = nameStr.c_str();
        

        for(int i = 0; i < classInfo->Methods.size(); ++i)
        {
            if (classInfo->Methods[i].IsStatic == isStatic && classInfo->Methods[i].IsGetter == isGetter && classInfo->Methods[i].IsGetter == isGetter && classInfo->Methods[i].Name == name)
            {
                if (isGetter || isSetter) // no overload for getter or setter
                {
                    free(data);
                    return nullptr;
                }
                //PLog("add overload for %s, %s", name, signature);
                classInfo->Methods[i].OverloadDatas.push_back(data);
                return data;
            }
        }
        
        std::vector<WrapData*> OverloadDatas;
        OverloadDatas.push_back(data);
        classInfo->Methods.push_back({std::string(name), isStatic, isGetter, isSetter, std::move(OverloadDatas)});
        return data;
    }

    
    bool AddField(LuaClassInfo* classInfo, FieldWrapFuncInfo* wrapFuncInfo, Il2CppString* nameIlstring, bool is_static, void* fieldInfo, int offset, void* fieldTypeInfo)
    {
        FieldWrapFuncPtr Getter = nullptr;
        FieldWrapFuncPtr Setter = nullptr;
        if (wrapFuncInfo) 
        {
            Getter = wrapFuncInfo->Getter;
            Setter = wrapFuncInfo->Setter;
        }
        else
        {
            return false;
        }
        std::string str = ilstring2stdstring(nameIlstring);
        const char* name = str.c_str();

        FieldWrapData* data = new FieldWrapData();
        data->Getter = Getter;
        data->Setter = Setter;
        data->FieldInfo = fieldInfo;
        data->Offset = offset;
        data->TypeInfo = fieldTypeInfo;
        
        classInfo->Fields.push_back({std::string(name), is_static, data});
        return true;
    }

    void SetTypeInfo(WrapData* data, int index, void* typeInfo)
    {
        data->TypeInfos[index] = typeInfo;
    }

    #include "FunctionBridge.Gen.h"



    
    MethodPointer FindBridgeFunc(Il2CppString * ilstring)
    {
        std::string str = ilstring2stdstring(ilstring);
        const char* signature = str.c_str();
        xlua::GLogFormatted("FindBridgeFunc ilstring %s",signature);
        auto begin = &g_bridgeFuncInfos[0];
        auto end = &g_bridgeFuncInfos[sizeof(g_bridgeFuncInfos) / sizeof(BridgeFuncInfo) - 1];
        auto first = std::lower_bound(begin, end, signature, [](const BridgeFuncInfo& x, const char* signature) {return strcmp(x.Signature, signature) < 0;});
        if (first != end && strcmp(first->Signature, signature) == 0) {
            return first->Method;
        }
        return nullptr;
    }

    WrapFuncPtr FindWrapFunc(Il2CppString * ilstring)
    {
        std::string str = ilstring2stdstring(ilstring);
        const char* signature = str.c_str();
        auto begin = &g_wrapFuncInfos[0];
        auto end = &g_wrapFuncInfos[sizeof(g_wrapFuncInfos) / sizeof(WrapFuncInfo) - 1];
        auto first = std::lower_bound(begin, end, signature, [](const WrapFuncInfo& x, const char* signature) {return strcmp(x.Signature, signature) < 0;});
        if (first != end && strcmp(first->Signature, signature) == 0) {
            xlua::GLogFormatted("FindWrapFunc %s",signature);
            return first->Method;
        }
        return nullptr;
    }

    FieldWrapFuncInfo * FindFieldWrapFuncInfo(Il2CppString * ilstring)
    {
        std::string str = ilstring2stdstring(ilstring);
        const char* signature = str.c_str();
        xlua::GLogFormatted("FindFieldWrapFuncInfo %s",signature);
        auto begin = &g_fieldWrapFuncInfos[0];
        auto end = &g_fieldWrapFuncInfos[sizeof(g_fieldWrapFuncInfos) / sizeof(FieldWrapFuncInfo) - 1];
        auto first = std::lower_bound(begin, end, signature, [](const FieldWrapFuncInfo& x, const char* signature) {return strcmp(x.Signature, signature) < 0;});
        if (first != end && strcmp(first->Signature, signature) == 0) {
            return first;
        }
        
        return nullptr;
    }
    
    
    static xlua::LuaClassInfo* GetLuaClsInfo(lua_State* L) {
        if (lapi_lua_isuserdata(L, 1)) {
            void* ptr = lapi_xlua_getcsobj_ptr(L, 1);
            if (ptr) {
                void* kclass = *reinterpret_cast<void**>(ptr);
                auto clsInfo = xlua::GetLuaClsInfoByTypeId(kclass);
                return clsInfo;
            }
        }
        return nullptr;
    }

    static xlua::LuaClassInfo* GetLuaClsInfoByClsMeta(lua_State* L) {
        if (lapi_lua_isuserdata(L, lapi_lua_upvalueindex(1))) {
            const void* ptr = lapi_lua_topointer(L, lapi_lua_upvalueindex(1));
            xlua::LuaClassInfo* clsInfo = xlua::GetLuaClsInfoByTypeId(ptr);
            return clsInfo;
        }
        return nullptr;
    }

    //class的set回调
    int ClsSetCallBack(lua_State *L){
        auto clsInfo = GetLuaClsInfoByClsMeta(L);

        if(clsInfo){
             if (lapi_lua_isstring(L, 2)) {
                const char* key = lapi_lua_tolstring(L, 2); 
                auto fieldIter = clsInfo->FieldMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换
                if (fieldIter != clsInfo->FieldMap.end())
                {
                    // find method 
                    auto field = fieldIter->second;
                    if (field->IsStatic && field->Data->Setter) {
                        auto setter = field->Data->Setter;
                        setter(L, (FieldInfo*)field->Data->FieldInfo, field->Data->Offset, (Il2CppClass*)field->Data->TypeInfo);
                    }
                    return 0;
                }
            }
        }
        return 0;
    }

    int MethodCallbackLuaWrap(lua_State* L) {
        
        if (lapi_lua_isuserdata(L, lapi_lua_upvalueindex(1))) {
            const void* pointer = lapi_lua_topointer(L, lapi_lua_upvalueindex(1));
            void* p1 = const_cast<void*>(pointer);
            xlua::WrapData** wrapDatas = reinterpret_cast<xlua::WrapData**>(p1);
            if (wrapDatas) {
                return xlua::MethodCallback(L, wrapDatas);
            }
        }

        return 0;
    }

    //class的get回调
    int ClsGetCallBack(lua_State *L){
        auto clsInfo = GetLuaClsInfoByClsMeta(L);
        
        if(clsInfo){
            //#TODO@benp 这里可以考虑初始化静态变量
            //method
            //todo 考虑不使用closure 优势1 减少闭包的内存 2 减少对Lua堆栈的操作 后续测试确定
            // todo 这里先注释  里面有脏东西
            //if (!lapi_lua_isnil(L, lapi_lua_upvalueindex(2))) {
            //    lapi_lua_pushvalue(L, 2);
            //    lapi_lua_gettable(L, lapi_lua_upvalueindex(2));
            //    if (!lapi_lua_isnil(L, -1)) {//has method
            //        return 1;
            //    }
            //    lapi_lua_pop(L, 1);
            //}
            
            //find method or field
            if (lapi_lua_isstring(L, 2)) {
                
                const char* key = lapi_lua_tolstring(L, 2); //#TODO@benp 这里会有char* => string的隐式转换

                auto iter = clsInfo->MethodsMap.find(key);
                if(iter != clsInfo->MethodsMap.end())
                {
                    // find method 
                    auto method = iter->second;
                    if(method->IsStatic){
                        xlua::WrapData** p = method->OverloadDatas.data();
                        lapi_lua_pushlightuserdata(L, p);
                        lapi_lua_pushcclosure(L, MethodCallbackLuaWrap, 1);
                        //cache method
                        lapi_lua_pushvalue(L, lapi_lua_upvalueindex(2)); //-1 table   -2closure 
                        if(lapi_lua_type(L, lapi_lua_gettop(L)) == LUA_TTABLE){
                            lapi_lua_pushvalue(L, 2);   //-1 stringkey -2 table   -3closure 
                            lapi_lua_pushvalue(L, -3); //-1 closure -2 stringkey -3 table   -4closure 
                            lapi_lua_settable(L, -3);
                        }
                        lapi_lua_pop(L,1);
                        return 1;
                    }
                    //找不到就退出不允许重复的key
                    return 0;
                }

                auto fieldIter = clsInfo->FieldMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换
                if(fieldIter != clsInfo->FieldMap.end())
                {
                    auto field = fieldIter->second;
                    if(field->IsStatic && field->Data->Getter){
                        field->Data->Getter(L, (FieldInfo*)field->Data->FieldInfo, field->Data->Offset, (Il2CppClass*)field->Data->TypeInfo);
                        return 1;
                    }
                    //找不到就退出不允许重复的key
                    return 0;
                }
            }
        }
        return 0;
    }

    int ClsConstructorCallBack(lua_State *L){
        //xlua::GLogFormatted("ClsConstructorCallBack Begin" );
        auto clsInfo = GetLuaClsInfoByClsMeta(L);
        //int type = lapi_lua_type(L,lapi_lua_upvalueindex(1));
        
        /*if (type == LUA_TLIGHTUSERDATA) {
            const void * ptr = lapi_lua_topointer(L, lapi_lua_upvalueindex(1));
            
            xlua::GLogFormatted("light user data %p", ptr);
            xlua::LuaClassInfo* clsInfo = xlua::GetLuaClsInfoByTypeId(ptr);*/
            if(clsInfo){
                //xlua::GLogFormatted("find LuaClassDefinition %s", clsInfo->Name.c_str());

                void* ptr = xlua::ObjectAllocate(clsInfo->Class);

                auto isValueType = Class::IsValuetype(clsInfo->Class);
                if(!isValueType){
                    //-1 clsTable -2 param -3 obj
                    if (GetCppObjMapper()->TryPushObject(L, ptr)) {
                        //-1 obj -2 param
                        lapi_lua_replace(L, 1);
                        int top = lapi_lua_gettop(L);
                        xlua::GLogFormatted("top %d", top);
                        if (clsInfo->CtorWrapDatas) {
                            //lapi_lua_pushvalue(L, -1);
                            //#TODO@benp 构造函数失败  清除引用
                            return xlua::MethodCallback(L, clsInfo->CtorWrapDatas, -1);
                        }
                    }
                    else {
                        //todo throw error
                        throw_exception2lua(L, "push valu to lua failed");
                        return 0;
                    }
                }else{
                    //#TODO@benp copy value

                }
                return 1;
            }
        //}
        return 0;
    }



    int FieldCallbackLuaWrap(lua_State *L){
        if(lapi_lua_isuserdata(L, lapi_lua_upvalueindex(1))){
            xlua::FieldWrapData* wrapData = (xlua::FieldWrapData*) (lapi_lua_topointer(L, lapi_lua_upvalueindex(1)));
            if(wrapData){
                wrapData->Getter(L, ( FieldInfo*)wrapData->FieldInfo, wrapData->Offset, (Il2CppClass*)wrapData->TypeInfo);
                return 1;
            }
        }
        return 0;
    }

    int FieldGetCallbackLuaWrap(lua_State* L) {
        if (lapi_lua_isuserdata(L, lapi_lua_upvalueindex(1))) {
            xlua::FieldWrapData* wrapData = (xlua::FieldWrapData*)(lapi_lua_topointer(L, lapi_lua_upvalueindex(1)));
            if (wrapData) {
                wrapData->Getter(L, (FieldInfo*)wrapData->FieldInfo, wrapData->Offset, (Il2CppClass*)wrapData->TypeInfo);
                return 1;
            }
        }
        return 0;
    }

    



    ///Lua CSharp对象的setter函数
    /// 1 getFromCache 1 method 2 getter
    /// 2 cspointer=>kclass=>luaDef
    /// 3 luaDef=> method or field
    /// 4 push result
    int ObjSetCallBack(lua_State *L){
        xlua::GLogFormatted("ObjSetCallBack");
        auto clsInfo = xlua::GetLuaClsInfo(L);
        if (clsInfo) {
            //find method or field
            if (lapi_lua_isstring(L, 2)) {

                const char* key = lapi_lua_tolstring(L, 2); //#TODO@benp 这里会有char* => string的隐式转换

                auto fieldIter = clsInfo->FieldMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换
                if (fieldIter != clsInfo->FieldMap.end())
                {
                    // find method 
                    auto field = fieldIter->second;
                    if (!field->IsStatic && field->Data->Setter) {
                        auto setter = field->Data->Setter;
                        setter(L, (FieldInfo*)field->Data->FieldInfo, field->Data->Offset, (Il2CppClass*)field->Data->TypeInfo);
                    }
                    return 0;
                }
            }
            else if (lapi_lua_isnumber(L, 2)) {
                //#TODO@benp 数组处理
            }

        }
        else {
            //todo throw error
        }
        return 0;
    }


    ///Lua CSharp对象的getter函数
    /// 1 getFromCache 1 method 2 getter
    /// 2 cspointer=>kclass=>luaDef
    /// 3 luaDef=> method or field
    /// 4 push result
    int ObjGetCallBack(lua_State *L){
        xlua::GLogFormatted("ObjGetCallBack");
        //#TODO@benp 校验 可以通过xluatag or getDef的形式
        //method
        //todo 考虑不使用closure 优势1 减少闭包的内存 2 减少对Lua堆栈的操作 后续测试确定
        if (!lapi_lua_isnil(L, lapi_lua_upvalueindex(2))) {
            lapi_lua_pushvalue(L, 2);
            lapi_lua_gettable(L, lapi_lua_upvalueindex(2));
            if (!lapi_lua_isnil(L, -1)) {//has method
                return 1;
            }
            lapi_lua_pop(L, 1);
        }

        auto clsInfo = GetLuaClsInfo(L);
        while (clsInfo) {

            //find method or field
            if (lapi_lua_isstring(L, 2)) {
                
                const char* key = lapi_lua_tolstring(L, 2); 

                auto iter = clsInfo->MethodsMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换
                if(iter != clsInfo->MethodsMap.end())
                {
                    // find method 
                    auto method = iter->second;
                    if(!method->IsSetter && !method->IsGetter && !method->IsStatic){
                        xlua::WrapData** p = method->OverloadDatas.data();
                        lapi_lua_pushlightuserdata(L, p);
                        lapi_lua_pushcclosure(L, MethodCallbackLuaWrap, 1);
                        //cache method
                        lapi_lua_pushvalue(L, lapi_lua_upvalueindex(2)); //-1 table   -2closure 
                        if(lapi_lua_type(L, lapi_lua_gettop(L)) == LUA_TTABLE){
                            lapi_lua_pushvalue(L, 2);   //-1 stringkey -2 table   -3closure 
                            lapi_lua_pushvalue(L, -3); //-1 closure -2 stringkey -3 table   -4closure 
                            lapi_lua_settable(L, -3);
                        }
                        lapi_lua_pop(L,1);
                        return 1;
                    }
                    //找不到就退出不允许重复的key
                    return 0;
                }

                auto fieldIter = clsInfo->FieldMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换
                if(fieldIter != clsInfo->FieldMap.end())
                {
                    // find method  
                    auto field = fieldIter->second;
                    if(!field->IsStatic && field->Data->Getter){
                        field->Data->Getter(L, (FieldInfo*)field->Data->FieldInfo, field->Data->Offset, (Il2CppClass*)field->Data->TypeInfo);
                        return 1;
                    }
                    //找不到就退出不允许重复的key
                    return 0;
                }
            }
            else if(lapi_lua_isnumber(L, 2)) {
                //#TODO@benp 数组处理
            }
            if(clsInfo->SuperTypeId){
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass*)clsInfo->SuperTypeId, L);
            }else{
                clsInfo = nullptr;
            }
        }
        
        return 0;
    }

    void SetGetTypeFuncPointer(Il2CppReflectionMethod* method){
        auto methodPointer = xlua::GetMethodPointer(method);
        auto methodInfo = xlua::GetMethodInfoPointer(method);
        xlua::GetLuaClassRegister()->SetGetTypeIdFuncPtr((CSharpGetTypeId)methodPointer, (void*)methodInfo);
    }
    xlua::UnityExports* GetUnityExports()
    {
        g_unityExports.ObjectAllocate = &ObjectAllocate;
        g_unityExports.DelegateAllocate = &DelegateAllocate; 
        g_unityExports.ValueTypeDeallocate = &ValueTypeFree;
        // g_unityExports.MethodCallback = &MethodCallback;
        //g_unityExports.ConstructorCallback = &CtorCallback;
        g_unityExports.FieldGet = &GetFieldValue;
        g_unityExports.FieldSet = &SetFieldValue;
        g_unityExports.GetValueTypeFieldPtr = &GetValueTypeFieldPtr;
        g_unityExports.IsInst = &Object::IsInst;
        g_unityExports.IsInstClass = &IsInstClass;
        g_unityExports.IsInstSealed = &IsInstSealed;
        g_unityExports.IsValueType = &IsValueType;
        g_unityExports.IsDelegate = &IsDelegate;
        g_unityExports.IsAssignableFrom = &Class::IsAssignableFrom;
        g_unityExports.CSharpTypeToTypeId = &CSharpTypeToTypeId;
        g_unityExports.CStringToCSharpString = &String::NewWrapper;
        g_unityExports.GetTID = &GetTID;
        g_unityExports.ThrowInvalidOperationException = &ThrowInvalidOperationException;
        g_unityExports.GetReturnType = &GetReturnType;
        g_unityExports.GetParameterType = &GetParameterType;
        g_unityExports.NewArray = NewArray;
        g_unityExports.GetArrayFirstElementAddress = Array::GetFirstElementAddress;
        g_unityExports.ArraySetRef = ArraySetRef;
        g_unityExports.GetArrayElementTypeId = Class::GetElementClass;
        g_unityExports.GetArrayLength = Array::GetLength;
        g_unityExports.GetDefaultValuePtr = GetDefaultValuePtr;
        // g_unityExports.ReflectionWrapper = ReflectionWrapper;
        // g_unityExports.ReflectionGetFieldWrapper = ReflectionGetFieldWrapper;
        // g_unityExports.ReflectionSetFieldWrapper = ReflectionSetFieldWrapper;
        g_unityExports.SizeOfRuntimeObject = sizeof(RuntimeObject);
        return &g_unityExports;
    }
}

// namespace end

#ifdef __cplusplus
extern "C" {
#endif


void InitialXLua_IL2CPP(lapi_func_ptr* func_array)
{
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetMethodPointer(System.Reflection.MethodBase)", (Il2CppMethodPointer)xlua::GetMethodPointer);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetMethodInfoPointer(System.Reflection.MethodBase)", (Il2CppMethodPointer)xlua::GetMethodInfoPointer);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::SetGetTypeFuncPointer(System.Reflection.MethodBase)", (Il2CppMethodPointer)xlua::SetGetTypeFuncPointer);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetObjectPointer(System.Object)", (Il2CppMethodPointer)xlua::GetObjectPointer);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetTypeId(System.Type)", (Il2CppMethodPointer)xlua::GetTypeId);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::TypeIdToType(System.IntPtr)", (Il2CppMethodPointer)xlua::TypeIdToType);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetFieldOffset(System.Reflection.FieldInfo,System.Boolean)", (Il2CppMethodPointer)xlua::GetFieldOffset);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetFieldInfoPointer(System.Reflection.FieldInfo)", (Il2CppMethodPointer)xlua::GetFieldInfoPointer);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetUnityExports()", (Il2CppMethodPointer)xlua::GetUnityExports);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::CreateCSharpTypeInfo(System.String,System.IntPtr,System.IntPtr,System.IntPtr,System.Boolean,System.Boolean,System.String)", (Il2CppMethodPointer)xlua::CreateCSharpTypeInfo);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::SetTypeInfo(System.IntPtr,System.Int32,System.IntPtr)", (Il2CppMethodPointer)xlua::SetTypeInfo);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::FindWrapFunc(System.String)", (Il2CppMethodPointer)xlua::FindWrapFunc);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::FindFieldWrap(System.String)", (Il2CppMethodPointer)xlua::FindFieldWrapFuncInfo);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::AddConstructor(System.IntPtr,System.String,System.IntPtr,System.IntPtr,System.IntPtr,System.Int32)", (Il2CppMethodPointer)xlua::AddConstructor);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::AddField(System.IntPtr,System.IntPtr,System.String,System.Boolean,System.IntPtr,System.Int32,System.IntPtr)", (Il2CppMethodPointer)xlua::AddField);

    InternalCalls::Add("XLua.IL2CPP.NativeAPI::AddMethod(System.IntPtr,System.String,System.IntPtr,System.String,System.Boolean,System.Boolean,System.Boolean,System.Boolean,System.IntPtr,System.IntPtr,System.Int32)", (Il2CppMethodPointer)xlua::AddMethod);
    lapi_init(func_array);
    
}

void SetLogCallback(xlua::LogCallback log)
{
    xlua::SetLogHandler(log);
}

static void dumpstack (lua_State *L) {
  int top=lapi_lua_gettop(L);
  for (int i=1; i <= top; i++) {
    xlua::GLogFormatted("dump stack i:%d type:%s", i, lapi_lua_type(L,i));
    switch (lapi_lua_type(L, i)) {
      case LUA_TNUMBER:
        xlua::GLogFormatted("%g\n",lapi_lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        xlua::GLogFormatted("%s\n",lapi_lua_tolstring(L,i));
        break;
      case LUA_TBOOLEAN:
        xlua::GLogFormatted("%s\n", (lapi_lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        xlua::GLogFormatted("%s\n", "nil");
        break;
      default:
        xlua::GLogFormatted("%p\n",lapi_lua_topointer(L,i));
        break;
    }
  }
}



void HandleObjMetatable(lua_State *L, int metatable_idx, void* typeId){
    lapi_lua_pushstring(L, "__index");
    lapi_lua_pushlightuserdata(L, typeId);
    lapi_lua_createtable(L, 0,0);
    lapi_lua_createtable(L, 0,0);
    lapi_lua_pushcclosure(L, xlua::ObjGetCallBack, 3);
    lapi_lua_settable(L, metatable_idx);

    lapi_lua_pushstring(L, "__newindex");
    lapi_lua_pushlightuserdata(L, typeId);
    lapi_lua_createtable(L, 0,0);
    lapi_lua_pushcclosure(L, xlua::ObjSetCallBack, 2);
    lapi_lua_settable(L, metatable_idx);
    //#TODO@benp gc
    //#TODO@benp tostring
}

void HandleClsMetaTable(lua_State *L, int clstable_idx, void* typeId){
    lapi_lua_pushstring(L, "__call");
    lapi_lua_pushlightuserdata(L, typeId);
    lapi_lua_pushcclosure(L, xlua::ClsConstructorCallBack, 1);
    lapi_lua_settable(L, clstable_idx);

    lapi_lua_pushstring(L, "__index");
    lapi_lua_pushlightuserdata(L, typeId);
    lapi_lua_pushcclosure(L, xlua::ClsGetCallBack, 1);
    lapi_lua_settable(L, clstable_idx);

    lapi_lua_pushstring(L, "__newindex");
    lapi_lua_pushlightuserdata(L, typeId);
    lapi_lua_pushcclosure(L, xlua::ClsSetCallBack, 1);
    lapi_lua_settable(L, clstable_idx);
}


int RegisterLuaClass(xlua::LuaClassInfo *luaClsInfo)
{
    return xlua::GetLuaClassRegister()->RegisterClass(luaClsInfo);
}

void UnRegisterLuaClass(Il2CppString* ilstring){
    const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(ilstring);
    auto name = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
    xlua::GetLuaClassRegister()->UnRegisterClass(name);
}


void SetLuaCacheRef(int cache_ref){
    GetCppObjMapper()->SetCacheRef(cache_ref);
}

void SetClassMetaId(void * ilclass, int metaId){
    GetCppObjMapper()->SetTypeId(ilclass, metaId);
}




#ifdef __cplusplus
}
#endif