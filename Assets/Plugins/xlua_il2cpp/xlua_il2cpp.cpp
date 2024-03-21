/// luastack model 
/// paramOffset 第一个参数所在lua栈的位置 如果没有参数填最后一个参数的位置
/// clsMethod 1 params ...
/// clsProperty get 1 clsTbl
/// clsProperty set 1 clsTbl 2 params ...
/// clsField 1 clsTbl 2 key 3 params ...
/// clsConstructor 1 obj 2params
/// objMethod 1 obj 2 params ...
/// objProperty get 1 obj
/// objProperty set 1 obj 2 params ...
/// objField 1 obj 2 key 3 params ...
/// objIndexer 1 obj 2 param1(key) 3 param2(value)
/// operation 
//#TODO@benp 资源释放 malloc new
//#TODO@benp 静态变量初始化 IL2CPP_RUNTIME_CLASS_INIT
//#TODO@benp tostring 
//#TODO@benp 可变参数
//#TODO@benp 反射实现
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
#include "Converter.cpp"
#include <mutex>
#include <vm-utils/NativeDelegateMethodCache.h>
#include "Il2CppTools.h"
static_assert(IL2CPP_GC_BOEHM, "Only BOEHM GC supported!");

using namespace il2cpp::vm;

struct cmp_str
{
    bool operator()(char const* a, char const* b) const
    {
        return std::strcmp(a, b) < 0;
    }
};

namespace xlua
{
    static xlua::UnityExports g_unityExports;
    static int32_t errorFunc_ref = -1;
    typedef void *((*DelegateCombineType)) (Il2CppDelegate *d1, Il2CppDelegate* d2, MethodInfo* methodInfo);
    static DelegateCombineType DelegateCombineMethodPointer = nullptr;
    static const MethodInfo* DelegateCombineMethodInfo = nullptr;

    typedef void *((*DelegateRemoveType)) (Il2CppDelegate *d1, Il2CppDelegate* d2, MethodInfo* methodInfo);
    static DelegateRemoveType DelegateRemoveMethodPointer = nullptr;
    static const MethodInfo* DelegateRemoveMethodInfo = nullptr;

    typedef Il2CppDelegate *((*GetCacheDelegateType)) (lua_State* L, int referenced, const MethodInfo* methodInfo);
    static GetCacheDelegateType GetCacheDelegateMethodPointer = nullptr;
    static const MethodInfo* GetCacheDelegateMethodInfo = nullptr;

    typedef void ((*CacheDelegateType)) (lua_State* L, int referenced, RuntimeObject* obj, const MethodInfo* methodInfo);
    static CacheDelegateType CacheDelegateMethodPointer = nullptr;
    static const MethodInfo* CacheDelegateMethodInfo = nullptr;

    Il2CppDelegate* GetCacheDelegate(lua_State* L, int referenced){
       return GetCacheDelegateMethodPointer(L, referenced, GetCacheDelegateMethodInfo);
    }

    void CacheDelegate(lua_State* L, int referenced, Il2CppDelegate* delegate){
        CacheDelegateMethodPointer(L, referenced, (RuntimeObject*)delegate, CacheDelegateMethodInfo);
    }

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
        //return (int32_t)Field::GetOffset(field->field);
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

