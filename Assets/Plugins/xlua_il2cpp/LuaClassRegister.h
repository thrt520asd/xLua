#pragma once
#include "xlua_il2cpp_def.h"
#include "il2cpp-api.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
#include <string>
#include "unordered_map"
typedef int (*CSharpGetTypeIdFunc)( lua_State *L, Il2CppReflectionType * type, void* method);
namespace xlua{


    class LuaClassRegister
    {
    public:
        LuaClassRegister(/* args */);
        ~LuaClassRegister();

        LuaClassInfo *GetOrLoadLuaClsInfoByTypeId(const Il2CppClass *klass, lua_State *L);
        int32_t GetTypeIdByIl2cppClass(lua_State *L, const Il2CppClass *klass);
        int CSharpGetTypeId(lua_State *L, Il2CppReflectionType *reflectionType);
        LuaClassInfo *GetLuaClsInfoByTypeId(const void *typeId);

        int RegisterClass( LuaClassInfo *luaClsInfo);
        void UnRegisterClass( std::string name);
        void SetGetTypeIdFuncPtr(CSharpGetTypeIdFunc methodPtr, void* method);
        void SetTypeId(void *kclass, int32_t metaId);
        int GetClassMetaId(void *kclass);

    private:
        CSharpGetTypeIdFunc cSharpGetTypeMethodPtr = nullptr;
        void* cSharpGetTypeMethod = nullptr;
        std::map<std::string, LuaClassInfo*> clsName2ClsInfo;
        std::map<const void*, LuaClassInfo*> clsId2ClsDef;
        std::unordered_map<void*, int32_t> ilclass2luaMetaId;
    };

    LuaClassRegister* GetLuaClassRegister();
    xlua::LuaClassInfo* GetLuaClsInfoByTypeId(const void* typeId);
    

}