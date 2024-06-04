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
#include "xlua_il2cpp_def.h"
#include "LuaClassRegister.h"
#include "CppObjMapper.h"
#include "Converter.cpp"
#include <mutex>
#include <vm-utils/NativeDelegateMethodCache.h>
#include "Il2CppTools.h"
#include <string>
static_assert(IL2CPP_GC_BOEHM, "Only BOEHM GC supported!");

using namespace il2cpp::vm;

namespace xlua
{
    static int32_t errorFunc_ref = -1;
    static DelegateCombineType DelegateCombineMethodPointer = nullptr;
    static const MethodInfo *DelegateCombineMethodInfo = nullptr;

    static DelegateRemoveType DelegateRemoveMethodPointer = nullptr;
    static const MethodInfo *DelegateRemoveMethodInfo = nullptr;

    static Bytes2StringType Bytes2StringMethodPointer = nullptr;
    static const MethodInfo *Bytes2StringMethodInfo = nullptr;

    static FallBackLua2CSObjType FallBackLua2CSObjMethodPointer = nullptr;
    static const MethodInfo *FallBackLua2CSObjMethodInfo = nullptr;

    static GetDelegateType GetDelegateMethodPointer = nullptr;
    static const MethodInfo *GetDelegateMethodInfo = nullptr;

    static Il2CppClass *g_typeofDelegateMiddleware;

    Il2CppObject *LuaValueToCSRef(Il2CppClass *klass, lua_State *L, int index);
    Il2CppArray *TranslateArray(lua_State *L, int index, Il2CppClass *klass);

    Il2CppDelegate *GetDelegate(lua_State *L, int index, Il2CppClass *klass, intptr_t funcptr)
    {
        Il2CppReflectionType *reflectType = Reflection::GetTypeObject(Class::GetType(klass));
        return GetDelegateMethodPointer(L, index, reflectType, funcptr, GetDelegateMethodInfo);
    }

    Il2CppObject *FallBackLua2CSObj(lua_State *L, int index, Il2CppClass *klass)
    {
        auto reflectType = Reflection::GetTypeObject(Class::GetType(klass));
        if (reflectType)
        {
            return FallBackLua2CSObjMethodPointer(L, index, reflectType, FallBackLua2CSObjMethodInfo);
        }

        return nullptr;
    }

    // 获取引用类型对象的指针
    Il2CppObject *GetRawObjPointer(lua_State *L, int index, void *klass)
    {
        auto obj = GetCppObjMapper()->GetCSharpObj(L, index);
        if (obj)
        {
            if (Class::IsAssignableFrom((Il2CppClass *)klass, obj->klass))
            {
                return obj;
            }
            else
            {
                throw_exception2lua_format(L, "expect type[%s] not [%s]", ((Il2CppClass *)klass)->name, obj->klass->name);
            }
        }
        return nullptr;
    }

    // 获取值类型对象的指针
    void *GetRawValuePointer(lua_State *L, int index, void *klass)
    {
        return GetCppObjMapper()->ToStruct(L, index, (Il2CppClass *)klass);
    }

    // 获取值类型对象的指针 左移一个runtimeObject
    void *GetRawValuePointer_Offset(lua_State *L, int index, void *klass)
    {
        void *ptr = GetCppObjMapper()->ToStruct(L, index, (Il2CppClass *)klass);
        if (ptr)
        {
            char *data = reinterpret_cast<char *>(ptr);
            data -= sizeof(RuntimeObject);
            return data;
        }
        else
        {
            return nullptr;
        }
    }

    Il2CppArray *GetArrayPointer(lua_State *L, int index, void *klass)
    {
        if (lapi_lua_type(L, index) == LUA_TTABLE)
        {
            return TranslateArray(L, index, (Il2CppClass *)klass);
        }
        else if (lapi_lua_isuserdata(L, index))
        {
            Il2CppObject *obj = GetRawObjPointer(L, index, (Il2CppClass *)klass);
            return (Il2CppArray *)obj;
        }
        else
        {
            throw_exception2lua(L, "invalid param for array");
            return nullptr;
        }
    }

    Il2CppString *Bytes2String(void *bytes)
    {
        return Bytes2StringMethodPointer(bytes, Bytes2StringMethodInfo);
    }

    void *GetCSharpStructPointer(lua_State *L, int index, void *typeId)
    {
        void *ptr = GetCppObjMapper()->ToStruct(L, index);
        if (ptr)
        {
            return ptr;
        }
        return nullptr;
    }

    template <typename T>
    static T *GetCSharpStructPointerFromLua(lua_State *L, int index, void *typeId)
    {
        void *ptr = GetCppObjMapper()->ToStruct(L, index, (Il2CppClass *)typeId);
        if (ptr)
        {
            return reinterpret_cast<T *>(ptr);
        }
        return nullptr;
    }

    intptr_t GetMethodPointer(Il2CppReflectionMethod *method)
    {
        auto methodInfo = method->method;
        auto ret = MetadataCache::GetMethodPointer(methodInfo->klass->image, methodInfo->token);
        if (!ret)
        {
            ret = methodInfo->methodPointer;
        }
        return (intptr_t)ret;
    }

    std::string ilstring2stdstring(Il2CppString *result)
    {
        const Il2CppChar *utf16 = il2cpp::utils::StringUtils::GetChars(result);
        return il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
    }

    void *LuaStr2CSharpString(lua_State *L, int index)
    {
        if (lapi_lua_isnil(L, index))
        {
            return nullptr;
        }
        const char *str = lapi_lua_tostring(L, index);
        if (str)
        {
            return String::NewWrapper(str);
        }
        else
        {
            throw_exception2lua_format(L, "invalid argument for lua string convert #%d", index);
        }
        return nullptr;
    }

    intptr_t GetMethodInfoPointer(Il2CppReflectionMethod *method)
    {
        return (intptr_t)method->method;
    }

    int32_t GetFieldOffset(Il2CppReflectionField *field, bool isInValueType)
    {
        return (int32_t)Field::GetOffset(field->field) - (Class::IsValuetype(Field::GetParent(field->field)) ? sizeof(RuntimeObject) : 0);
    }

    intptr_t GetFieldInfoPointer(Il2CppReflectionField *field)
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

    const Il2CppReflectionType *TypeIdToType(Il2CppClass *klass)
    {
        if (!klass)
            return nullptr;
        return Reflection::GetTypeObject(Class::GetType(klass));
    }

    static int32_t GetValueTypeSize(Il2CppClass *klass)
    {
        if (klass->native_size)
        {
            return klass->native_size;
        }
        else
        {
            return klass->instance_size - sizeof(RuntimeObject);
        }
    }
    static void *ObjectAllocate(Il2CppClass *klass)
    {
        if (Class::IsValuetype(klass))
        {
            return nullptr;
        }
        else
        {
            auto obj = il2cpp::vm::Object::New(klass);
            return obj;
        }
    }

    int throw_exception2lua(lua_State *L, const char *msg)
    {
        return lapi_luaL_error(L, msg);
    }

    int throw_exception2lua_format(lua_State *L, const char *format, ...)
    {
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        va_list args;
        va_start(args, format);
        vsnprintf(buffer, BUFFER_SIZE, format, args);
        va_end(args);
        return throw_exception2lua(L, buffer);
    }

    static void throwExceptionFromLuaError(lua_State *L, int oldTop)
    {
        const char *errorMsg = "";
        if (lapi_lua_isstring(L, -1))
        {
            errorMsg = lapi_lua_tostring(L, -1);
        }
        else
        {
            errorMsg = "unknown error in lua";
        }
        Exception::Raise(Exception::GetInvalidOperationException(errorMsg));
    }

    const Il2CppClass *GetReturnType(const MethodInfo *method)
    {
        if (kInvalidIl2CppMethodSlot != method->slot)
        {
            Class::Init(method->klass);
        }
        return Class::FromIl2CppType(Method::GetReturnType(method), false);
    }

    const Il2CppClass *GetParameterType(const MethodInfo *method, int index)
    {
        if (kInvalidIl2CppMethodSlot != method->slot)
        {
            Class::Init(method->klass);
        }
        const Il2CppType *type = Method::GetParam(method, index);
        if (type)
        {
            return Class::FromIl2CppType(type, false);
        }
        else
        {
            return nullptr;
        }
    }

    static std::map<const MethodInfo *, const MethodInfo *> WrapFuncPtrToMethodInfo;
    static std::recursive_mutex WrapFuncPtrToMethodInfoMutex;