    //
    static int32_t GetValueTypeSize(Il2CppClass *klass){
        if(klass->native_size){
            return klass->native_size;
        }else{
            return klass->instance_size - sizeof(RuntimeObject);
        }
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

    void* GetCSharpStructPointer(lua_State* L, int index, void* typeId) {
        auto css = lapi_xlua_tocss(L, index);
        if (css && css->fake_id == -1 && (typeId ? true : typeId == css->typeId)) {
            return &css->data[0];
        }
        return {};
    }

    template <typename T>
    static T* GetCSharpStructPointerFromLua(lua_State* L, int index, void* typeId) {
        auto css = lapi_xlua_tocss(L, index);
        if (css && css->fake_id == -1 && (typeId ? true : typeId == css->typeId)) {
            return  reinterpret_cast<T*>(&css->data[0]);
        }
        return nullptr;
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
    static std::recursive_mutex WrapFuncPtrToMethodInfoMutex;

    Il2CppDelegate* FunctionPointerToDelegate(Il2CppMethodPointer functionPtr, Il2CppClass* delegateType, Il2CppObject* target)
    {
        Il2CppObject* delegate = il2cpp::vm::Object::New(delegateType);
        const MethodInfo* invoke = il2cpp::vm::Runtime::GetDelegateInvoke(delegateType);

        const MethodInfo* method = NULL;
        {
            std::lock_guard<std::recursive_mutex> lock(WrapFuncPtrToMethodInfoMutex);
            il2cpp::utils::NativeDelegateMethodCache::GetNativeDelegate((Il2CppMethodPointer)invoke);
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
                newMethod->is_marshaled_from_native = true;
                il2cpp::utils::NativeDelegateMethodCache::AddNativeDelegate((Il2CppMethodPointer)invoke, newMethod);
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

    static PersistentObjectInfo* GetDelegateInfo(void* target){
        return (PersistentObjectInfo*)(((char*)target) + sizeof(RuntimeObject));
    }

    static Il2CppDelegate* DelegateAllocate(Il2CppClass *klass, Il2CppMethodPointer functionPtr, void** outTargetData)
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

    void SetGlobalType_LuaObject(Il2CppReflectionType *type)
    {
        if (!type)
        {
            Exception::Raise(Exception::GetInvalidOperationException("type of JSObject is null"));
        }
        g_typeofPersistentObjectInfo = il2cpp_codegen_class_from_type(type->type);
    }

    void SetGlobalType_TypedValue(Il2CppReflectionType *type)
    {
        if (!type)
        {
            Exception::Raise(Exception::GetInvalidOperationException("type of TypedValue is null"));
        }
        g_typeofTypedValue = il2cpp_codegen_class_from_type(type->type);
    }

    static int PropertyCallback(lua_State* L, WrapData* wrapDatas, int offset = 3) {
        try
        {
            bool checkArgument = true;
            if (wrapDatas->Wrap(wrapDatas->Method, wrapDatas->MethodPointer, L, checkArgument, wrapDatas, offset) >= 0) {
                return 1;
            }
            else {

                return throw_exception2lua(L, "invalid arguments for PropertyCallback");
            }
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
                throw_exception2lua(L, str.c_str());
            }
        }
        return 0;
    }

    static int MethodCallback(lua_State *L, WrapData** wrapDatas, int paramOffset) {
        try 
        {
             bool checkArgument = *wrapDatas && *(wrapDatas + 1);
             while(*wrapDatas)
             {
                 int n = (*wrapDatas)->Wrap((*wrapDatas)->Method, (*wrapDatas)->MethodPointer, L, checkArgument, *wrapDatas, paramOffset);
                 if (n >= 0)
                 {
                     return n;
                 }
                 ++wrapDatas;
             }
            throw_exception2lua(L, "invalid arguments"); 
            return -1;
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

    /*bool IsDelegate(Il2CppClass *klass)
    {
        return Class::IsAssignableFrom(il2cpp_defaults.delegate_class, klass) && klass != il2cpp_defaults.delegate_class && klass != il2cpp_defaults.multicastdelegate_class;
    }*/

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

    static bool TryTranslatePrimitiveWithType(lua_State* L, void* ptr, const Il2CppType* type) {
        int t = type->type;
        switch (t)
        {
        case IL2CPP_TYPE_I1:
        {
            lapi_xlua_pushinteger(L, (int32_t)(*((int8_t*)ptr)));
        }
        case IL2CPP_TYPE_BOOLEAN:
        {
            lapi_lua_pushboolean(L, (int)(*((uint8_t*)ptr)));
        }
        case IL2CPP_TYPE_U1:
        {
            lapi_xlua_pushinteger(L, (uint32_t)(*((uint8_t*)ptr)));
        }
        case IL2CPP_TYPE_I2:
        {
            lapi_xlua_pushinteger(L, (int32_t)(*((int16_t*)ptr)));
        }
        case IL2CPP_TYPE_U2:
        {
            lapi_xlua_pushinteger(L, (uint32_t)(*((uint16_t*)ptr)));
        }
        case IL2CPP_TYPE_CHAR:
        {
            lapi_xlua_pushinteger(L, (int32_t)(*((Il2CppChar*)ptr)));
        }
#if IL2CPP_SIZEOF_VOID_P == 4
        case IL2CPP_TYPE_I:
#endif
        case IL2CPP_TYPE_I4:
        {
            lapi_xlua_pushinteger(L, (int32_t)(*((int32_t*)ptr)));
        }
#if IL2CPP_SIZEOF_VOID_P == 4
        case IL2CPP_TYPE_U:
#endif
        case IL2CPP_TYPE_U4:
        {
            lapi_xlua_pushinteger(L, (uint32_t)(*((uint32_t*)ptr)));
        }
#if IL2CPP_SIZEOF_VOID_P == 8
        case IL2CPP_TYPE_I:
#endif
        case IL2CPP_TYPE_I8:
        {
            lapi_xlua_pushinteger(L, *((int64_t*)ptr));
        }
#if IL2CPP_SIZEOF_VOID_P == 8
        case IL2CPP_TYPE_U:
#endif
        case IL2CPP_TYPE_U8:
        {
            lapi_xlua_pushinteger(L, *((uint64_t*)ptr));
        }
        case IL2CPP_TYPE_R4:
        {
            lapi_lua_pushnumber(L, (double)(*((float*)ptr)));
        }
        case IL2CPP_TYPE_R8:
        {
            lapi_lua_pushnumber(L, *((double*)ptr));
        }

        default:
            return false;
        }
        return true;
    }

    static bool TryTranslatePrimitiveWithClass(lua_State* L, Il2CppObject* obj, Il2CppClass *klass = nullptr)
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
                return true;
            }
            void* ptr = Object::Unbox(obj);
            return TryTranslatePrimitiveWithType(L, ptr, type);
        }
        return true;
    }

    bool TryTranslatePrimitive(lua_State* L, Il2CppObject* obj)
    {
        return TryTranslatePrimitiveWithClass(L, obj);
    }

    static bool CSValueToLuaValue(lua_State* L, void* klass, void* ptr, unsigned int size) {
        return GetCppObjMapper()->TryPushStruct(L, klass, ptr, size);
    }
    static bool CopyNullableValueType(lua_State* L, void* klass, void* ptr, bool hasValue, unsigned int size) {
        if (!hasValue) {
            lapi_lua_pushnil(L);
            return true;
        }
        return CSValueToLuaValue(L, klass, ptr, size);
    }
   
    static bool CSRefToLuaValue(lua_State* L, void* obj) {
        return GetCppObjMapper()->TryPushObject(L, obj);
    }

    /// @brief C#obj传递到lua 处理struct copy 值类型装箱
    /// @param L 
    /// @param obj 
    /// @param klass 
    /// @return 
    static bool CSAnyToLuaValue(lua_State* L, void* obj, void* klass = nullptr)
    {
        if (!obj) {
            std::string message = "obj null wrong for CSAnyToLuaValue: ";
            Exception::Raise(Exception::GetInvalidOperationException(message.c_str()));
            return false;
        }
        Il2CppObject* il2cppObj = (Il2CppObject*)obj;
        Il2CppClass* ilcppClass = (Il2CppClass*)(klass ? klass : il2cppObj->klass);
        bool suc = TryTranslatePrimitive(L, il2cppObj);

        if (!suc) {
            if (Class::IsValuetype(ilcppClass)) {
                if (il2cpp_class_is_enum(ilcppClass)) {
                    auto t = Class::GetEnumBaseType(ilcppClass);
                    void* ptr = Object::Unbox(il2cppObj);
                    return TryTranslatePrimitiveWithType(L, ptr, t);
                }
                else {
                    return GetCppObjMapper()->TryPushStruct(L, ilcppClass, Object::Unbox(il2cppObj), GetValueTypeSize(ilcppClass));
                }
            }
            else {
                return GetCppObjMapper()->TryPushObject(L, obj);
            }
        }
        return true;
    }
    
    Il2CppClass* GetLuaObjCls(lua_State *L, int index){
        if(lapi_lua_isuserdata(L, index)){
            void* ptr = lapi_xlua_getcsobj_ptr(L, index);
            if(ptr){
                auto obj = (Il2CppObject*) ptr;
                return obj->klass;
            }

            CSharpStruct* css = lapi_xlua_tocss(L, index);
            if(css){
                return (Il2CppClass*)css->typeId;
            }
        }
        return nullptr;
    }

    //从lua中构造一个C#的delegate  此处要考虑缓存
    Il2CppDelegate* CreateDelegateFromLua(lua_State*L, int32_t index, Il2CppClass* klass, bool throwIfFail){
        LuaClassInfo* clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId(klass,  L);
        if(clsInfo){
            lapi_lua_pushvalue(L, index);
            lapi_lua_rawget(L, lapi_xlua_get_registry_index());
            if(!lapi_lua_isnil(L, -1)){
                int referenced = lapi_xlua_tointeger(L, -1);
                lapi_lua_pop(L, 1);
                auto delegate = GetCacheDelegate(L, referenced);
                if(delegate){
                    return delegate;
                }
            }
            lapi_lua_pop(L, 1);
            void* target = nullptr;
            auto delegate = DelegateAllocate(klass, clsInfo->DelegateBridge, &target);
            if(delegate){
                lapi_lua_pushvalue(L, index);
                int reference = lapi_luaL_ref(L, lapi_xlua_get_registry_index());
                lapi_lua_pushvalue(L, index);
                lapi_lua_pushnumber(L, reference);
                lapi_lua_rawset(L, lapi_xlua_get_registry_index());

                PersistentObjectInfo* delegateInfo = (PersistentObjectInfo*)target;

                delegateInfo->L = L;
                delegateInfo->reference = reference;
                delegateInfo->klass = klass;
                CacheDelegate(L,reference, delegate);
                xlua::GLogFormatted("create delegate %s lua_state %p refenence %d", delegate, delegateInfo->L, delegateInfo->reference);
                return delegate;
            }
        }else{
            //#TODO@benp throw error
        }
    }

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

    Il2CppObject* LuaValueToCSRef(Il2CppClass *klass, lua_State* L, int index)
    {
        if (klass == il2cpp_defaults.void_class) return nullptr;
        
        if (!klass)
        {
            klass = il2cpp_defaults.object_class;
        }        
        
        const Il2CppType *type = Class::GetType(klass);
        int t = type->type;
        
        PrimitiveValueType data;
        
        void* toBox = &data;
        
        Il2CppObject* ret = nullptr;
        
    handle_underlying:
        switch (t)
        {
            case IL2CPP_TYPE_I1:
            {
                data.i1 = (int8_t)lapi_lua_tonumber(L, index);
                break;
            }
            case IL2CPP_TYPE_BOOLEAN:
            {
                data.u1 = (uint8_t)lapi_lua_toboolean(L, index);
            }
            case IL2CPP_TYPE_U1:
            {
                data.u1 = (uint8_t)lapi_lua_tonumber(L, index);
                break;
            }
            case IL2CPP_TYPE_I2:
            {
                data.i2 = (int16_t)lapi_lua_tonumber(L, index);
                break;
            }
            case IL2CPP_TYPE_U2:
            {
                data.u2 = (uint16_t)lapi_lua_tonumber(L, index);
                break;
            }
            case IL2CPP_TYPE_CHAR:
            {
                data.c = (Il2CppChar)lapi_lua_tonumber(L, index);
                break;
            }
    #if IL2CPP_SIZEOF_VOID_P == 4
            case IL2CPP_TYPE_I:
    #endif
            case IL2CPP_TYPE_I4:
            {
                data.i4 = (int32_t)lapi_lua_tonumber(L, index);
                break;
            }
    #if IL2CPP_SIZEOF_VOID_P == 4
            case IL2CPP_TYPE_U:
    #endif
            case IL2CPP_TYPE_U4:
            {
                data.u4 = (uint32_t)lapi_lua_tonumber(L, index);
                break;
            }
    #if IL2CPP_SIZEOF_VOID_P == 8
            case IL2CPP_TYPE_I:
    #endif
            case IL2CPP_TYPE_I8:
            {
                data.i8 = (L, index);
                break;
            }
    #if IL2CPP_SIZEOF_VOID_P == 8
            case IL2CPP_TYPE_U:
    #endif
            case IL2CPP_TYPE_U8:
            {
                data.u8 = lapi_lua_toint64(L, index);
                break;
            }
            case IL2CPP_TYPE_R4:
            {
                data.r4 = (float)lapi_lua_tonumber(L, index);
                break;
            }
            case IL2CPP_TYPE_R8:
            {
                data.r8 = lapi_lua_tonumber(L, index);
                break;
            }
            case IL2CPP_TYPE_STRING:
            {
                auto str = lapi_lua_tostring(L, index);
                if (str)
                {
                    return (Il2CppObject*)il2cpp::vm::String::NewWrapper(str);
                }
                return nullptr;
            }
            case IL2CPP_TYPE_SZARRAY:
            case IL2CPP_TYPE_CLASS:
            case IL2CPP_TYPE_OBJECT:
            case IL2CPP_TYPE_ARRAY:
            case IL2CPP_TYPE_FNPTR:
            case IL2CPP_TYPE_PTR:
            {
                if (lapi_lua_isfunction(L, index))
                {
                    if (IsDelegate(klass))
                    {
                        return (Il2CppObject*)CreateDelegateFromLua(L, index, klass, true);
                    }
                    return nullptr;
                }
                
                auto ptr = GetCppObjMapper()->ToCppObj(L, index);;
                if (!ptr)
                {
                    // if ((klass == g_typeofArrayBuffer || klass == il2cpp_defaults.object_class) && pesapi_is_binary(env, jsval)) 
                    // {
                    //     RuntimeObject* ret = il2cpp::vm::Object::New(g_typeofArrayBuffer);

                    //     const MethodInfo* ctor = il2cpp_class_get_method_from_name(g_typeofArrayBuffer, ".ctor", 3);
                    //     typedef void (*NativeCtorPtr)(Il2CppObject* ___this, void*, int, int, const MethodInfo* method);
                        
                    //     void* data;
                    //     size_t length;
                    //     data = pesapi_get_value_binary(env, jsval, &length);
                    //     ((NativeCtorPtr)ctor->methodPointer)(ret, data, length, 0, ctor);   
                    //     return ret;
                    // }
                    // if ((klass == g_typeofPersistentObjectInfo || klass == il2cpp_defaults.object_class) && pesapi_is_object(env, jsval))
                    // {
                    //     Il2CppClass* persistentObjectInfoClass = g_typeofPersistentObjectInfo;
                        
                    //     RuntimeObject* ret = (RuntimeObject*)g_unityExports.GetRuntimeObjectFromPersistentObject(env, jsval);
                    //     if (ret == nullptr) 
                    //     {
                    //         ret = il2cpp::vm::Object::New(persistentObjectInfoClass);

                    //         const MethodInfo* ctor = il2cpp_class_get_method_from_name(persistentObjectInfoClass, ".ctor", 0);
                    //         typedef void (*NativeCtorPtr)(Il2CppObject* ___this, const MethodInfo* method);
                    //         ((NativeCtorPtr)ctor->methodPointer)(ret, ctor);
                            
                    //         PersistentObjectInfo* objectInfo = reinterpret_cast<PersistentObjectInfo*>(ret + 1);
                    //         g_unityExports.SetPersistentObject(env, jsval, objectInfo);
                    //         g_unityExports.SetRuntimeObjectToPersistentObject(env, jsval, ret);
                    //     }
                    //     return ret;
                    // }
                    if (klass == il2cpp_defaults.object_class)
                    {
                        if (lapi_lua_isstring(L, index))
                        {
                            t = IL2CPP_TYPE_STRING;
                            klass = il2cpp_defaults.string_class;
                        }
                        else if (lapi_lua_isnumber(L, index))
                        {
                            t = IL2CPP_TYPE_R8;
                            klass = il2cpp_defaults.double_class;
                        }
                        else if (lapi_lua_isinteger(L, index))
                        {
                            t = IL2CPP_TYPE_I4;
                            klass = il2cpp_defaults.int32_class;
                        }
                        else if (lapi_lua_isint64(L, index))
                        {
                            t = IL2CPP_TYPE_I8;
                            klass = il2cpp_defaults.int64_class;
                        }
                        else if (lapi_lua_isuint64(L, index))
                        {
                            t = IL2CPP_TYPE_U8;
                            klass = il2cpp_defaults.uint64_class;
                        }
                        else if (lapi_lua_isboolean(L, index))
                        {
                            t = IL2CPP_TYPE_BOOLEAN;
                            klass = il2cpp_defaults.boolean_class;
                        }
                        else
                        {
                            goto return_nothing;
                        }
                        goto handle_underlying;
                    }
                return_nothing:
                    return nullptr;
                }
                auto objClass = GetLuaObjCls(L, index);
                if (Class::IsAssignableFrom(klass, objClass))
                {
                    return Class::IsValuetype(objClass) ? Object::Box(objClass, ptr) : (Il2CppObject*)ptr;
                }
                return nullptr;
            }
            case IL2CPP_TYPE_VALUETYPE:
                /* note that 't' and 'type->type' can be different */
                if (type->type == IL2CPP_TYPE_VALUETYPE && Type::IsEnum(type))
                {
                    t = Class::GetEnumBaseType(Type::GetClass(type))->type;
                    goto handle_underlying;
                }
                else
                {
                    auto objClass = GetLuaObjCls(L, index);
                    if (!Class::IsAssignableFrom(klass, objClass))
                    {
                        return nullptr;
                    }
                    toBox = GetCSharpStructPointer(L, index, nullptr);
                    if (!toBox)
                    {
                        std::string message = "expect ValueType: ";
                        message += klass->name;
                        message += ", by got null";
                        Exception::Raise(Exception::GetInvalidOperationException(message.c_str()));
                        return nullptr;
                    }
                }
                break;
            case IL2CPP_TYPE_GENERICINST:
                t = GenericClass::GetTypeDefinition(type->data.generic_class)->byval_arg.type;
                goto handle_underlying;
            default:
                IL2CPP_ASSERT(0);
        }
        return Object::Box(klass, toBox);
    }

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
        DelegateWrapFunc WrapMethod;
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

    

    void* LuaStr2CSharpString(lua_State*L, int index){
        const char* str = lapi_lua_tostring(L, index);
        return String::NewWrapper(str);
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

    WrapData* AddProperty(LuaClassInfo* classInfo, Il2CppString* signatureIlstring, WrapFuncPtr WrapFunc, Il2CppString* nameIlstring, bool isStatic, bool isGetter, void* method, MethodPointer methodPointer, int typeInfoNum)
    {
          if (!WrapFunc) return nullptr;
        int allocSize = sizeof(WrapData) + sizeof(void*) * typeInfoNum;
        WrapData* data = (WrapData*)malloc(allocSize);
        memset(data, 0, allocSize);
        data->Method = static_cast<MethodType*>(method);
        data->MethodPointer = methodPointer;
        data->Wrap = WrapFunc;
        data->IsStatic = isStatic;
        data->IsExtensionMethod = false;
        std::string str = ilstring2stdstring(signatureIlstring);
        const char* signature = str.c_str();
        SetParamArrayFlagAndOptionalNum(data, signature);

        std::string nameStr = ilstring2stdstring(nameIlstring);
        const char* name = nameStr.c_str();

        for (int i = 0; i < classInfo->Properties.size(); ++i)
        {
            auto propertyInfo = &classInfo->Properties[i];
            if (propertyInfo->Name == name) {
                if (isGetter) {
                    propertyInfo->GetWrapData = data;
                }
                else {
                    propertyInfo->SetWrapData = data;
                }
                return data ;
            }
        }

        PropertyWrapData propertyInfo;
        propertyInfo.Name = name;
        propertyInfo.IsStatic = isStatic;
        propertyInfo.GetWrapData = nullptr;
        propertyInfo.SetWrapData = nullptr;
        if (isGetter) {
            propertyInfo.GetWrapData = data;
        }
        else {
            propertyInfo.SetWrapData = data;
        }
        classInfo->Properties.push_back(std::move(propertyInfo));

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



    void* GetCSharpStructPointerWithOffset(lua_State* L, int index, int offset, void* typeId) {
        auto css = lapi_xlua_tocss(L, index);
        if (css && css->fake_id == -1 && (typeId ? true : typeId == css->typeId)) {
            char* data = &css->data[0];
            data -= offset;
            return data;
        }
        return nullptr;
    }

    bool CheckIsStruct(lua_State* L, int index, void* typeId){
        auto css = lapi_xlua_tocss(L, index);

        return css && css->typeId == typeId;
    }

    bool CheckIsClass(lua_State* L, int index, void* typeId){
        if (lapi_lua_isuserdata(L, index)) {
            auto ptr = lapi_xlua_getcsobj_ptr(L, index);
            if(ptr){
                void* kclass = *reinterpret_cast<void**>(ptr);
                if(Class::IsAssignableFrom((Il2CppClass*)typeId, (Il2CppClass*)kclass)){
                    return true;
                }
            }
        }
        
        return false;
    }

    template <typename T>
    struct OptionalParameter
    {
        static T GetPrimitive(lua_State* L, int index, const void* methodInfo, WrapData* wrapData, int paramIndex)
        {
            if (lapi_lua_gettop(L) >= index)
            {
                return converter::Converter<T>::toCpp(L, index);
            }
            else
            {
                if (wrapData->IsExtensionMethod) ++paramIndex;
                auto pret = (T*)GetDefaultValuePtr((MethodInfo*)methodInfo, paramIndex);
                if (pret)
                {
                    return *pret;
                }
                return {};
            }

        }

        static T GetValueType(lua_State* L, int index, const void* methodInfo, WrapData* wrapData, int paramIndex)
        {
            if (lapi_lua_gettop(L) >= index)
            {
                return GetCSharpStructPointer(L, index);
            }
            else
            {
                if (wrapData->IsExtensionMethod) ++paramIndex;
                auto pret = (T*)GetDefaultValuePtr((MethodInfo*)methodInfo, paramIndex);
                if (pret) 
                {
                    return *pret;
                }
                T ret;
                memset(&ret, 0, sizeof(T));
                return ret;
            }
        }
        
        static void* GetString(lua_State* L, int index, const void* methodInfo, WrapData* wrapData, int paramIndex)
        {
            if (lapi_lua_gettop(L) >= index)
            {
                return LuaStr2CSharpString(L, index);
            }
            else
            {
                if (wrapData->IsExtensionMethod) ++paramIndex;
                return xlua::GetDefaultValuePtr((MethodInfo*)methodInfo, paramIndex);
            }
        }
        
        static void* GetRefType(lua_State* L, int index, const void* methodInfo, WrapData* wrapData, int paramIndex, const void* typeId)
        {
            if (lapi_lua_gettop(L) >= index)
            {
                return LuaValueToCSRef(L, typeId, index);
            }
            else
            {
                if (wrapData->IsExtensionMethod) ++index;
                return GetDefaultValuePtr((MethodInfo*)methodInfo, index);
            }
        }
    };

    #include "FunctionBridge.Gen.h"



    
    BridgeFuncInfo* FindBridgeFunc(Il2CppString * ilstring)
    {
        std::string str = ilstring2stdstring(ilstring);
        const char* signature = str.c_str();
        // xlua::GLogFormatted("FindBridgeFunc ilstring %s",signature);
        auto begin = &g_bridgeFuncInfos[0];
        auto end = &g_bridgeFuncInfos[sizeof(g_bridgeFuncInfos) / sizeof(BridgeFuncInfo) - 1];
        auto first = std::lower_bound(begin, end, signature, [](const BridgeFuncInfo& x, const char* signature) {return strcmp(x.Signature, signature) < 0;});
        if (first != end && strcmp(first->Signature, signature) == 0) {
            return first;
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
            // xlua::GLogFormatted("FindWrapFunc %s",signature);
            return first->Method;
        }
        return nullptr;
    }

    FieldWrapFuncInfo * FindFieldWrapFuncInfo(Il2CppString * ilstring)
    {
        std::string str = ilstring2stdstring(ilstring);
        const char* signature = str.c_str();
        // xlua::GLogFormatted("FindFieldWrapFuncInfo %s",signature);
        auto begin = &g_fieldWrapFuncInfos[0];
        auto end = &g_fieldWrapFuncInfos[sizeof(g_fieldWrapFuncInfos) / sizeof(FieldWrapFuncInfo) - 1];
        auto first = std::lower_bound(begin, end, signature, [](const FieldWrapFuncInfo& x, const char* signature) {return strcmp(x.Signature, signature) < 0;});
        if (first != end && strcmp(first->Signature, signature) == 0) {
            return first;
        }
        
        return nullptr;
    }
    LuaClassInfo* CreateCSharpTypeInfo(Il2CppString* name, const void* type_id, const void* super_type_id, void* klass, bool isValueType, bool isDelegate, Il2CppString* delegateSignature)
    {
        const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(name);
        std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
         xlua::GLogFormatted("CreateCSharpTypeInfo %s", str.c_str());
        BridgeFuncInfo* bridgeFuncInfo = nullptr ;
        if (isDelegate)
        {
            bridgeFuncInfo = FindBridgeFunc(delegateSignature);
            if (!bridgeFuncInfo) return nullptr;
        }
        LuaClassInfo* ret = new LuaClassInfo();
        ret->Name = str;
        ret->TypeId = (Il2CppClass*)type_id;
        ret->SuperTypeId = super_type_id;
        ret->Class = static_cast<TypeIdType*>(klass);
        ret->IsValueType = isValueType;
        
        if(bridgeFuncInfo){
            ret->DelegateBridge = bridgeFuncInfo->Method;
            ret->DelegateWrapBridge = bridgeFuncInfo->WrapMethod;
        }
        return ret;
    }
    
    static xlua::LuaClassInfo* GetLuaClsInfo(lua_State* L) {
        if (lapi_lua_isuserdata(L, 1)) {
            void* ptr = lapi_xlua_getcsobj_ptr(L, 1);
            if (ptr) {
                
                void* kclass = *reinterpret_cast<void**>(ptr);
                auto clsInfo = xlua::GetLuaClsInfoByTypeId(kclass);
                // xlua:GLogFormatted("GetLuaClsInfo ptr %p kclass %p clsInfo %p", ptr, kclass, clsInfo);
                return clsInfo;
            }

            CSharpStruct* css = lapi_xlua_tocss(L, 1);
            if(css){
                void * kclass = css->typeId;
                auto clsInfo = xlua::GetLuaClsInfoByTypeId(kclass);
                //xlua:GLogFormatted("GetLuaClsInfo css %p kclass %p clsInfo %p", css, kclass, clsInfo);
                return clsInfo;
            }
        }
        return nullptr;
    }

    static xlua::LuaClassInfo* GetLuaClsInfoByClsMeta(lua_State* L) {
        if (lapi_lua_isuserdata(L, lapi_lapi_lua_upvalueindex(1))) {
            const void* ptr = lapi_lua_topointer(L, lapi_lapi_lua_upvalueindex(1));
            xlua::LuaClassInfo* clsInfo = xlua::GetLuaClsInfoByTypeId(ptr);
            // xlua:GLogFormatted("GetLuaClsInfoByClsMeta ptr %p clsInfo %p", ptr, clsInfo);
            return clsInfo;
        }
        return nullptr;
    }

    //class的set回调
    int ClsSetCallBack(lua_State *L){
        auto clsInfo = GetLuaClsInfoByClsMeta(L);

        while(clsInfo){
             if (lapi_lua_isstring(L, 2)) {
                const char* key = lapi_lua_tostring(L, 2); 

                auto propertyIter = clsInfo->PropertyMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换
                if (propertyIter != clsInfo->PropertyMap.end()) {
                    auto propertyInfo = propertyIter->second;
                    if (propertyInfo->IsStatic && propertyInfo->SetWrapData) {
                        // -1 cls -2 key -3 param
                        lapi_lua_replace(L, 2);
                        // -1 cls -2 param
                        return xlua::PropertyCallback(L, propertyInfo->SetWrapData, 2);
                    }
                }

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

             if(clsInfo->SuperTypeId){
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass*)clsInfo->SuperTypeId, L);
            }else{
                clsInfo = nullptr;
            }
        }
        return 0;
    }

    // luaStack 1 c#obj 2 param1 3param2
    int MethodCallbackLuaWrap(lua_State* L, int paramOffset = 2) {
        
        if (lapi_lua_isuserdata(L, lapi_lapi_lua_upvalueindex(1))) {
            const void* pointer = lapi_lua_topointer(L, lapi_lapi_lua_upvalueindex(1));
            void* p1 = const_cast<void*>(pointer);
            xlua::WrapData** wrapDatas = reinterpret_cast<xlua::WrapData**>(p1);
            if (wrapDatas) {
                return xlua::MethodCallback(L, wrapDatas, paramOffset);
            }
        }

        return 0;
    }

    //class的get回调
    int ClsGetCallBack(lua_State *L){
        auto clsInfo = GetLuaClsInfoByClsMeta(L);
        
        while(clsInfo){
            //#TODO@benp 这里可以考虑初始化静态变量
            //method
            //todo 考虑不使用closure 优势1 减少闭包的内存 2 减少对Lua堆栈的操作 后续测试确定
            // todo 这里先注释  里面有脏东西
            //if (!lapi_lua_isnil(L, lapi_lapi_lua_upvalueindex(2))) {
            //    lapi_lua_pushvalue(L, 2);
            //    lapi_lua_gettable(L, lapi_lapi_lua_upvalueindex(2));
            //    if (!lapi_lua_isnil(L, -1)) {//has method
            //        return 1;
            //    }
            //    lapi_lua_pop(L, 1);
            //}
            
            //find method or field
            if (lapi_lua_isstring(L, 2)) {
                
                const char* key = lapi_lua_tostring(L, 2); //#TODO@benp 这里会有char* => string的隐式转换
                if (strcmp(key, "UnderlyingSystemType") == 0) {
                    auto reflectType = TypeIdToType(clsInfo->Class);
                    GetCppObjMapper()->TryPushObject(L, (void*)reflectType);
                    return 1;
                }

                auto iter = clsInfo->MethodsMap.find(key);
                if(iter != clsInfo->MethodsMap.end())
                {
                    // find method 
                    auto method = iter->second;
                    if(method->IsStatic){
                        xlua::WrapData** p = method->OverloadDatas.data();
                        lapi_lua_pushlightuserdata(L, p);
                        // lapi_lua_pushcclosure(L, MethodCallbackLuaWrap, 1);
                        lapi_lua_pushcclosure(L, [](lua_State* L){return MethodCallbackLuaWrap(L, 1);}, 1);
                        //cache method
                        lapi_lua_pushvalue(L, lapi_lapi_lua_upvalueindex(2)); //-1 table   -2closure 
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

                auto propertyIter = clsInfo->PropertyMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换
                if (propertyIter != clsInfo->PropertyMap.end()) {
                    auto propertyInfo = propertyIter->second;
                    if (propertyInfo->IsStatic && propertyInfo->GetWrapData) {
                        lapi_lua_pop(L, 1);
                        return xlua::PropertyCallback(L, propertyInfo->GetWrapData, 2);
                    }
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
            
            if (clsInfo->SuperTypeId) {
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass*)clsInfo->SuperTypeId, L);
            }
            else {
                clsInfo = nullptr;
            }
        }
        return 0;
    }

    int ClsConstructorCallBack(lua_State *L){
        auto clsInfo = GetLuaClsInfoByClsMeta(L);
        
        if(clsInfo){
            auto isValueType = Class::IsValuetype(clsInfo->Class);

            if(!isValueType){
                void* ptr = xlua::ObjectAllocate(clsInfo->Class);
                //-1 clsTable -2 param -top obj
                if (GetCppObjMapper()->TryPushObject(L, ptr)) {
                    //-1 obj -2 param
                    lapi_lua_replace(L, 1);
                    
                    
                    if (clsInfo->CtorWrapDatas) {
                        //#TODO@benp 构造函数失败  清除引用
                        if (MethodCallback(L, clsInfo->CtorWrapDatas, 2) >= 0) {
                            lapi_lua_settop(L, 1);
                            return 1;
                        }
                        else {
                            
                            return 0;
                        }
                    }
                }
                else {
                    //todo throw error
                    throw_exception2lua(L, "push valu to lua failed");
                    return 0;
                }
            }else{
                 auto klass = clsInfo->TypeId;
                 //auto size = Class::GetValueSize,((Il2CppClass*)klass, NULL);
                 auto size = klass->native_size > 0 ? klass->native_size : (klass->instance_size - sizeof(Il2CppObject));
                //auto size = klass->instance_size;
                int32_t typeId = GetLuaClassRegister()->GetTypeIdByIl2cppClass(L, klass);
                CSharpStruct* css = lapi_xlua_createstruct_pointer(L, size, typeId, (void*)klass);
                
                if (clsInfo->CtorWrapDatas) {
                    //-1 obj -2 param
                    lapi_lua_replace(L, 1);
                    if (MethodCallback(L, clsInfo->CtorWrapDatas, 2) >= 0) {
                        lapi_lua_settop(L, 1);
                        return 1;
                    }
                    else {
                        return 0;
                    }
                }
            }
            return 1;
        }
        return 0;
    }



    int FieldCallbackLuaWrap(lua_State *L){
        if(lapi_lua_isuserdata(L, lapi_lapi_lua_upvalueindex(1))){
            xlua::FieldWrapData* wrapData = (xlua::FieldWrapData*) (lapi_lua_topointer(L, lapi_lapi_lua_upvalueindex(1)));
            if(wrapData){
                wrapData->Getter(L, ( FieldInfo*)wrapData->FieldInfo, wrapData->Offset, (Il2CppClass*)wrapData->TypeInfo);
                return 1;
            }
        }
        return 0;
    }

    int FieldGetCallbackLuaWrap(lua_State* L) {
        if (lapi_lua_isuserdata(L, lapi_lapi_lua_upvalueindex(1))) {
            xlua::FieldWrapData* wrapData = (xlua::FieldWrapData*)(lapi_lua_topointer(L, lapi_lapi_lua_upvalueindex(1)));
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
        // xlua::GLogFormatted("ObjSetCallBack");
        auto clsInfo = xlua::GetLuaClsInfo(L);
        while (clsInfo) {
            //find method or field
            if (lapi_lua_isstring(L, 2)) {

                const char* key = lapi_lua_tostring(L, 2); //#TODO@benp 这里会有char* => string的隐式转换

                auto propertyInfoIter = clsInfo->PropertyMap.find(key);
                if (propertyInfoIter != clsInfo->PropertyMap.end()) {
                    auto propertyInfo = propertyInfoIter->second;
                    if (!propertyInfo->IsStatic && propertyInfo->SetWrapData) {
                        //-1 obj -2 key -3 param
                        lapi_lua_replace(L, 2);
                        //-1 obj -2 param
                        xlua::PropertyCallback(L, propertyInfo->SetWrapData, 2);

                        return 0;
                    }
                }

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

            if (lapi_lua_isnumber(L, 2)) {
                auto propertyIter = clsInfo->PropertyMap.find("Item");//#TODO@benp 这里会有char* => string的隐式转换
                if (propertyIter != clsInfo->PropertyMap.end())
                {
                    // find method 
                    auto propertyInfo = propertyIter->second;
                    if (!propertyInfo->IsStatic && propertyInfo->SetWrapData) {
                        PropertyCallback(L, propertyInfo->SetWrapData, 2);
                        return 0;
                    }
                }
            }

            if(clsInfo->SuperTypeId){
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass*)clsInfo->SuperTypeId, L);
            }else{
                clsInfo = nullptr;
            }
        }

        throw_exception2lua(L, "obj invalid for ObjSetCallBack");
        return 0;
    }


    ///Lua CSharp对象的getter函数
    /// 1 getFromCache 1 method 2 getter
    /// 2 cspointer=>kclass=>luaDef
    /// 3 luaDef=> method or field
    /// 4 push result
    int ObjGetCallBack(lua_State *L){
        //#TODO@benp 校验 可以通过xluatag or getDef的形式
        //method
        //todo 考虑不使用closure 优势1 减少闭包的内存 2 减少对Lua堆栈的操作 后续测试确定
        // xlua::GLogFormatted("ObjGetCallBack");
        if (!lapi_lua_isnil(L, lapi_lapi_lua_upvalueindex(2))) {
            lapi_lua_pushvalue(L, 2);
            lapi_lua_gettable(L, lapi_lapi_lua_upvalueindex(2));
            if (!lapi_lua_isnil(L, -1)) {//has method
                return 1;
            }
            lapi_lua_pop(L, 1);
        }

        auto clsInfo = GetLuaClsInfo(L);
        while (clsInfo) {

            //find method or field
            if (lapi_lua_isstring(L, 2)) {
                
                const char* key = lapi_lua_tostring(L, 2); 

                auto iter = clsInfo->MethodsMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换
                if(iter != clsInfo->MethodsMap.end())
                {
                    // find method 
                    auto method = iter->second;
                    
                    if(!method->IsSetter && !method->IsGetter && !method->IsStatic){
                        xlua::WrapData** p = method->OverloadDatas.data();
                        lapi_lua_pushlightuserdata(L, p);
                        lapi_lua_pushcclosure(L, [](lua_State* L){return MethodCallbackLuaWrap(L, 2);}, 1);
                        //cache method
                        lapi_lua_pushvalue(L, lapi_lapi_lua_upvalueindex(2)); //-1 table   -2closure 
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

                auto propertyInfoIter = clsInfo->PropertyMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换

                if (propertyInfoIter != clsInfo->PropertyMap.end() )
                {
                    // find method  
                    auto propertyInfo = propertyInfoIter->second;
                    if (!propertyInfo->IsStatic && propertyInfo->GetWrapData) {
                        // -1 obj -2 key
                        lapi_lua_pop(L, 1);
                        // -1 obj
                        return xlua::PropertyCallback(L, propertyInfo->GetWrapData, 2);
                    }
                }


                auto fieldIter = clsInfo->FieldMap.find(key);//#TODO@benp 这里会有char* => string的隐式转换
                if(fieldIter != clsInfo->FieldMap.end())
                {
                    // find method  
                    auto field = fieldIter->second;
                    if(!field->IsStatic && field->Data->Getter){
                        xlua::GLogFormatted("get field %s", field->Name);
                        field->Data->Getter(L, (FieldInfo*)field->Data->FieldInfo, field->Data->Offset, (Il2CppClass*)field->Data->TypeInfo);
                        return 1;
                    }
                    //找不到就退出不允许重复的key
                    return 0;
                }
            }

            if (lapi_lua_isnumber(L, 2)) {
                auto propertyIter = clsInfo->PropertyMap.find("Item");//#TODO@benp 这里会有char* => string的隐式转换
                if (propertyIter != clsInfo->PropertyMap.end())
                {
                    // find method 
                    auto propertyInfo = propertyIter->second;
                    if (!propertyInfo->IsStatic && propertyInfo->GetWrapData) {
                        PropertyCallback(L, propertyInfo->GetWrapData, 2);
                        return 1;
                    }
                }
            }
            if(clsInfo->SuperTypeId){
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass*)clsInfo->SuperTypeId, L);
            }else{
                clsInfo = nullptr;
            }
        }
        return 0;
    }

    void SetCSharpAPI(Il2CppArray* methodArray){
        
        auto getTypeIdMethod = il2cpp_array_addr(methodArray,  Il2CppReflectionMethod*, 0);
        auto methodPointer = xlua::GetMethodPointer(*getTypeIdMethod);
        auto methodInfo = xlua::GetMethodInfoPointer(*getTypeIdMethod);
        xlua::GetLuaClassRegister()->SetGetTypeIdFuncPtr((CSharpGetTypeIdFunc)methodPointer, (void*)methodInfo);
        
        auto addObjMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod*, 1);
        auto addObjMethodPointer = xlua::GetMethodPointer(*addObjMethod);
        auto addObjMethodInfo = xlua::GetMethodInfoPointer(*addObjMethod);

        auto removeObjMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod*, 2);
        auto removeObjMethodPointer = xlua::GetMethodPointer(*removeObjMethod);
        auto removeObjMethodInfo = xlua::GetMethodInfoPointer(*removeObjMethod);
        GetCppObjMapper()->InitObjPoolMethod((Il2CppReflectionMethod*)addObjMethod, (Il2CppMethodPointer*)addObjMethodPointer, (Il2CppReflectionMethod*)removeObjMethod, (Il2CppMethodPointer*)removeObjMethodPointer);
        
        auto delegateCombineMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod*, 3);
        DelegateCombineMethodPointer = (DelegateCombineType)xlua::GetMethodPointer(*delegateCombineMethod);
        DelegateCombineMethodInfo = (*delegateCombineMethod)->method;

        auto delegateRemoveMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod*, 4);
        DelegateRemoveMethodPointer = (DelegateRemoveType)xlua::GetMethodPointer(*delegateRemoveMethod);
        DelegateRemoveMethodInfo = (*delegateRemoveMethod)->method;

        auto getCacheDelegateMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod*, 5);
        GetCacheDelegateMethodPointer = (GetCacheDelegateType)xlua::GetMethodPointer(*getCacheDelegateMethod);
        GetCacheDelegateMethodInfo = (*getCacheDelegateMethod)->method;

        auto CacheDelegateMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod*, 6);
        CacheDelegateMethodPointer = (CacheDelegateType)xlua::GetMethodPointer(*CacheDelegateMethod);
        CacheDelegateMethodInfo = (*CacheDelegateMethod)->method;

    }

    int LuaGCCallBack(lua_State *L){
         if(lapi_lua_isuserdata(L, lapi_lapi_lua_upvalueindex(1))){
            Il2CppClass* klass = static_cast<Il2CppClass*>(lapi_lua_touserdata(L, lapi_lapi_lua_upvalueindex(1)));
            if(!klass->valuetype){
                CSharpObject* obj = (CSharpObject*)lapi_lua_touserdata(L, 1);
                if(obj && obj->poolIdx != -1){
                    auto ptr = GetCppObjMapper()->RemoveFromPool(L, obj->poolIdx);
                    xlua::GLogFormatted("RemoveFromPool idx[%d] return %p", obj->poolIdx, ptr);
                    obj->poolIdx = -1;
                }
            }
         }
        return 0;
    }

    
    /// @brief 释放C#持有的lua引用
    void ReleaseLuaRef(RuntimeObject* obj){
        PersistentObjectInfo* persistentObj = (PersistentObjectInfo*)Object::Unbox(obj);
        if(persistentObj->reference != -1){
            auto L = persistentObj->L;
            lapi_lua_pushnil(L);
            lapi_xlua_rawseti(L, lapi_xlua_get_registry_index(), persistentObj->reference);
            persistentObj->L = nullptr;
            persistentObj->reference = -1;
        }
    }

    xlua::UnityExports* GetUnityExports()
    {
        //void* a = OptionalParameter<void*>::GetString(nullptr, 0, nullptr, nullptr, 0);
        //int32_t i32 = converter::Converter<int32_t>::toCpp(nullptr, 1);
        g_unityExports.ObjectAllocate = &ObjectAllocate;
        //g_unityExports.DelegateAllocate = &DelegateAllocate; 
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
        //g_unityExports.IsDelegate = &IsDelegate;
        g_unityExports.IsAssignableFrom = &Class::IsAssignableFrom;
        g_unityExports.CSharpTypeToTypeId = &CSharpTypeToTypeId;
        g_unityExports.CStringToCSharpString = &String::NewWrapper;
        g_unityExports.GetTID = &GetTID;
        g_unityExports.ThrowInvalidOperationException = &ThrowInvalidOperationException;
        g_unityExports.GetReturnType = &GetReturnType;
        // g_unityExports.GetParameterType = &GetParameterType;
        g_unityExports.NewArray = NewArray;
        g_unityExports.GetArrayFirstElementAddress = Array::GetFirstElementAddress;
        g_unityExports.ArraySetRef = ArraySetRef;
        //g_unityExports.GetArrayElementTypeId = Class::GetElementClass;
        g_unityExports.GetArrayLength = Array::GetLength;
        g_unityExports.GetDefaultValuePtr = GetDefaultValuePtr;
        g_unityExports.SizeOfRuntimeObject = sizeof(RuntimeObject);
        return &g_unityExports;
    }
}

