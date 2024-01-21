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
    static LogCallback GLogCallback = nullptr;

    void GLogFormatted(const char* format, ...) {
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        va_list args;
        va_start(args, format);
        vsnprintf(buffer, BUFFER_SIZE, format, args);
        va_end(args);

        if(GLogCallback){
            GLogCallback(buffer);
        }
    }
}