#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <cstdarg>
#include "lua_api_adapt.h"
#include "codegen/il2cpp-codegen.h"
#include "il2cpp-api.h"
#include "il2cpp-api-types.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
using namespace std;
namespace xlua
{

// #define PropertyWrapCache 1
// #define FieldWrapCache 1
#define ENABLE_MEMBER_HASH 1
#define MethodPointer Il2CppMethodPointer

    typedef int (*WrapFuncPtr)(MethodInfo *method, Il2CppMethodPointer methodPointer, lua_State *L, bool checkArgument, struct WrapData *wrapData, int paramOffset);

    typedef void (*FieldWrapFuncPtr)(lua_State* L, FieldInfo *field, size_t offset, Il2CppClass *fieldType, void* klass);

    typedef int (*DelegateWrapFunc)(lua_State *L, Il2CppDelegate *ilDelegate, const MethodInfo *method);

    typedef Il2CppDelegate*((*DelegateCombineType))(Il2CppDelegate *d1, Il2CppDelegate *d2, MethodInfo *methodInfo);

    typedef Il2CppDelegate *((*DelegateRemoveType))(Il2CppDelegate *d1, Il2CppDelegate *d2, MethodInfo *methodInfo);


    typedef Il2CppString *((*Bytes2StringType))(void *bytes, const MethodInfo *methodInfo);

    typedef Il2CppObject *((*FallBackLua2CSObjType))(lua_State *L, int index, const Il2CppReflectionType* reflectType, const MethodInfo *methodInfo);

    typedef Il2CppDelegate *((*GetDelegateType))(lua_State *L, int index, const Il2CppReflectionType* reflectType, intptr_t methodPointer, const MethodInfo *methodInfo);

    typedef unsigned int (*MemberHash)(register const char*, register unsigned int len);

    typedef int (*CSharpGetTypeIdFunc)(lua_State* L, Il2CppReflectionType* type, void* method);

    

    struct DelegateMiddlerware
    {
        lua_State *L;
        Il2CppMethodPointer FuncPtr;
        int reference;
    };
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
    struct WrapData
    {
        WrapFuncPtr Wrap;
        MethodInfo *Method;
        void* klass;
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
    struct MemberWrapData
    {
        void* data;
        int type;
    };


    #define XLUA_UDTYPE_NONE 0
    #define XLUA_UDTYPE_OBJ 1
    #define XLUA_UDTYPE_STRUCT 2

    #define XLUA_MemberType_Method 1
    #define XLUA_MemberType_Property 2
    #define XLUA_MemberType_Field 3
    struct CStrEqual {
        bool operator()(const char* lhs, const char* rhs) const {
            return strcmp(lhs, rhs) == 0;
            }
    };

    struct CStrHash {
        std::size_t operator()(const char* s) const {
            // std::size_t hash = 0;
            // while (*s) {
            //     hash = hash * 101 + *s++;
            // }
            // return hash;
            int seed = 131;//31  131 1313 13131131313 etc//
            int hash = 0;
            while(*s)
            {
                hash = (hash * seed) + (*s++);
            }
            return hash & (0x7FFFFFFF);
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
        std::unordered_map<const char*, MemberWrapData, CStrHash, CStrEqual> ObjGetMap;
        std::unordered_map<const char*, MemberWrapData, CStrHash, CStrEqual> ObjSetMap;
        std::unordered_map<const char*, MemberWrapData, CStrHash, CStrEqual> ClsGetMap;
        std::unordered_map<const char*, MemberWrapData, CStrHash, CStrEqual> ClsSetMap;
        MemberHash memberHash;
        MemberWrapData* memberWarpDatas;
        int memberLength;
        PropertyWrapData *Indexer;

        ~ LuaClassInfo(){
            if(memberWarpDatas){
                delete[] memberWarpDatas;
                memberWarpDatas = nullptr;
                memberHash = nullptr;
            }

            for(auto& method: Methods){
                for(auto& wrapData: method.OverloadDatas){
                    if(wrapData){
                        free(wrapData); 
                    }
                }
                method.OverloadDatas.clear();
            }
            
            for(auto& wrapData: Ctors){
                if(wrapData){
                    free(wrapData);
                }
            }

            for(auto& field: Fields){
                delete field.Data;
                field.Data = nullptr;
            }

            for(auto& property: Properties){
                if(property.GetWrapData){
                    delete property.GetWrapData;
                    property.GetWrapData = nullptr;
                }
                if(property.SetWrapData){
                    delete property.SetWrapData;
                    property.SetWrapData = nullptr;
                }
            }
        }
    };

    struct ObjUD
    {
        int* tag;
        void* pointer;
    };

        // lua中的C#Struct
    struct StructUD
    {
        int* tag;
        void* typeId;
        unsigned int len;
        char data[1];
    };
    
    Il2CppObject* FallBackLua2CSObj(lua_State* L, int index, Il2CppClass* klass);
    void *LuaStr2CSharpString(lua_State *L, int index);
    void ReleaseCSharpTypeInfo(LuaClassInfo *clsInfo);
    int throw_exception2lua(lua_State* L, const char* msg);
    int throw_exception2lua_format(lua_State* L, const char* msg, ...);

    /// @brief UnityLog
    typedef void (*LogCallback)(const char *value);

    void SetLogHandler(LogCallback log);
    void GLogFormatted(const char *format, ...);
    void GWarnFormatted(const char* format, ...);
    void ErrorFormatted(const char* format, ...);

}