// namespace end

#ifdef __cplusplus
extern "C" {
#endif


int InitXluaIl2Cpp(lua_State* L){
    
    return 0;
}
void SetXLuaRef(Il2CppArray* refArr){
    int32_t* cache_ref = il2cpp_array_addr(refArr, int32_t, 0);
    GetCppObjMapper()->SetCacheRef(*cache_ref);
    int32_t* num2 = il2cpp_array_addr(refArr, int32_t, 1);
    xlua::errorFunc_ref = *num2;
    xlua::GLogFormatted("xlua::errorFunc_ref %d", xlua::errorFunc_ref);
    
}

void InitialXLua_IL2CPP(lapi_func_ptr* func_array, lua_State* L)
{
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetMethodPointer(System.Reflection.MethodBase)", (Il2CppMethodPointer)xlua::GetMethodPointer);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetMethodInfoPointer(System.Reflection.MethodBase)", (Il2CppMethodPointer)xlua::GetMethodInfoPointer);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::SetCSharpAPI(System.Reflection.MethodBase[])", (Il2CppMethodPointer)xlua::SetCSharpAPI);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetObjectPointer(System.Object)", (Il2CppMethodPointer)xlua::GetObjectPointer);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetTypeId(System.Type)", (Il2CppMethodPointer)xlua::GetTypeId);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::TypeIdToType(System.IntPtr)", (Il2CppMethodPointer)xlua::TypeIdToType);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetFieldOffset(System.Reflection.FieldInfo,System.Boolean)", (Il2CppMethodPointer)xlua::GetFieldOffset);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetFieldInfoPointer(System.Reflection.FieldInfo)", (Il2CppMethodPointer)xlua::GetFieldInfoPointer);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::CreateCSharpTypeInfo(System.String,System.IntPtr,System.IntPtr,System.IntPtr,System.Boolean,System.Boolean,System.String)", (Il2CppMethodPointer)xlua::CreateCSharpTypeInfo);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::SetTypeInfo(System.IntPtr,System.Int32,System.IntPtr)", (Il2CppMethodPointer)xlua::SetTypeInfo);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::FindWrapFunc(System.String)", (Il2CppMethodPointer)xlua::FindWrapFunc);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::FindFieldWrap(System.String)", (Il2CppMethodPointer)xlua::FindFieldWrapFuncInfo);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::AddConstructor(System.IntPtr,System.String,System.IntPtr,System.IntPtr,System.IntPtr,System.Int32)", (Il2CppMethodPointer)xlua::AddConstructor);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::AddField(System.IntPtr,System.IntPtr,System.String,System.Boolean,System.IntPtr,System.Int32,System.IntPtr)", (Il2CppMethodPointer)xlua::AddField);

    InternalCalls::Add("XLua.IL2CPP.NativeAPI::AddMethod(System.IntPtr,System.String,System.IntPtr,System.String,System.Boolean,System.Boolean,System.Boolean,System.Boolean,System.IntPtr,System.IntPtr,System.Int32)", (Il2CppMethodPointer)xlua::AddMethod);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::AddProperty(System.IntPtr,System.String,System.IntPtr,System.String,System.Boolean,System.Boolean,System.IntPtr,System.IntPtr,System.Int32)", (Il2CppMethodPointer)xlua::AddProperty);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::AddProperty(System.IntPtr,System.String,System.IntPtr,System.String,System.Boolean,System.Boolean,System.IntPtr,System.IntPtr,System.Int32)", (Il2CppMethodPointer)xlua::AddProperty);
    InternalCalls::Add("XLua.IL2CPP.LuaObject::ReleaseLuaRef()", (Il2CppMethodPointer)xlua::ReleaseLuaRef);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::SetGlobalType_LuaObject(System.Type)", (Il2CppMethodPointer)xlua::SetGlobalType_LuaObject);
    InternalCalls::Add("XLua.IL2CPP.NativeAPI::SetXLuaRef(System.Int32[])", (Il2CppMethodPointer)SetXLuaRef);
    
    lapi_init(func_array);
    
}

