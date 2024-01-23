#pragma once
#include <string>
#include <vector>
#include <map>
#include <cstdarg>
#include "UnityExports4Lua.h"
namespace xlua{

    struct CSharpMethodInfo
    {
        std::string Name;
        bool IsStatic;
        bool IsGetter;
        bool IsSetter;
        std::vector<WrapData*> OverloadDatas;
    };

    struct FieldWrapData
    {
        FieldWrapFuncPtr Getter;
        FieldWrapFuncPtr Setter;
        void *FieldInfo;
        size_t Offset;
        void* TypeInfo;
    };

    struct CSharpFieldInfo
    {
        std::string Name;
        bool IsStatic;
        FieldWrapData *Data;
    };

    struct LuaClassInfo 
    {
        const void* TypeId;
        const void* SuperTypeId;
        TypeIdType* Class;
        bool IsValueType;
        MethodPointer DelegateBridge;
        WrapData** CtorWrapDatas;
        std::string Name;
        std::vector<WrapData*> Ctors;
        std::vector<CSharpMethodInfo> Methods;
        std::vector<CSharpFieldInfo> Fields;
    };

    struct LuaClassDefinition
    {
        const void* TypeId;
        const void* SuperTypeId;
        std::string Name;
        std::vector<WrapData*> Ctors;
        std::map<std::string, CSharpMethodInfo*> MethodsMap;
        std::map<std::string, CSharpFieldInfo>  FieldMap;
        LuaClassInfo * clsInfo;
    };

        /// @brief UnityLog
    typedef void(*LogCallback)(const char* value);
    
    void SetLogHandler(LogCallback log);
    void GLogFormatted(const char* format, ...);
    
    
}