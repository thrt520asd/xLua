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
        //// 默认构造函数
        //CSharpMethodInfo() {
        //    a = 0;
        //}
        //// 拷贝构造函数
        //CSharpMethodInfo(const CSharpMethodInfo& other) {
        //    a = 1;
        //}

        //// 移动构造函数
        //CSharpMethodInfo(CSharpMethodInfo&& other) noexcept {
        //    a = 2;
        //}
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

    struct LuaClassInfo:LuaClassInfoHeader
    {
        std::string Name;
        std::vector<WrapData*> Ctors;
        std::vector<CSharpMethodInfo> Methods;
        std::vector<CSharpFieldInfo> Fields;
        std::map<std::string, CSharpMethodInfo*> MethodsMap;
        std::map<std::string, CSharpFieldInfo*>  FieldMap;
        int a;

    };

    struct LuaClassDefinition
    {
        const void* TypeId;
        const void* SuperTypeId;
        const char* Name;
        WrapData** CtorWrapDatas;
        
        std::vector<WrapData*> Ctors;
        // std::map<std::string, WrapData*> MethodsMap;
        // std::map<std::string, CSharpFieldInfo>  FieldMap;
        LuaClassInfo *clsInfo;
    };

        /// @brief UnityLog
    typedef void(*LogCallback)(const char* value);
    
    void SetLogHandler(LogCallback log);
    void GLogFormatted(const char* format, ...);
    
    
}