void SetLogCallback(xlua::LogCallback log)
{
    xlua::SetLogHandler(log);
}

int DelegateCallBack(lua_State* L) {
    void* ptr = lapi_xlua_getcsobj_ptr(L, 1);
    if (ptr) {
        Il2CppDelegate* obj = reinterpret_cast<Il2CppDelegate*>(ptr);
        xlua::GLogFormatted("DelegateCallBack ptr %p", ptr);
        auto clsInfo = xlua::GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId(obj->object.klass, L);
        if (clsInfo) {
            return clsInfo->DelegateWrapBridge(L, obj, (const MethodInfo*) obj->method);
        }
        else {
            return xlua::throw_exception2lua(L, "obj is not delegate obj");
        }
        return 0;
    }
    else {
        return xlua::throw_exception2lua(L, "obj is not cpp obj");
    }
}

int DelegateCombineCallBack(lua_State* L){
    Il2CppClass* klass = nullptr;
    if (lapi_lua_isuserdata(L, 1)) {
        auto ptr = lapi_xlua_getcsobj_ptr(L, 1);
        if (ptr) {
            
            klass = (Il2CppClass*)*reinterpret_cast<void**>(ptr);
        }
    }
    else if (lapi_lua_isuserdata(L, 2)) {
        auto ptr = lapi_xlua_getcsobj_ptr(L, 2);
        if (ptr) {
            klass = (Il2CppClass*)*reinterpret_cast<void**>(ptr);
        }
    }
    if (!klass) {
        xlua::throw_exception2lua(L, "params wrong for delegate combine");
        return 0;
    }
    void* delegate1 = xlua::LuaValueToCSRef(klass, L, 1);
    void* delegate2 = xlua::LuaValueToCSRef(klass, L, 2);
    if(delegate1 && delegate2){
        auto ptr = xlua::DelegateCombineMethodPointer((Il2CppDelegate*)delegate1, (Il2CppDelegate*)delegate2, (MethodInfo*)xlua::DelegateCombineMethodInfo);
        GetCppObjMapper()->TryPushObject(L, ptr);
        xlua::GLogFormatted("DelegateCombine push to lua %p", ptr);
        return 1;
    }else{
        return xlua::throw_exception2lua(L, "params wrong for delegate combine");
    }
}