    Il2CppDelegate *FunctionPointerToDelegate(Il2CppMethodPointer functionPtr, Il2CppClass *delegateType, Il2CppObject *target)
    {
        Il2CppObject *delegate = il2cpp::vm::Object::New(delegateType);
        const MethodInfo *invoke = il2cpp::vm::Runtime::GetDelegateInvoke(delegateType);

        const MethodInfo *method = NULL;
        {
            std::lock_guard<std::recursive_mutex> lock(WrapFuncPtrToMethodInfoMutex);
            il2cpp::utils::NativeDelegateMethodCache::GetNativeDelegate((Il2CppMethodPointer)invoke);
            auto iter = WrapFuncPtrToMethodInfo.find(invoke);
            if (iter == WrapFuncPtrToMethodInfo.end())
            {
                MethodInfo *newMethod = (MethodInfo *)IL2CPP_CALLOC(1, sizeof(MethodInfo));
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
        Type::ConstructClosedDelegate((Il2CppDelegate *)delegate, target, functionPtr, method);
#else
        Type::ConstructDelegate((Il2CppDelegate *)delegate, target, functionPtr, method);
#endif

        return (Il2CppDelegate *)delegate;
    }

    static DelegateMiddlerware *GetDelegateInfo(void *target)
    {
        return (DelegateMiddlerware *)(((char *)target) + sizeof(RuntimeObject));
    }

    static DelegateMiddlerware *GetDelegateInfoByDelegate(Il2CppDelegate *d)
    {
        return GetDelegateInfo(d->target);
    }

    void SetGlobalType_LuaObject(Il2CppReflectionType *type)
    {
        if (!type)
        {
            Exception::Raise(Exception::GetInvalidOperationException("type of JSObject is null"));
        }
        g_typeofDelegateMiddleware = il2cpp_codegen_class_from_type(type->type);
    }

    static int PropertyCallback(lua_State *L, WrapData *wrapDatas, int offset = 3)
    {
        try
        {
            bool checkArgument = true;
            if (wrapDatas->Wrap(wrapDatas->Method, wrapDatas->MethodPointer, L, checkArgument, wrapDatas, offset) >= 0)
            {
                return 1;
            }
            else
            {

                return throw_exception2lua(L, "invalid arguments for PropertyCallback");
            }
        }
        catch (Il2CppExceptionWrapper &exception)
        {
            Il2CppClass *klass = il2cpp::vm::Object::GetClass(exception.ex);
            const MethodInfo *toStringMethod = il2cpp::vm::Class::GetMethodFromName(klass, "ToString", 0);

            Il2CppException *outException = NULL;
            Il2CppString *result = (Il2CppString *)il2cpp::vm::Runtime::Invoke(toStringMethod, exception.ex, NULL, &outException);
            if (outException != NULL)
            {
                throw_exception2lua(L, "unknow c# execption!");
            }
            else
            {
                const Il2CppChar *utf16 = il2cpp::utils::StringUtils::GetChars(result);
                std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
                throw_exception2lua(L, str.c_str());
            }
        }
        return 0;
    }

    static int MethodCallback(lua_State *L, WrapData **wrapDatas, int paramOffset)
    {
        try
        {
            bool checkArgument = *wrapDatas && *(wrapDatas + 1);
            while (*wrapDatas)
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
        catch (Il2CppExceptionWrapper &exception)
        {
            Il2CppClass *klass = il2cpp::vm::Object::GetClass(exception.ex);
            const MethodInfo *toStringMethod = il2cpp::vm::Class::GetMethodFromName(klass, "ToString", 0);

            Il2CppException *outException = NULL;
            Il2CppString *result = (Il2CppString *)il2cpp::vm::Runtime::Invoke(toStringMethod, exception.ex, NULL, &outException);
            if (outException != NULL)
            {
                throw_exception2lua(L, "unknow c# execption!");
            }
            else
            {
                const Il2CppChar *utf16 = il2cpp::utils::StringUtils::GetChars(result);
                std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
                throw_exception2lua(L, str.c_str());
            }
        }
        return 0;
    }

    static int MethoCallBackHash1(lua_State *L)
    {
        WrapData **wrapData = GetLuaClassRegister()->GetMemberWrapData(4, 23);
        if (wrapData)
        {
            return MethodCallback(L, wrapData, 2);
        }
        else
        {
            // #TODO@benp
        }
        return 0;
    }
    void GetFieldValue(void *ptr, FieldInfo *field, size_t offset, void *value)
    {
        void *src;

        if (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
        {
            IL2CPP_ASSERT(ptr);
            src = (char *)ptr + offset;
            Field::SetValueRaw(field->type, value, src, true);
        }
        else
        {
            Field::StaticGetValue(field, value);
        }
    }

    void *GetValueTypeFieldPtr(void *obj, FieldInfo *field, size_t offset)
    {
        if (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
        {
            IL2CPP_ASSERT(obj);
            return (char *)obj + offset;
        }
        else
        {
            Class::SetupFields(field->parent);

            void *threadStaticData = NULL;
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
                return ((char *)threadStaticData) + threadStaticFieldOffset;
            }
            else
            {
                return ((char *)field->parent->static_fields) + field->offset;
            }
        }
    }

    void SetFieldValue(void *ptr, FieldInfo *field, size_t offset, void *value)
    {
        void *dest;

        if (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
        {
            IL2CPP_ASSERT(ptr);
            dest = (char *)ptr + offset;
            Field::SetValueRaw(field->type, dest, value, true);
        }
        else
        {
            Field::StaticSetValue(field, value);
        }
    }

    void *GetDefaultValuePtr(const MethodInfo *method, uint32_t index)
    {
        bool isExplicitySetNullDefaultValue = false;
#ifdef UNITY_2021_1_OR_NEWER
        Il2CppObject *defaultValue = Parameter::GetDefaultParameterValueObject(method, index, &isExplicitySetNullDefaultValue);
#else
        Il2CppObject *defaultValue = Parameter::GetDefaultParameterValueObject(method, &method->parameters[index], &isExplicitySetNullDefaultValue);
#endif
        return (defaultValue && Class::IsValuetype(Class::FromIl2CppType(Method::GetParam(method, index), false))) ? Object::Unbox(defaultValue) : defaultValue;
    }

    static FieldInfo *ArrayBufferCountField = nullptr;
    static FieldInfo *ArrayBufferBytesField = nullptr;

    static bool TryTranslatePrimitiveWithType(lua_State *L, void *ptr, const Il2CppType *type)
    {
        int t = type->type;
        switch (t)
        {
        case IL2CPP_TYPE_I1:
        {
            lapi_xlua_pushinteger(L, (int32_t)(*((int8_t *)ptr)));
            break;
        }
        case IL2CPP_TYPE_BOOLEAN:
        {
            lapi_lua_pushboolean(L, (int)(*((uint8_t *)ptr)));
            break;
        }
        case IL2CPP_TYPE_U1:
        {
            lapi_xlua_pushinteger(L, (uint32_t)(*((uint8_t *)ptr)));
            break;
        }
        case IL2CPP_TYPE_I2:
        {
            lapi_xlua_pushinteger(L, (int32_t)(*((int16_t *)ptr)));
            break;
        }
        case IL2CPP_TYPE_U2:
        {
            lapi_xlua_pushinteger(L, (uint32_t)(*((uint16_t *)ptr)));
            break;
        }
        case IL2CPP_TYPE_CHAR:
        {
            lapi_xlua_pushinteger(L, (int32_t)(*((Il2CppChar *)ptr)));
            break;
        }
#if IL2CPP_SIZEOF_VOID_P == 4
        case IL2CPP_TYPE_I:
#endif
        case IL2CPP_TYPE_I4:
        {
            lapi_xlua_pushinteger(L, (int32_t)(*((int32_t *)ptr)));
            break;
        }
#if IL2CPP_SIZEOF_VOID_P == 4
        case IL2CPP_TYPE_U:
#endif
        case IL2CPP_TYPE_U4:
        {
            lapi_xlua_pushinteger(L, (uint32_t)(*((uint32_t *)ptr)));
            break;
        }
#if IL2CPP_SIZEOF_VOID_P == 8
        case IL2CPP_TYPE_I:
#endif
        case IL2CPP_TYPE_I8:
        {
            lapi_xlua_pushinteger(L, *((int64_t *)ptr));
            break;
        }
#if IL2CPP_SIZEOF_VOID_P == 8
        case IL2CPP_TYPE_U:
#endif
        case IL2CPP_TYPE_U8:
        {
            lapi_xlua_pushinteger(L, *((uint64_t *)ptr));
            break;
        }
        case IL2CPP_TYPE_R4:
        {
            lapi_lua_pushnumber(L, (double)(*((float *)ptr)));
            break;
        }
        case IL2CPP_TYPE_R8:
        {
            lapi_lua_pushnumber(L, *((double *)ptr));
            break;
        }

        default:
            return false;
        }
        return true;
    }

    static bool TryTranslatePrimitiveWithClass(lua_State *L, Il2CppObject *obj, Il2CppClass *klass = nullptr)
    {
        if (obj)
        {
            const Il2CppType *type = Class::GetType(klass ? klass : obj->klass);
            int t = type->type;
            if (t == IL2CPP_TYPE_STRING)
            {
                const Il2CppChar *utf16 = il2cpp::utils::StringUtils::GetChars((Il2CppString *)obj);
                std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
                lapi_lua_pushstring(L, str.c_str());
                return true;
            }
            void *ptr = Object::Unbox(obj);
            return TryTranslatePrimitiveWithType(L, ptr, type);
        }
        return true;
    }

    bool TryTranslatePrimitive(lua_State *L, Il2CppObject *obj)
    {
        return TryTranslatePrimitiveWithClass(L, obj);
    }

    static bool CSValueToLuaValue(lua_State *L, void *klass, void *ptr, unsigned int size)
    {

        return GetCppObjMapper()->TryPushStruct(L, (Il2CppClass *)klass, ptr, size);
    }
    static bool CopyNullableValueType(lua_State *L, void *klass, void *ptr, bool hasValue, unsigned int size)
    {
        if (!hasValue)
        {
            lapi_lua_pushnil(L);
            return true;
        }
        return CSValueToLuaValue(L, klass, ptr, size);
    }

    static bool CSRefToLuaValue(lua_State *L, void *obj)
    {
        return GetCppObjMapper()->TryPushObject(L, (Il2CppObject *)obj);
    }

    static bool IsDelegateMiddleware(Il2CppObject *obj)
    {
        if (!obj)
        {
            return false;
        }
        if (IsDelegate(obj->klass))
        {
            Il2CppDelegate *d = (Il2CppDelegate *)obj;
            return d->target ? d->target->klass == g_typeofDelegateMiddleware : false;
        }
        return false;
    }

    /// @brief C#obj传递到lua 处理struct copy 值类型装箱
    /// @param L
    /// @param obj
    /// @param klass
    /// @return
    static bool CSAnyToLuaValue(lua_State *L, void *obj, void *klass = nullptr)
    {
        if (!obj)
        {
            lapi_lua_pushnil(L);
            return true;
        }
        Il2CppObject *il2cppObj = (Il2CppObject *)obj;
        Il2CppClass *ilcppClass = (Il2CppClass *)(klass ? klass : il2cppObj->klass);
        bool suc = TryTranslatePrimitive(L, il2cppObj);

        if (!suc)
        {
            if (Class::IsValuetype(ilcppClass))
            {
                if (il2cpp_class_is_enum(ilcppClass))
                {
                    auto t = Class::GetEnumBaseType(ilcppClass);
                    void *ptr = Object::Unbox(il2cppObj);
                    return TryTranslatePrimitiveWithType(L, ptr, t);
                }
                else
                {
                    return GetCppObjMapper()->TryPushStruct(L, ilcppClass, Object::Unbox(il2cppObj), GetValueTypeSize(ilcppClass));
                }
            }
            else if (IsArray(ilcppClass) && ilcppClass->element_class == il2cpp_defaults.byte_class)
            {
                auto ilstring = Bytes2String(obj);
                const Il2CppChar *utf16 = il2cpp::utils::StringUtils::GetChars(ilstring);
                std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
                lapi_lua_pushstring(L, str.c_str());
            }
            else if (IsDelegateMiddleware(il2cppObj))
            {
                DelegateMiddlerware *delegateInfo = GetDelegateInfoByDelegate((Il2CppDelegate *)obj);
                lapi_lua_getref(L, delegateInfo->reference);
            }
            // #TODO@benp 处理luabase
            else
            {
                return GetCppObjMapper()->TryPushObject(L, il2cppObj);
            }
        }
        return true;
    }

    Il2CppClass *GetLuaObjCls(lua_State *L, int index)
    {
        if (lapi_lua_isuserdata(L, index))
        {
            void *ptr = GetCppObjMapper()->GetCSharpObj(L, index);
            if (ptr)
            {
                auto obj = (Il2CppObject *)ptr;
                return obj->klass;
            }

            StructUD *sud = GetCppObjMapper()->ToStructUD(L, index);
            if (sud)
            {
                return (Il2CppClass *)sud->typeId;
            }
        }
        return nullptr;
    }

    // 从lua中构造一个C#的delegate
    Il2CppDelegate *CreateDelegateByMiddleware(lua_State *L, int32_t index, Il2CppClass *klass, bool throwIfFail)
    {
        LuaClassInfo *clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId(klass, L);
        if (clsInfo)
        {
            return GetDelegate(L, index, klass, (intptr_t)clsInfo->DelegateBridge);
        }
        else
        {
            throw_exception2lua(L, "expect function in index");
            return nullptr;
        }
    }

    Il2CppArray *TranslateArray(lua_State *L, int index, Il2CppClass *klass)
    {
        auto len = lapi_xlua_objlen(L, index);
        int n = lapi_lua_gettop(L);
        int idx = index > 0 ? index : n + index + 1;
        auto array = Array::NewSpecific(klass, len);
        if (!array)
        {
            return nullptr;
        }
        auto elementKlass = klass->element_class;

        for (uint32_t i = 0; i < len; i++)
        {
            lapi_lua_pushnumber(L, i + 1);
            lapi_lua_rawget(L, idx);
            if (lapi_lua_isnil(L, -1))
            {
                continue;
            }
            const Il2CppType *type = Class::GetType(klass->element_class);
            int elementType = type->type;
            switch (elementType)
            {
            case IL2CPP_TYPE_I1:
            {
                auto value = (int8_t)lapi_lua_tonumber(L, -1);
                il2cpp_array_set(array, int8_t, i, value);
                break;
            }
            case IL2CPP_TYPE_BOOLEAN:
            case IL2CPP_TYPE_U1:
            {
                auto value = (uint8_t)lapi_lua_tonumber(L, -1);
                il2cpp_array_set(array, uint8_t, i, value);
                break;
            }
            case IL2CPP_TYPE_I2:
            {
                auto value = (int16_t)lapi_lua_tonumber(L, -1);
                il2cpp_array_set(array, int16_t, i, value);
                break;
            }
            case IL2CPP_TYPE_U2:
            {
                auto value = (uint16_t)lapi_lua_tonumber(L, -1);
                il2cpp_array_set(array, uint16_t, i, value);
                break;
            }
            case IL2CPP_TYPE_CHAR:
            {
                auto value = (Il2CppChar)lapi_lua_tonumber(L, -1);
                il2cpp_array_set(array, Il2CppChar, i, value);
                break;
            }
#if IL2CPP_SIZEOF_VOID_P == 4
            case IL2CPP_TYPE_I:
#endif
            case IL2CPP_TYPE_I4:
            {
                auto value = (int32_t)lapi_lua_tonumber(L, -1);
                il2cpp_array_set(array, int32_t, i, value);
                break;
            }
#if IL2CPP_SIZEOF_VOID_P == 4
            case IL2CPP_TYPE_U:
#endif
            case IL2CPP_TYPE_U4:
            {
                auto value = (uint32_t)lapi_lua_tonumber(L, -1);
                il2cpp_array_set(array, uint32_t, i, value);
                break;
            }
#if IL2CPP_SIZEOF_VOID_P == 8
            case IL2CPP_TYPE_I:
#endif
            case IL2CPP_TYPE_I8:
            {
                auto value = lapi_lua_toint64(L, -1);
                il2cpp_array_set(array, int64_t, i, value);
                break;
            }
#if IL2CPP_SIZEOF_VOID_P == 8
            case IL2CPP_TYPE_U:
#endif
            case IL2CPP_TYPE_U8:
            {
                auto value = lapi_lua_touint64(L, -1);
                il2cpp_array_set(array, uint64_t, i, value);
                break;
            }
            case IL2CPP_TYPE_R4:
            {
                auto value = (float)lapi_lua_tonumber(L, -1);
                il2cpp_array_set(array, float, i, value);
                break;
            }
            case IL2CPP_TYPE_R8:
            {
                auto value = lapi_lua_tonumber(L, -1);
                il2cpp_array_set(array, double, i, value);
                break;
            }
            case IL2CPP_TYPE_VALUETYPE:
                /* note that 't' and 'type->type' can be different */
                if (type->type == IL2CPP_TYPE_VALUETYPE && Type::IsEnum(type))
                {
                    // #TODO@benp enum
                }
                else
                {
                    // 这里可以考虑实现成递归 递归要处理装箱拆箱的情况
                    if (lapi_lua_isuserdata(L, -1))
                    {
                        auto objClass = GetLuaObjCls(L, -1);
                        if (!Class::IsAssignableFrom(klass->element_class, objClass))
                        {
                            break;
                        }
                        void *p = GetRawValuePointer(L, -1, klass->element_class);
                        if (p)
                        {
                            auto size = GetValueTypeSize(elementKlass);
                            il2cpp_array_setrefwithsize(array, size, i, p);
                        }
                    }
                    else
                    {
                        auto obj = FallBackLua2CSObj(L, index, klass->element_class);
                        if (obj)
                        {
                            auto size = GetValueTypeSize(elementKlass);
                            il2cpp_array_setrefwithsize(array, size, i, Object::Unbox(obj));
                        }
                    }
                }
                break;
            default:
            {
                Il2CppObject *obj = LuaValueToCSRef(elementKlass, L, -1);
                if (obj)
                {
                    il2cpp_array_setref(array, i, obj);
                }
                break;
            }
            }
        }
        return array;
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

    Il2CppObject *LuaValueToCSRef(Il2CppClass *klass, lua_State *L, int index)
    {
        if (klass == il2cpp_defaults.void_class)
            return nullptr;

        if (!klass)
        {
            klass = il2cpp_defaults.object_class;
        }

        const Il2CppType *type = Class::GetType(klass);
        int t = type->type;

        PrimitiveValueType data;

        void *toBox = &data;

        Il2CppObject *ret = nullptr;

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
            data.i8 = (int64_t)lapi_lua_toint64(L, index);
            break;
        }
#if IL2CPP_SIZEOF_VOID_P == 8
        case IL2CPP_TYPE_U:
#endif
        case IL2CPP_TYPE_U8:
        {
            data.u8 = (uint64_t)lapi_lua_touint64(L, index);
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
                return (Il2CppObject *)il2cpp::vm::String::NewWrapper(str);
            }
            return nullptr;
        }
        case IL2CPP_TYPE_ARRAY:
        case IL2CPP_TYPE_SZARRAY:
        {
            return GetArrayPointer(L, index, klass);
        }
        case IL2CPP_TYPE_CLASS:
        case IL2CPP_TYPE_OBJECT:
        case IL2CPP_TYPE_FNPTR:
        case IL2CPP_TYPE_PTR:
        {
            int valueType = lapi_lua_type(L, index);
            if (valueType == LUA_TFUNCTION)
            {
                if (IsDelegate(klass))
                {
                    return (Il2CppObject *)CreateDelegateByMiddleware(L, index, klass, true);
                }
            }

            if (valueType == LUA_TUSERDATA)
            {
                auto ptr = GetCppObjMapper()->ToCppObj(L, index); // 原生C#对象
                if (ptr)
                {
                    auto objClass = GetLuaObjCls(L, index);
                    if (Class::IsAssignableFrom(klass, objClass))
                    {
                        return Class::IsValuetype(objClass) ? Object::Box(objClass, ptr) : (Il2CppObject *)ptr;
                    }
                }
            }

            if (klass == il2cpp_defaults.object_class)
            {
                int type = lapi_lua_type(L, index);
                switch (type)
                {
                case LUA_TNIL:
                    return nullptr;
                case LUA_TBOOLEAN:
                {
                    t = IL2CPP_TYPE_BOOLEAN;
                    klass = il2cpp_defaults.boolean_class;
                    break;
                }
                // case LUA_TLIGHTUSERDATA: pointer
                case LUA_TNUMBER:
                {
                    if (lapi_lua_isinteger(L, index))
                    {
                        /*t = IL2CPP_TYPE_I4;
                        klass = il2cpp_defaults.int32_class;*/
                        t = IL2CPP_TYPE_I8;
                        klass = il2cpp_defaults.int64_class;
                    }
                    else
                    {
                        t = IL2CPP_TYPE_R8;
                        klass = il2cpp_defaults.double_class;
                    }
                    break;
                }
                case LUA_TSTRING:
                {
                    t = IL2CPP_TYPE_STRING;
                    klass = il2cpp_defaults.string_class;
                    break;
                }
                case LUA_TTABLE:
                {
                    // todo 转换为cstable
                    return nullptr;
                }
                case LUA_TUSERDATA:
                {
                    if (lapi_lua_isint64(L, index))
                    {
                        t = IL2CPP_TYPE_I8;
                        klass = il2cpp_defaults.int64_class;
                        break;
                    }
                    else if (lapi_lua_isuint64(L, index))
                    {
                        t = IL2CPP_TYPE_U8;
                        klass = il2cpp_defaults.uint64_class;
                        break;
                    }
                    else
                    {
                        goto FallBackLua2CS;
                    }
                }
                default:
                    return nullptr;
                }
                goto handle_underlying;
            }
        FallBackLua2CS:
            return FallBackLua2CSObj(L, index, klass);
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
                if (lapi_lua_isuserdata(L, index))
                {
                    auto objClass = GetLuaObjCls(L, index);
                    if (!Class::IsAssignableFrom(klass, objClass))
                    {
                        return nullptr;
                    }
                    toBox = GetRawValuePointer(L, index, klass);
                    if (!toBox)
                    {
                        std::string message = "expect ValueType: ";
                        message += klass->name;
                        message += ", by got null";
                        Exception::Raise(Exception::GetInvalidOperationException(message.c_str()));
                        return nullptr;
                    }
                }
                else
                {
                    return FallBackLua2CSObj(L, index, klass);
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

    void ReleaseCSharpTypeInfo(LuaClassInfo *classInfo)
    {
        // TODO: 有内存泄漏，需要释放里面的内容
        delete classInfo;
    }

    static void SetParamArrayFlagAndOptionalNum(WrapData *data, const char *signature)
    {
        data->HasParamArray = false;
        data->OptionalNum = 0;

        const char *p = signature;
        while (*p)
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

    WrapData *AddConstructor(LuaClassInfo *classInfo, const char *signature, WrapFuncPtr WrapFunc, void *method, MethodPointer methodPointer, int typeInfoNum)
    {
        if (!WrapFunc)
            return nullptr;
        int allocSize = sizeof(WrapData) + sizeof(void *) * typeInfoNum;
        WrapData *data = (WrapData *)malloc(allocSize);
        memset(data, 0, allocSize);
        data->Method = static_cast<MethodInfo *>(method);
        data->MethodPointer = methodPointer;
        data->Wrap = WrapFunc;
        data->IsStatic = false;
        data->IsExtensionMethod = false;
        data->klass = classInfo->klass;
        SetParamArrayFlagAndOptionalNum(data, signature);

        classInfo->Ctors.push_back(data);
        return data;
    }

    WrapData *AddMethod(LuaClassInfo *classInfo, Il2CppString *signatureIlstring, WrapFuncPtr WrapFunc, Il2CppString *nameIlstring, bool isStatic, bool isExtensionMethod, bool isGetter, bool isSetter, void *method, MethodPointer methodPointer, int typeInfoNum)
    {
        if (!WrapFunc)
            return nullptr;
        int allocSize = sizeof(WrapData) + sizeof(void *) * typeInfoNum;
        WrapData *data = (WrapData *)malloc(allocSize);
        memset(data, 0, allocSize);
        data->Method = static_cast<MethodInfo *>(method);
        data->MethodPointer = methodPointer;
        data->Wrap = WrapFunc;
        data->IsStatic = isStatic;
        data->klass = classInfo->klass;
        data->IsExtensionMethod = isExtensionMethod;
        std::string str = ilstring2stdstring(signatureIlstring);
        const char *signature = str.c_str();
        SetParamArrayFlagAndOptionalNum(data, signature);

        std::string nameStr = ilstring2stdstring(nameIlstring);
        const char *name = nameStr.c_str();

        for (int i = 0; i < classInfo->Methods.size(); ++i)
        {
            if (classInfo->Methods[i].IsStatic == isStatic && classInfo->Methods[i].IsGetter == isGetter && classInfo->Methods[i].IsGetter == isGetter && classInfo->Methods[i].Name == name)
            {
                if (isGetter || isSetter) // no overload for getter or setter
                {
                    free(data);
                    return nullptr;
                }
                // PLog("add overload for %s, %s", name, signature);
                classInfo->Methods[i].OverloadDatas.push_back(data);
                return data;
            }
        }

        std::vector<WrapData *> OverloadDatas;
        OverloadDatas.push_back(data);
        classInfo->Methods.push_back({std::string(name), isStatic, isGetter, isSetter, std::move(OverloadDatas)});
        return data;
    }

    WrapData *AddProperty(LuaClassInfo *classInfo, Il2CppString *signatureIlstring, WrapFuncPtr WrapFunc, Il2CppString *nameIlstring, bool isStatic, bool isGetter, void *method, MethodPointer methodPointer, int typeInfoNum)
    {
        if (!WrapFunc)
            return nullptr;
        int allocSize = sizeof(WrapData) + sizeof(void *) * typeInfoNum;
        WrapData *data = (WrapData *)malloc(allocSize);
        memset(data, 0, allocSize);
        data->Method = static_cast<MethodInfo *>(method);
        data->MethodPointer = methodPointer;
        data->Wrap = WrapFunc;
        data->IsStatic = isStatic;
        data->IsExtensionMethod = false;
        data->klass = classInfo->klass;
        std::string str = ilstring2stdstring(signatureIlstring);
        const char *signature = str.c_str();
        SetParamArrayFlagAndOptionalNum(data, signature);

        std::string nameStr = ilstring2stdstring(nameIlstring);
        const char *name = nameStr.c_str();

        for (int i = 0; i < classInfo->Properties.size(); ++i)
        {
            auto propertyInfo = &classInfo->Properties[i];
            if (propertyInfo->Name == name)
            {
                if (isGetter)
                {
                    propertyInfo->GetWrapData = data;
                }
                else
                {
                    propertyInfo->SetWrapData = data;
                }
                return data;
            }
        }

        PropertyWrapData propertyInfo;
        propertyInfo.Name = name;
        propertyInfo.IsStatic = isStatic;
        propertyInfo.GetWrapData = nullptr;
        propertyInfo.SetWrapData = nullptr;
        if (isGetter)
        {
            propertyInfo.GetWrapData = data;
        }
        else
        {
            propertyInfo.SetWrapData = data;
        }
        classInfo->Properties.push_back(std::move(propertyInfo));

        return data;
    }

    bool AddField(LuaClassInfo *classInfo, FieldWrapFuncInfo *wrapFuncInfo, Il2CppString *nameIlstring, bool is_static, void *fieldInfo, int offset, void *fieldTypeInfo)
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
        const char *name = str.c_str();

        FieldWrapData *data = new FieldWrapData();
        data->Getter = Getter;
        data->Setter = Setter;
        data->FieldInfo = fieldInfo;
        data->Offset = offset;
        data->TypeInfo = fieldTypeInfo;

        classInfo->Fields.push_back({std::string(name), is_static, data});
        return true;
    }

    void SetTypeInfo(WrapData *data, int index, void *typeInfo)
    {
        data->TypeInfos[index] = typeInfo;
    }

    bool CheckIsArray(lua_State *L, int index, void *typeInfo)
    {
        int type = lapi_lua_type(L, index);
        return type == LUA_TTABLE || type == LUA_TUSERDATA;
    }

    bool CheckIsString(lua_State *L, int index)
    {
        int type = lapi_lua_type(L, index);
        return type == LUA_TSTRING || type == LUA_TNIL;
    }

    bool CheckIsClass(lua_State *L, int index, void *typeId)
    {
        if (lapi_lua_isuserdata(L, index))
        {
            auto ptr = xlua::GetCppObjMapper()->GetCSharpObj(L, index);
            if (ptr)
            {
                void *kclass = *reinterpret_cast<void **>(ptr);
                if (Class::IsAssignableFrom((Il2CppClass *)typeId, (Il2CppClass *)kclass))
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool CheckIsStruct(lua_State *L, int index, void *typeId)
    {
        auto sud = GetCppObjMapper()->ToStructUD(L, index);
        if (sud)
        {
            return sud && sud->typeId == typeId;
        }

        return CheckIsClass(L, index, typeId);
    }

    bool CheckIsNullableStruct(lua_State *L, int index, void *typeId)
    {
        return CheckIsStruct(L, index, typeId) || lapi_lua_isnil(L, index);
    }

    template <typename T>
    struct OptionalParameter
    {
        static T GetPrimitive(lua_State *L, int index, const void *methodInfo, WrapData *wrapData, int paramIndex)
        {
            if (lapi_lua_gettop(L) >= index)
            {
                return converter::Converter<T>::toCpp(L, index);
            }
            else
            {
                if (wrapData->IsExtensionMethod)
                    ++paramIndex;
                auto pret = (T *)GetDefaultValuePtr((MethodInfo *)methodInfo, paramIndex);
                if (pret)
                {
                    return *pret;
                }
                return {};
            }
        }

        static T GetValueType(lua_State *L, int index, const void *methodInfo, WrapData *wrapData, int paramIndex)
        {
            if (lapi_lua_gettop(L) >= index)
            {
                T *p = GetCSharpStructPointerFromLua<T>(L, index, nullptr);
                if (p)
                {
                    return *p;
                }
                else
                {
                    return {};
                }
                // return GetCSharpStructPointer(L, index);
            }
            else
            {
                if (wrapData->IsExtensionMethod)
                    ++paramIndex;
                auto pret = (T *)GetDefaultValuePtr((MethodInfo *)methodInfo, paramIndex);
                if (pret)
                {
                    return *pret;
                }
                T ret;
                memset(&ret, 0, sizeof(T));
                return ret;
            }
        }

        static void *GetString(lua_State *L, int index, const void *methodInfo, WrapData *wrapData, int paramIndex)
        {
            if (lapi_lua_gettop(L) >= index)
            {
                return LuaStr2CSharpString(L, index);
            }
            else
            {
                if (wrapData->IsExtensionMethod)
                    ++paramIndex;
                return xlua::GetDefaultValuePtr((MethodInfo *)methodInfo, paramIndex);
            }
        }

        static void *GetRefType(lua_State *L, int index, const void *methodInfo, WrapData *wrapData, int paramIndex, const void *typeId)
        {
            if (lapi_lua_gettop(L) >= index)
            {
                return (void *)LuaValueToCSRef((Il2CppClass *)typeId, L, index);
            }
            else
            {
                if (wrapData->IsExtensionMethod)
                    ++index;
                return GetDefaultValuePtr((MethodInfo *)methodInfo, index);
            }
        }
    };

    template <typename T>
    struct RestArguments
    {
        static void *PackPrimitive(lua_State *L, int index, void *klass)
        {
            int top = lapi_lua_gettop(L);
            if (top < index)
            {
                return nullptr;
            }
            int len = top - index + 1;
            auto array = Array::NewSpecific((Il2CppClass *)klass, len);
            for (int i = index; i <= top; i++)
            {
                il2cpp_array_set(array, T, i, converter::Converter<T>::toCpp(L, i));
            }
            /*T* arr = (T*)(Array::GetFirstElementAddress(array));
            {
                array[i - index] = converter::Converter<T>::toCpp(L, i);
            }*/

            return array;
        }

        static void *PackString(lua_State *L, int index, void *klass)
        {
            int top = lapi_lua_gettop(L);
            if (top < index)
            {
                return nullptr;
            }
            int len = top - index + 1;
            auto array = Array::NewSpecific((Il2CppClass *)klass, len);
            for (int i = index; i <= top; i++)
            {
                void *p = LuaStr2CSharpString(L, i);
                il2cpp_array_setref(array, i - index, p);
            }

            return array;
        }

        static void *PackRef(lua_State *L, int index, void *klass)
        {
            int top = lapi_lua_gettop(L);
            if (top < index)
            {
                return nullptr;
            }
            int len = top - index + 1;
            auto array = Array::NewSpecific((Il2CppClass *)klass, len);
            auto elementClass = ((Il2CppClass *)klass)->element_class;
            for (int i = index; i <= top; i++)
            {
                void *p = LuaValueToCSRef(elementClass, L, i);
                il2cpp_array_setref(array, i - index, p);
            }
            return array;
        }

        static void *PackValueType(lua_State *L, int index, void *klass)
        {
            int top = lapi_lua_gettop(L);
            if (top < index)
            {
                return nullptr;
            }
            int len = top - index + 1;
            auto array = Array::NewSpecific((Il2CppClass *)klass, len);
            T *arr = (T *)(Array::GetFirstElementAddress(array));
            auto elementClass = ((Il2CppClass *)klass)->element_class;
            for (int i = index; i <= top; i++)
            {
                T *e = GetCSharpStructPointerFromLua<T>(L, i, elementClass);
                if (!e)
                    continue;
                arr[i - index] = *e;
            }
            return array;
        }
    };

#include "FunctionBridge.Gen.h"

    BridgeFuncInfo *FindBridgeFunc(Il2CppString *ilstring)
    {
        std::string str = ilstring2stdstring(ilstring);
        const char *signature = str.c_str();
        auto begin = &g_bridgeFuncInfos[0];
        auto end = &g_bridgeFuncInfos[sizeof(g_bridgeFuncInfos) / sizeof(BridgeFuncInfo) - 1];
        auto first = std::lower_bound(begin, end, signature, [](const BridgeFuncInfo &x, const char *signature)
                                      { return strcmp(x.Signature, signature) < 0; });
        if (first != end && strcmp(first->Signature, signature) == 0)
        {
            return first;
        }
        return nullptr;
    }

    WrapFuncPtr FindWrapFunc(Il2CppString *ilstring)
    {
        std::string str = ilstring2stdstring(ilstring);
        const char *signature = str.c_str();
        auto begin = &g_wrapFuncInfos[0];
        auto end = &g_wrapFuncInfos[sizeof(g_wrapFuncInfos) / sizeof(WrapFuncInfo) - 1];
        auto first = std::lower_bound(begin, end, signature, [](const WrapFuncInfo &x, const char *signature)
                                      { return strcmp(x.Signature, signature) < 0; });
        if (first != end && strcmp(first->Signature, signature) == 0)
        {
            return first->Method;
        }
        return nullptr;
    }

    FieldWrapFuncInfo *FindFieldWrapFuncInfo(Il2CppString *ilstring)
    {
        std::string str = ilstring2stdstring(ilstring);
        const char *signature = str.c_str();
        auto begin = &g_fieldWrapFuncInfos[0];
        auto end = &g_fieldWrapFuncInfos[sizeof(g_fieldWrapFuncInfos) / sizeof(FieldWrapFuncInfo) - 1];
        auto first = std::lower_bound(begin, end, signature, [](const FieldWrapFuncInfo &x, const char *signature)
                                      { return strcmp(x.Signature, signature) < 0; });
        if (first != end && strcmp(first->Signature, signature) == 0)
        {
            return first;
        }

        return nullptr;
    }

    LuaClassInfo *CreateCSharpTypeInfo(Il2CppString *name, const void *type_id, const void *super_type_id, void *klass, bool isValueType, bool isDelegate, Il2CppString *delegateSignature)
    {
        const Il2CppChar *utf16 = il2cpp::utils::StringUtils::GetChars(name);
        std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
        BridgeFuncInfo *bridgeFuncInfo = nullptr;
        if (isDelegate)
        {
            bridgeFuncInfo = FindBridgeFunc(delegateSignature);
            if (!bridgeFuncInfo)
                return nullptr;
        }
        LuaClassInfo *ret = new LuaClassInfo();
        ret->Name = str;
        ret->SuperTypeId = super_type_id;
        ret->klass = static_cast<Il2CppClass *>(klass);

        if (bridgeFuncInfo)
        {
            ret->DelegateBridge = bridgeFuncInfo->Method;
            ret->DelegateWrapBridge = bridgeFuncInfo->WrapMethod;
        }
        return ret;
    }

    static LuaClassInfo *GetLuaClsInfo(lua_State *L)
    {
        // todo tag 保护
        void *ptr = lapi_lua_touserdata(L, lapi_lapi_lua_upvalueindex(1));
        if (ptr)
        {
            return (LuaClassInfo *)ptr;
        }
        return nullptr;
    }

    // class的set回调
    int ClsSetCallBack(lua_State *L)
    {
        auto clsInfo = GetLuaClsInfo(L);

        while (clsInfo)
        {

            IL2CPP_RUNTIME_CLASS_INIT(clsInfo->klass);
            if (lapi_lua_isstring(L, 2))
            {
                const char *key = lapi_lua_tostring(L, 2);

                auto setIter = clsInfo->ClsSetMap.find(key);
                if (setIter != clsInfo->ClsSetMap.end())
                {
                    MemberWrapData member = setIter->second;
                    if (member.type == XLUA_MemberType_Property)
                    {
                        WrapData *wrapData = (WrapData *)member.data;
                        // -1 cls -2 key -3 param
                        lapi_lua_replace(L, 2);
                        // -1 cls -2 param
                        return xlua::PropertyCallback(L, wrapData, 2);
                    }
                    else if (member.type == XLUA_MemberType_Field)
                    {
                        FieldWrapData *field = (FieldWrapData *)member.data;

                        field->Setter(L, (FieldInfo *)field->FieldInfo, field->Offset, (Il2CppClass *)field->TypeInfo, clsInfo->klass);
                        return 1;
                    }
                }
            }

            if (clsInfo->SuperTypeId)
            {
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass *)clsInfo->SuperTypeId, L);
            }
            else
            {
                clsInfo = nullptr;
            }
        }
        return 0;
    }

    // luaStack 1 c#obj 2 param1 3param2
    int MethodCallbackLuaWrap(lua_State *L, int paramOffset = 2)
    {

        void *pointer = lapi_lua_touserdata(L, lapi_lapi_lua_upvalueindex(1));
        if (pointer)
        {
            WrapData **wrapDatas = reinterpret_cast<WrapData **>(pointer);
            return MethodCallback(L, wrapDatas, paramOffset);
        }

        return 0;
    }

    // class的get回调
    int ClsGetCallBack(lua_State *L)
    {
        auto clsInfo = GetLuaClsInfo(L);
        while (clsInfo)
        {
            IL2CPP_RUNTIME_CLASS_INIT(clsInfo->klass);

            if (lapi_lua_isstring(L, 2))
            {
                const char *key = lapi_lua_tostring(L, 2);

                auto getIter = clsInfo->ClsGetMap.find(key);
                if (getIter != clsInfo->ClsGetMap.end())
                {
                    MemberWrapData member = getIter->second;
                    if (member.type == XLUA_MemberType_Method)
                    {
                        WrapData **method = (WrapData **)member.data;
                        lapi_lua_pushlightuserdata(L, method);
                        lapi_lua_pushcclosure(
                            L, [](lua_State *L)
                            { return MethodCallbackLuaWrap(L, 1); },
                            1);
                        lapi_lua_pushvalue(L, 1);  //-1 clsTbl -2closure -3 cls
                        lapi_lua_pushvalue(L, 2);  //-1 stringkey -2 clsTbl   -3closure
                        lapi_lua_pushvalue(L, -3); //-1 closure -2 stringkey -3 clsTbl   -4closure
                        lapi_lua_rawset(L, -3);
                        lapi_lua_pop(L, 1);
                        return 1;
                    }
                    else if (member.type == XLUA_MemberType_Field)
                    {

                        FieldWrapData *field = (FieldWrapData *)member.data;

                        field->Getter(L, (FieldInfo *)field->FieldInfo, field->Offset, (Il2CppClass *)field->TypeInfo, clsInfo->klass);
                        return 1;
                    }
                    else
                    {
                        WrapData *wrapData = (WrapData *)member.data;
                        lapi_lua_pop(L, 1);
                        // -1 obj
                        return xlua::PropertyCallback(L, wrapData, 2);
                    }
                }
            }

            if (clsInfo->SuperTypeId)
            {
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass *)clsInfo->SuperTypeId, L);
            }
            else
            {
                clsInfo = nullptr;
            }
        }
        return 0;
    }
    // 构造参数回调
    int ClsConstructorCallBack(lua_State *L)
    {
        auto clsInfo = GetLuaClsInfo(L);

        if (clsInfo)
        {
            Il2CppClass *klass = clsInfo->klass;
            if (IsArray(klass))
            {
                if (lapi_lua_isnumber(L, 2))
                { // 数组长度
                    int len = lapi_xlua_tointeger(L, 2);
                    Il2CppArray *array = il2cpp_array_new_specific(klass, len);
                    if (array)
                    {
                        GetCppObjMapper()->TryPushObject(L, array);
                        return 1;
                    }
                    else
                    {
                        throw_exception2lua(L, "malloc array failed");
                        return 0;
                    }
                }
                else
                {
                    throw_exception2lua(L, "param wrong (not number) for C# array");
                }
            }
            else if (!Class::IsValuetype(klass))
            {

                auto *ptr = il2cpp::vm::Object::New(klass);
                //-1 clsTable -2 param -top obj
                if (GetCppObjMapper()->TryPushObject(L, ptr))
                {
                    //-1 obj -2 param
                    lapi_lua_replace(L, 1);

                    if (clsInfo->CtorWrapDatas)
                    {
                        int count = MethodCallback(L, clsInfo->CtorWrapDatas, 2); // 目前不支持 构造函数的参数是ref类型的
                        if (count >= 0)
                        {
                            lapi_lua_settop(L, 1);
                            return 1;
                        }
                        else
                        {
                            // set top为0 留给gc处理
                            return 0;
                        }
                    }
                }
                else
                {
                    throw_exception2lua(L, "push value to lua failed");
                    return 0;
                }
            }
            else
            {
                int32_t typeId = GetLuaClassRegister()->GetTypeIdByIl2cppClass(L, klass);
                if (klass->has_references)
                {
                    auto obj = Object::New(klass);
                    GetCppObjMapper()->TryPushObject(L, obj);
                }
                else
                {
                    int32_t size = klass->native_size > 0 ? klass->native_size : (klass->instance_size - sizeof(Il2CppObject));
                    GetCppObjMapper()->CreateStruct(L, klass, size);
                }
                if (clsInfo->CtorWrapDatas && *clsInfo->CtorWrapDatas)
                {
                    //-1 obj -2 param
                    lapi_lua_replace(L, 1);
                    if (MethodCallback(L, clsInfo->CtorWrapDatas, 2) >= 0)
                    {
                        lapi_lua_settop(L, 1);
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
        return 0;
    }

    /// Lua CSharp对象的setter函数
    int ObjSetCallBack(lua_State *L)
    {
        auto clsInfo = GetLuaClsInfo(L);
        while (clsInfo)
        {
            if (lapi_lua_isstring(L, 2))
            {
                const char *key = lapi_lua_tostring(L, 2);
                auto setIter = clsInfo->ObjSetMap.find(key);
                if (setIter != clsInfo->ObjSetMap.end())
                {
                    MemberWrapData member = setIter->second;
                    if (member.type == XLUA_MemberType_Field)
                    {
                        FieldWrapData *field = (FieldWrapData *)member.data;
                        field->Setter(L, (FieldInfo *)field->FieldInfo, field->Offset, (Il2CppClass *)field->TypeInfo, clsInfo->klass);
                        return 1;
                    }
                    else
                    {
                        WrapData *wrapData = (WrapData *)member.data;
                        lapi_lua_replace(L, 2);
                        return xlua::PropertyCallback(L, wrapData, 2);
                    }
                }
            }
            int idx = lapi_lua_isnumber(L, 2);
            if (idx)
            {
                if (clsInfo->Indexer && clsInfo->Indexer->SetWrapData)
                {
                    lapi_lua_pushnumber(L, idx);
                    lapi_lua_replace(L, 2);
                    if (IsArray(clsInfo->klass))
                    {
                        PropertyCallback(L, clsInfo->Indexer->SetWrapData, 1);
                    }
                    else
                    {
                        PropertyCallback(L, clsInfo->Indexer->SetWrapData, 2);
                    }
                    return 0;
                }
            }

            if (clsInfo->SuperTypeId)
            {
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass *)clsInfo->SuperTypeId, L);
            }
            else
            {
                clsInfo = nullptr;
            }
        }

        throw_exception2lua(L, "obj invalid for ObjSetCallBack");
        return 0;
    }

    /// Lua CSharp对象的getter函数 hash
    int ObjGetCallBack_Hash(lua_State *L)
    {
        auto clsInfo = GetLuaClsInfo(L);

        while (clsInfo)
        {
            int type = lapi_lua_type(L, 2);
            switch (type)
            {
            case LUA_TSTRING:
            {
                size_t len = 0;
                const char *key = lapi_lua_tolstring(L, 2, &len);
                auto hash = clsInfo->memberHash(key, len);
                // #TODO@benp check
                MemberWrapData member = clsInfo->memberWarpDatas[hash];
                switch (member.type)
                {
                case XLUA_MemberType_Method:{
                    // #TODO@benp  正式处理
                    lapi_lua_pushcclosure(L, MethoCallBackHash1, 0);
                    return 1;
                }
                case XLUA_MemberType_Field: {
                    FieldWrapData *field = (FieldWrapData *)member.data;
                    field->Getter(L, (FieldInfo *)field->FieldInfo, field->Offset, (Il2CppClass *)field->TypeInfo, clsInfo->klass);
                    return 1;
                }
                case XLUA_MemberType_Property: {
                    WrapData *wrapData = (WrapData *)member.data;
                    lapi_lua_settop(L, 1);
                    PropertyCallback(L, wrapData, 2);
                    return 1;
                }
                default:
                    break;
                }
                break;
            }
            case LUA_TNUMBER: 
            {
                if (clsInfo->Indexer && clsInfo->Indexer->GetWrapData)
                {
                    int number = lapi_xlua_tointeger(L, 2);
                    lapi_lua_pop(L, 1);
                    lapi_lua_pushnumber(L, number);
                    if (IsArray(clsInfo->klass))
                    {
                        PropertyCallback(L, clsInfo->Indexer->GetWrapData, 1);
                    }
                    else
                    {
                        PropertyCallback(L, clsInfo->Indexer->GetWrapData, 2);
                    }
                    return 1;
                }
            }
            default:
                break;
            }

            if (clsInfo->SuperTypeId)
            {
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass *)clsInfo->SuperTypeId, L);
            }
            else
            {
                clsInfo = nullptr;
            }
        }
        return 0;
    }

    /// Lua CSharp对象的getter函数
    ///  1 getFromCache 1 method 2 getter
    ///  2 cspointer=>kclass=>luaDef
    ///  3 luaDef=> method or field
    ///  4 push result
    int ObjGetCallBack(lua_State *L)
    {
        if (!lapi_lua_isnil(L, lapi_lapi_lua_upvalueindex(2)))
        {
            lapi_lua_pushvalue(L, 2);
            lapi_lua_gettable(L, lapi_lapi_lua_upvalueindex(2));
            if (!lapi_lua_isnil(L, -1))
            { // has method
                return 1;
            }
            lapi_lua_pop(L, 1);
        }
        auto clsInfo = GetLuaClsInfo(L);
        int type = lapi_lua_type(L, 2);
        while (clsInfo)
        {
            switch (type)
            {
            case LUA_TSTRING:
            {
                const char* key = lapi_lua_tolstring(L, 2, nullptr);
                auto getIter = clsInfo->ObjGetMap.find(key);
                if (getIter != clsInfo->ObjGetMap.end())
                {
                    MemberWrapData member = getIter->second;
                    if (member.type == XLUA_MemberType_Method)
                    {
                        WrapData** method = (WrapData**)member.data;
                        lapi_lua_pushlightuserdata(L, method);
                        lapi_lua_pushcclosure(
                            L, [](lua_State* L)
                            { return MethodCallbackLuaWrap(L, 2); },
                            1);

                        // cache method
                        lapi_lua_pushvalue(L, lapi_lapi_lua_upvalueindex(2)); //-1 table   -2closure
                        if (lapi_lua_type(L, lapi_lua_gettop(L)) == LUA_TTABLE)
                        {
                            lapi_lua_pushvalue(L, 2);  //-1 stringkey -2 table   -3closure
                            lapi_lua_pushvalue(L, -3); //-1 closure -2 stringkey -3 table   -4closure
                            lapi_lua_settable(L, -3);
                        }
                        lapi_lua_pop(L, 1);
                        return 1;
                    }
                    else if (member.type == XLUA_MemberType_Field)
                    {

                        FieldWrapData* field = (FieldWrapData*)member.data;
                        field->Getter(L, (FieldInfo*)field->FieldInfo, field->Offset, (Il2CppClass*)field->TypeInfo, clsInfo->klass);
                        return 1;
                    }
                    else if (member.type == XLUA_MemberType_Property)
                    {
                        WrapData* wrapData = (WrapData*)member.data;
                        lapi_lua_settop(L, 1);
                        PropertyCallback(L, wrapData, 2);
                        return 1;
                    }
                }
            }
            case LUA_TNUMBER:
            {
                if (clsInfo->Indexer && clsInfo->Indexer->GetWrapData)
                {
                    int number = lapi_xlua_tointeger(L, 2);
                    lapi_lua_pop(L, 1);
                    lapi_lua_pushnumber(L, number);
                    if (IsArray(clsInfo->klass))
                    {
                        PropertyCallback(L, clsInfo->Indexer->GetWrapData, 1);
                    }
                    else
                    {
                        PropertyCallback(L, clsInfo->Indexer->GetWrapData, 2);
                    }
                    return 1;
                }
            }
            default:
                break;
            }

            if (clsInfo->SuperTypeId)
            {
                clsInfo = GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId((Il2CppClass*)clsInfo->SuperTypeId, L);
            }
            else
            {
                clsInfo = nullptr;
            }
        }
        return 0;
    }

    void SetCSharpAPI(Il2CppArray *methodArray)
    {

        auto getTypeIdMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod *, 0);
        auto methodPointer = xlua::GetMethodPointer(*getTypeIdMethod);
        auto methodInfo = xlua::GetMethodInfoPointer(*getTypeIdMethod);
        xlua::GetLuaClassRegister()->SetGetTypeIdFuncPtr((CSharpGetTypeIdFunc)methodPointer, (void *)methodInfo);

        auto addObjMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod *, 1);
        auto addObjMethodPointer = xlua::GetMethodPointer(*addObjMethod);
        auto addObjMethodInfo = xlua::GetMethodInfoPointer(*addObjMethod);

        auto removeObjMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod *, 2);
        auto removeObjMethodPointer = xlua::GetMethodPointer(*removeObjMethod);
        auto removeObjMethodInfo = xlua::GetMethodInfoPointer(*removeObjMethod);
        GetCppObjMapper()->InitObjPoolMethod((Il2CppReflectionMethod *)addObjMethod, (Il2CppMethodPointer *)addObjMethodPointer, (Il2CppReflectionMethod *)removeObjMethod, (Il2CppMethodPointer *)removeObjMethodPointer);

        auto delegateCombineMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod *, 3);
        DelegateCombineMethodPointer = (DelegateCombineType)xlua::GetMethodPointer(*delegateCombineMethod);
        DelegateCombineMethodInfo = (*delegateCombineMethod)->method;

        auto delegateRemoveMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod *, 4);
        DelegateRemoveMethodPointer = (DelegateRemoveType)xlua::GetMethodPointer(*delegateRemoveMethod);
        DelegateRemoveMethodInfo = (*delegateRemoveMethod)->method;

        auto Bytes2StringMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod *, 5);
        Bytes2StringMethodPointer = (Bytes2StringType)xlua::GetMethodPointer(*Bytes2StringMethod);
        Bytes2StringMethodInfo = (*Bytes2StringMethod)->method;

        auto FallBackLua2CSObjMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod *, 6);
        FallBackLua2CSObjMethodPointer = (FallBackLua2CSObjType)xlua::GetMethodPointer(*FallBackLua2CSObjMethod);
        FallBackLua2CSObjMethodInfo = (*FallBackLua2CSObjMethod)->method;

