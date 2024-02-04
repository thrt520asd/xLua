#pragma once
#include "xlua_il2cpp_def.h"
#include "il2cpp-api.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
#include <string>
typedef int (*CSharpGetTypeId)(void*, lua_State *L, Il2CppReflectionType * type, void* method);
namespace xlua{


    class LuaClassRegister
    {
    public:
        LuaClassRegister(/* args */);
        ~LuaClassRegister();

        LuaClassInfo *GetOrLoadLuaClsInfoByTypeId(const Il2CppClass *klass, lua_State *L);
        int CSharpRegister(lua_State* L, Il2CppReflectionType* reflectionType);
        LuaClassInfo *GetLuaClsInfoByTypeId(const void *typeId);

        int RegisterClass( LuaClassInfo *luaClsInfo);
        void UnRegisterClass( std::string name);
        void SetGetTypeIdFuncPtr(CSharpGetTypeId methodPtr, void* method);

    private:
        CSharpGetTypeId cSharpGetTypeMethodPtr = nullptr;
        void* cSharpGetTypeMethod = nullptr;
        std::map<std::string, LuaClassInfo*> clsName2ClsInfo;
        std::map<const void*, LuaClassInfo*> clsId2ClsDef;
    };

    LuaClassRegister* GetLuaClassRegister();
    xlua::LuaClassInfo* GetLuaClsInfoByTypeId(const void* typeId);
    

}