int DelegateRemoveCallBack(lua_State* L){
    void* delegate1 = xlua::LuaValueToCSRef(il2cpp_defaults.delegate_class, L, 1);
    if (!delegate1) {
        return xlua::throw_exception2lua(L, "#1 parameter must be a delegate");
    }
    Il2CppClass* klass = ((Il2CppObject*)delegate1)->klass;
    if (!klass) {
        return xlua::throw_exception2lua(L, "#1 parameter must be a delegate error 2");
    }
    auto delegate2 = xlua::LuaValueToCSRef(klass, L, 2);
    if (!delegate2) {
        return xlua::throw_exception2lua(L, "#2 parameter must be a delegate or a function ");
    }
    auto ptr = xlua::DelegateRemoveMethodPointer((Il2CppDelegate*)delegate1, (Il2CppDelegate*)delegate2, (MethodInfo*)xlua::DelegateRemoveMethodInfo);
    GetCppObjMapper()->TryPushObject(L, ptr);
    return 1;
}

void HandleDeleagateMetatable(lua_State* L, int metatable_idx, Il2CppClass* typeId) {
    lapi_lua_pushstring(L, "__call");
    lapi_lua_pushcclosure(L, DelegateCallBack, 0);
    lapi_lua_settable(L, metatable_idx);
    lapi_lua_pushstring(L, "__gc");
    lapi_lua_pushlightuserdata(L, typeId);
    lapi_lua_pushcclosure(L, xlua::LuaGCCallBack, 1);
    lapi_lua_settable(L, metatable_idx);
    lapi_lua_pushstring(L, "__add");
    lapi_lua_pushcclosure(L, DelegateCombineCallBack, 0);
    lapi_lua_settable(L, metatable_idx);
    lapi_lua_pushstring(L, "__sub");
    lapi_lua_pushcclosure(L, DelegateRemoveCallBack, 0);
    lapi_lua_settable(L, metatable_idx);
    lapi_lua_pop(L, 1);
}
static std::map<const char*, const char*, cmp_str> supportOp = {
    { "op_Addition", "__add" },
    { "op_Subtraction", "__sub" },
    { "op_Multiply", "__mul" },
    { "op_Division", "__div" },
    { "op_Equality", "__eq" },
    { "op_UnaryNegation", "__unm" },
    { "op_LessThan", "__lt" },
    { "op_LessThanOrEqual", "__le" },
    { "op_Modulus", "__mod" },
    { "op_BitwiseAnd", "__band" },
    { "op_BitwiseOr", "__bor" },
    { "op_ExclusiveOr", "__bxor" },
    { "op_OnesComplement", "__bnot" },
    { "op_LeftShift", "__shl" },
    { "op_RightShift", "__shr" },
};
void InitObjOperation(lua_State* L, int objMetaIdx, Il2CppClass* klass) {
    auto clsInfo = xlua::GetLuaClassRegister()->GetLuaClsInfoByTypeId((void*)klass);
    if (clsInfo) {
        for (auto& method : clsInfo->Methods) {
            auto iter = supportOp.find(method.Name.c_str());

            if (iter != supportOp.end()) {
                auto key = iter->second;
                lapi_lua_pushstring(L, key);
                xlua::WrapData** p = method.OverloadDatas.data();
                lapi_lua_pushlightuserdata(L, p);
                lapi_lua_pushcclosure(L, [](lua_State* L) {return xlua::MethodCallbackLuaWrap(L, 1); }, 1);
                lapi_lua_rawset(L, objMetaIdx);
                xlua::GLogFormatted("InitObjOperation class[%s]  op[%s]", klass->name, iter->second);
            }
        }
    }
}