        auto GetDelegateMethod = il2cpp_array_addr(methodArray, Il2CppReflectionMethod *, 7);
        GetDelegateMethodPointer = (GetDelegateType)xlua::GetMethodPointer(*GetDelegateMethod);
        GetDelegateMethodInfo = (*GetDelegateMethod)->method;
    }

    int LuaGCCallBack(lua_State *L)
    {
        GetCppObjMapper()->FreeObj(L, -1);
        return 0;
    }

    int ArrayLengthCallBack(lua_State *L)
    {
        void *ptr = xlua::GetCppObjMapper()->GetCSharpObj(L, 1);
        if (ptr)
        {
            Il2CppArray *array = reinterpret_cast<Il2CppArray *>(ptr);
            lapi_lua_pushnumber(L, array->max_length);
            return 1;
        }
        else
        {
            throw_exception2lua(L, "invalid obj for ArrayLengthCallBack");
            return 0;
        }
    }

    Il2CppDelegate *DelegateMiddleware_CreateDelegate(RuntimeObject *obj, Il2CppReflectionType *reflectType)
    {
        DelegateMiddlerware *middleware = (DelegateMiddlerware *)Object::Unbox(obj);
        if (middleware->FuncPtr)
        {
            auto delegate = FunctionPointerToDelegate(middleware->FuncPtr, il2cpp_codegen_class_from_type(reflectType->type), obj);
            if (MethodIsStatic(delegate->method))
            {
                return nullptr;
            }
            return delegate;
        }
    }

    /// @brief 释放C#持有的lua引用
    void DelegateMiddleware_ReleaseLuaRef(RuntimeObject *obj)
    {
        DelegateMiddlerware *middleware = (DelegateMiddlerware *)Object::Unbox(obj);
        if (middleware->reference != -1 && middleware->L)
        {
            auto L = middleware->L;
            lapi_lua_pushnil(L);
            lapi_xlua_rawseti(L, lapi_xlua_get_registry_index(), middleware->reference);
            middleware->L = nullptr;
            middleware->reference = -1;
            middleware->FuncPtr = nullptr;
        }
    }

    Il2CppObject *GetObjectFromLua(lua_State *L, int index, const Il2CppReflectionType *type)
    {
        Il2CppClass *klass = il2cpp_codegen_class_from_type(type->type);
        if (klass)
        {
            return LuaValueToCSRef(klass, L, index);
        }

        return nullptr;
    }

    void PushAnyToLua(lua_State *L, Il2CppObject *obj)
    {
        CSAnyToLuaValue(L, obj);
    }


}

