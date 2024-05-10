#pragma once
#include <string>
#include <vector>
#include <map>
#include <cstdarg>
#include "lua_api_adapt.h"
#include "codegen/il2cpp-codegen.h"
#include "il2cpp-api.h"
#include "il2cpp-api-types.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
namespace xlua
{

#define MethodPointer Il2CppMethodPointer

    typedef int (*WrapFuncPtr)(MethodInfo *method, Il2CppMethodPointer methodPointer, lua_State *L, bool checkArgument, struct WrapData *wrapData, int paramOffset);

    typedef void (*FieldWrapFuncPtr)(lua_State L, FieldInfo *field, size_t offset, Il2CppClass *fieldType);

    typedef int (*DelegateWrapFunc)(lua_State *L, Il2CppDelegate *ilDelegate, const MethodInfo *method);

    typedef void *((*DelegateCombineType))(Il2CppDelegate *d1, Il2CppDelegate *d2, MethodInfo *methodInfo);

    typedef Il2CppString *((*Bytes2StringType))(void *bytes, const MethodInfo *methodInfo);

    typedef Il2CppObject *((*FallBackLua2CSObjType))(lua_State *L, int index, const Il2CppReflectionType* reflectType, const MethodInfo *methodInfo);

    typedef Il2CppDelegate *((*GetDelegateType))(lua_State *L, int index, const Il2CppReflectionType* reflectType, intptr_t methodPointer, const MethodInfo *methodInfo);

    struct DelegateMiddlerware
    {
        lua_State *L;
        Il2CppMethodPointer FuncPtr;
        int reference;
    };

    struct WrapData
    {
        WrapFuncPtr Wrap;
        MethodInfo *Method;
        MethodPointer MethodPointer;
        bool IsStatic;
        bool IsExtensionMethod;
        bool HasParamArray;
        int OptionalNum;
        void *TypeInfos[0];
    };

    struct CSharpMethodInfo
    {
        std::string Name;
        bool IsStatic;
        bool IsGetter;
        bool IsSetter;
        std::vector<WrapData *> OverloadDatas;
    };

    struct FieldWrapData
    {
        FieldWrapFuncPtr Getter;
        FieldWrapFuncPtr Setter;
        void *FieldInfo;
        size_t Offset;
        void *TypeInfo;
    };

    struct CSharpFieldInfo
    {
        std::string Name;
        bool IsStatic;
        FieldWrapData *Data;
    };

    struct PropertyWrapData
    {
        std::string Name;
        bool IsStatic;
        WrapData *GetWrapData;
        WrapData *SetWrapData;
    };
    struct cmp_str
    {
        bool operator()(char const *a, char const *b) const
        {
            return std::strcmp(a, b) < 0;
        }
    };
    struct LuaClassInfo
    {
        const void *SuperTypeId;
        Il2CppClass *klass;
        MethodPointer DelegateBridge;
        DelegateWrapFunc DelegateWrapBridge;
        WrapData **CtorWrapDatas;
        std::string Name;
        std::vector<WrapData *> Ctors;
        std::vector<CSharpMethodInfo> Methods;
        std::vector<CSharpFieldInfo> Fields;
        std::vector<PropertyWrapData> Properties;
        std::map<const char*, CSharpMethodInfo *, cmp_str> MethodsMap;
        std::map<const char*, CSharpFieldInfo *, cmp_str> FieldMap;
        std::map<const char*, PropertyWrapData *, cmp_str> PropertyMap;
        std::map<const char*, CSharpMethodInfo*, cmp_str> StaticMethodsMap;
        PropertyWrapData *Indexer;
    };

    

    /// @brief UnityLog
    typedef void (*LogCallback)(const char *value);

    void SetLogHandler(LogCallback log);
    void GLogFormatted(const char *format, ...);
    void GWarnFormatted(const char* format, ...);

}