void HandleObjMetatable(lua_State *L, int metatable_idx, Il2CppClass* typeId){
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

    if(!typeId->valuetype){ //只有引用类型的需要GC
        lapi_lua_pushstring(L, "__gc");
        lapi_lua_pushlightuserdata(L, typeId);
        lapi_lua_pushcclosure(L, xlua::LuaGCCallBack, 1);
        lapi_lua_settable(L, metatable_idx);
    }
    InitObjOperation(L, metatable_idx, typeId);
}

void HandleClsMetaTable(lua_State *L, int clstable_idx, Il2CppClass* typeId){
    //#TODO@benp 采用非闭包
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

    lapi_lua_pushstring(L, "UnderlyingSystemType");
    lapi_lua_pushlightuserdata(L, typeId);
    lapi_lua_settable(L, clstable_idx);
}


int RegisterLuaClass(xlua::LuaClassInfo *luaClsInfo, lua_State* L, const char* fullName){
    return xlua::GetLuaClassRegister()->RegisterClass(luaClsInfo);
}


void UnRegisterLuaClass(Il2CppString* ilstring){
    const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(ilstring);
    auto name = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
    xlua::GetLuaClassRegister()->UnRegisterClass(name);
}




void SetClassMetaId(void * ilclass, int metaId){
    xlua::GetLuaClassRegister()->SetTypeId(ilclass, metaId);
    // GetCppObjMapper()->SetTypeId(ilclass, metaId);
}




#ifdef __cplusplus
}
#endif