// namespace end

#ifdef __cplusplus
extern "C"
{
#endif

    int InitXluaIl2Cpp(lua_State *L)
    {

        return 0;
    }

    void SetXLuaRef(Il2CppArray *refArr)
    {
        int32_t *cache_ref = il2cpp_array_addr(refArr, int32_t, 0);
        xlua::GetCppObjMapper()->SetCacheRef(*cache_ref);
        int32_t *num2 = il2cpp_array_addr(refArr, int32_t, 1);
        xlua::errorFunc_ref = *num2;
    }

    void InitialXLua_IL2CPP(lapi_func_ptr *func_array, lua_State *L)
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
        InternalCalls::Add("XLua.IL2CPP.DelegateMiddleware::CreateDelegate(System.Type)", (Il2CppMethodPointer)xlua::DelegateMiddleware_CreateDelegate);
        InternalCalls::Add("XLua.IL2CPP.NativeAPI::SetGlobalType_LuaObject(System.Type)", (Il2CppMethodPointer)xlua::SetGlobalType_LuaObject);
        InternalCalls::Add("XLua.IL2CPP.NativeAPI::SetXLuaRef(System.Int32[])", (Il2CppMethodPointer)SetXLuaRef);
        InternalCalls::Add("XLua.IL2CPP.NativeAPI::ReleaseCSharpTypeInfo(System.IntPtr)", (Il2CppMethodPointer)xlua::ReleaseCSharpTypeInfo);
        InternalCalls::Add("XLua.IL2CPP.NativeAPI::GetObjectFromLua(System.IntPtr,System.Int32,System.Type)", (Il2CppMethodPointer)xlua::GetObjectFromLua);
        InternalCalls::Add("XLua.IL2CPP.NativeAPI::PushAnyToLua(System.IntPtr,System.Object)", (Il2CppMethodPointer)xlua::PushAnyToLua);
        lapi_init(func_array);
    }

    void SetLogCallback(xlua::LogCallback log)
    {
        xlua::SetLogHandler(log);
    }

    int DelegateCallBack(lua_State *L)
    {
        void *ptr = xlua::GetCppObjMapper()->GetCSharpObj(L, 1);
        if (ptr)
        {
            Il2CppDelegate *obj = reinterpret_cast<Il2CppDelegate *>(ptr);
            auto clsInfo = xlua::GetLuaClassRegister()->GetOrLoadLuaClsInfoByTypeId(obj->object.klass, L);
            if (clsInfo)
            {
                return clsInfo->DelegateWrapBridge(L, obj, (const MethodInfo *)obj->method);
            }
            else
            {
                return xlua::throw_exception2lua(L, "obj is not delegate obj");
            }
            return 0;
        }
        else
        {
            return xlua::throw_exception2lua(L, "obj is not cpp obj");
        }
    }

    int getDelegateLen(Il2CppMulticastDelegate *d)
    {
        if (!d)
        {
            return 0;
        }
        if (d->delegates && d->delegates->max_length > 0)
        {
            return d->delegates->max_length;
        }
        return 1;
    }

    int DelegateCombineCallBack(lua_State *L)
    {
        Il2CppClass *klass = nullptr;
        if (lapi_lua_isuserdata(L, 1))
        {
            auto ptr = xlua::GetCppObjMapper()->GetCSharpObj(L, 1);
            if (ptr)
            {

                klass = (Il2CppClass *)*reinterpret_cast<void **>(ptr);
            }
        }
        else if (lapi_lua_isuserdata(L, 2))
        {
            auto ptr = xlua::GetCppObjMapper()->GetCSharpObj(L, 2);
            if (ptr)
            {
                klass = (Il2CppClass *)*reinterpret_cast<void **>(ptr);
            }
        }
        if (!klass)
        {
            xlua::throw_exception2lua(L, "params wrong for delegate combine");
            return 0;
        }
        void *delegate1 = xlua::LuaValueToCSRef(klass, L, 1);
        void *delegate2 = xlua::LuaValueToCSRef(klass, L, 2);
        if (delegate1 && delegate2)
        {
            auto ptr = xlua::DelegateCombineMethodPointer((Il2CppDelegate *)delegate1, (Il2CppDelegate *)delegate2, (MethodInfo *)xlua::DelegateCombineMethodInfo);
            xlua::GetCppObjMapper()->TryPushObject(L, (Il2CppObject *)ptr);
            return 1;
        }
        else
        {
            return xlua::throw_exception2lua(L, "params wrong for delegate combine");
        }
    }

    int DelegateRemoveCallBack(lua_State *L)
    {
        void *delegate1 = xlua::LuaValueToCSRef(il2cpp_defaults.delegate_class, L, 1);
        if (!delegate1)
        {
            return xlua::throw_exception2lua(L, "#1 parameter must be a delegate");
        }
        Il2CppClass *klass = ((Il2CppObject *)delegate1)->klass;
        if (!klass)
        {
            return xlua::throw_exception2lua(L, "#1 parameter must be a delegate error 2");
        }
        auto delegate2 = xlua::LuaValueToCSRef(klass, L, 2);
        if (!delegate2)
        {
            return xlua::throw_exception2lua(L, "#2 parameter must be a delegate or a function ");
        }
        auto ptr = xlua::DelegateRemoveMethodPointer((Il2CppDelegate *)delegate1, (Il2CppDelegate *)delegate2, (MethodInfo *)xlua::DelegateRemoveMethodInfo);
        xlua::GetCppObjMapper()->TryPushObject(L, (Il2CppObject *)ptr);
        return 1;
    }

    void HandleDeleagateMetatable(lua_State *L, int metatable_idx, Il2CppClass *typeId)
    {
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
    static std::map<const char *, const char *, xlua::cmp_str> supportOp = {
        {"op_Addition", "__add"},
        {"op_Subtraction", "__sub"},
        {"op_Multiply", "__mul"},
        {"op_Division", "__div"},
        {"op_Equality", "__eq"},
        {"op_UnaryNegation", "__unm"},
        {"op_LessThan", "__lt"},
        {"op_LessThanOrEqual", "__le"},
        {"op_Modulus", "__mod"},
        {"op_BitwiseAnd", "__band"},
        {"op_BitwiseOr", "__bor"},
        {"op_ExclusiveOr", "__bxor"},
        {"op_OnesComplement", "__bnot"},
        {"op_LeftShift", "__shl"},
        {"op_RightShift", "__shr"},
    };
    void InitObjOperation(lua_State *L, int objMetaIdx, Il2CppClass *klass)
    {
        auto clsInfo = xlua::GetLuaClassRegister()->GetLuaClsInfoByTypeId((void *)klass);
        if (clsInfo)
        {
            for (auto &method : clsInfo->Methods)
            {
                auto iter = supportOp.find(method.Name.c_str());

                if (iter != supportOp.end())
                {
                    auto key = iter->second;
                    lapi_lua_pushstring(L, key);
                    xlua::WrapData **p = method.OverloadDatas.data();
                    lapi_lua_pushlightuserdata(L, p);
                    lapi_lua_pushcclosure(
                        L, [](lua_State *L)
                        { return xlua::MethodCallbackLuaWrap(L, 1); },
                        1);
                    lapi_lua_rawset(L, objMetaIdx);
                }
            }
        }
    }

    void InitObjMetatable_Hash(lua_State* L, int metatable_idx, xlua::LuaClassInfo* clsInfo)
    {
        auto klass = clsInfo->klass;
        lapi_lua_pushstring(L, "__index");

        lapi_lua_pushlightuserdata(L, clsInfo);
        lapi_lua_pushcclosure(L, xlua::ObjGetCallBack_Hash, 1);
        lapi_lua_settable(L, metatable_idx);

        lapi_lua_pushstring(L, "__newindex");
        lapi_lua_pushlightuserdata(L, clsInfo);
        lapi_lua_createtable(L, 0, 0);
        lapi_lua_pushcclosure(L, xlua::ObjSetCallBack, 2);
        lapi_lua_settable(L, metatable_idx);

        if (!klass->valuetype)
        { // 只有引用类型的需要GC
            lapi_lua_pushstring(L, "__gc");
            lapi_lua_pushcclosure(L, xlua::LuaGCCallBack, 0);
            lapi_lua_settable(L, metatable_idx);
            // todo valueType 考虑cache
        }

        InitObjOperation(L, metatable_idx, klass);

        if (IsArray(klass))
        {
            lapi_lua_pushstring(L, "__len");
            lapi_lua_pushcclosure(L, xlua::ArrayLengthCallBack, 0);
            lapi_lua_settable(L, metatable_idx);
            lapi_lua_pushstring(L, "Length");
            lapi_lua_pushcclosure(L, xlua::ArrayLengthCallBack, 0);
            lapi_lua_settable(L, metatable_idx);
        }
    }

    void InitObjMetatable_Map(lua_State* L, int metatable_idx, xlua::LuaClassInfo* clsInfo)
    {
        lapi_lua_pushstring(L, "__index");

        lapi_lua_pushlightuserdata(L, clsInfo);
        lapi_lua_createtable(L, 0, 0);
        lapi_lua_pushcclosure(L, xlua::ObjGetCallBack, 2);
        lapi_lua_settable(L, metatable_idx);

        lapi_lua_pushstring(L, "__newindex");
        lapi_lua_pushlightuserdata(L, clsInfo);
        lapi_lua_createtable(L, 0, 0);
        lapi_lua_pushcclosure(L, xlua::ObjSetCallBack, 2);
        lapi_lua_settable(L, metatable_idx);
        auto klass = clsInfo->klass;
        if (!klass->valuetype)
        { // 只有引用类型的需要GC
            lapi_lua_pushstring(L, "__gc");
            lapi_lua_pushcclosure(L, xlua::LuaGCCallBack, 0);
            lapi_lua_settable(L, metatable_idx);
            // todo valueType 考虑cache
        }

        InitObjOperation(L, metatable_idx, klass);

        if (IsArray(klass))
        {
            lapi_lua_pushstring(L, "__len");
            lapi_lua_pushcclosure(L, xlua::ArrayLengthCallBack, 0);
            lapi_lua_settable(L, metatable_idx);
            lapi_lua_pushstring(L, "Length");
            lapi_lua_pushcclosure(L, xlua::ArrayLengthCallBack, 0);
            lapi_lua_settable(L, metatable_idx);
        }
    }

    void HandleObjMetatable(lua_State *L, int metatable_idx, Il2CppClass *klass)
    {
        auto top = lapi_lua_gettop(L);
        auto clsInfo = xlua::GetLuaClassRegister()->GetLuaClsInfoByTypeId(klass);
        if (clsInfo->memberHash)
        {
            InitObjMetatable_Hash(L, metatable_idx, clsInfo);
        }
        else
        {
            InitObjMetatable_Map(L, metatable_idx, clsInfo);
        }
        
    }

    void HandleClsMetaTable(lua_State *L, int clsmeta_idx, Il2CppClass *klass)
    {
        auto clsInfo = xlua::GetLuaClassRegister()->GetLuaClsInfoByTypeId(klass);
        lapi_lua_pushstring(L, "__call");
        lapi_lua_pushlightuserdata(L, clsInfo);
        lapi_lua_pushcclosure(L, xlua::ClsConstructorCallBack, 1);
        lapi_lua_settable(L, clsmeta_idx);

        lapi_lua_pushstring(L, "__index");
        lapi_lua_pushlightuserdata(L, clsInfo);
        lapi_lua_pushcclosure(L, xlua::ClsGetCallBack, 1);
        lapi_lua_settable(L, clsmeta_idx);

        lapi_lua_pushstring(L, "__newindex");
        lapi_lua_pushlightuserdata(L, clsInfo);
        lapi_lua_pushcclosure(L, xlua::ClsSetCallBack, 1);
        lapi_lua_settable(L, clsmeta_idx);

        lapi_lua_pushstring(L, "UnderlyingSystemType");
        auto reflectType = xlua::TypeIdToType(klass);
        xlua::GetCppObjMapper()->TryPushObject(L, (Il2CppObject *)reflectType);
        lapi_lua_rawset(L, clsmeta_idx - 1);
    }

    int RegisterLuaClass(xlua::LuaClassInfo *luaClsInfo, lua_State *L, const char *fullName)
    {
        return xlua::GetLuaClassRegister()->RegisterClass(luaClsInfo);
    }

    void UnRegisterLuaClass(Il2CppString *ilstring)
    {
        const Il2CppChar *utf16 = il2cpp::utils::StringUtils::GetChars(ilstring);
        auto name = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
        xlua::GetLuaClassRegister()->UnRegisterClass(name);
    }

    void SetClassMetaId(void *ilclass, int metaId)
    {
        xlua::GetLuaClassRegister()->SetTypeId(ilclass, metaId);
    }

#ifdef __cplusplus
}
#endif