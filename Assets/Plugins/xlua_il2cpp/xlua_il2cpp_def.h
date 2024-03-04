#pragma once
#include <string>
#include <vector>
#include <map>
#include <cstdarg>
#include "UnityExports4Lua.h"
#include "xlua_il2cpp_def.h"
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

    struct PropertyWrapData 
    {
        std::string Name;
        bool IsStatic;
        WrapData* GetWrapData;
        WrapData* SetWrapData;
    };

    struct LuaClassInfo:LuaClassInfoHeader
    {
        std::string Name;
        std::vector<WrapData*> Ctors;
        std::vector<CSharpMethodInfo> Methods;
        std::vector<CSharpFieldInfo> Fields;
        std::vector<PropertyWrapData> Properties;
        std::map<std::string, CSharpMethodInfo*> MethodsMap;
        std::map<std::string, CSharpFieldInfo*>  FieldMap;
        std::map<std::string, PropertyWrapData*>  PropertyMap;
    };

    /// @brief UnityLog
    typedef void(*LogCallback)(const char* value);
    
    void SetLogHandler(LogCallback log);
    void GLogFormatted(const char* format, ...);
    
    
}