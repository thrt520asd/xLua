#pragma once
#include "xlua_il2cpp_def.h"
#include "il2cpp-api.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
#include <string>
namespace xlua{

    class LuaClassRegister
    {
    public:
        LuaClassRegister(/* args */);
        ~LuaClassRegister();

        LuaClassInfo *GetLuaClsInfoByTypeId(const void *typeId);

        int RegisterClass( LuaClassInfo *luaClsInfo);
        void UnRegisterClass( std::string name);
        

    private:
        std::map<std::string, LuaClassInfo*> clsName2ClsInfo;
        std::map<const void*, LuaClassInfo*> clsId2ClsDef;
    };

    LuaClassRegister* GetLuaClassRegister();
    xlua::LuaClassInfo* GetLuaClsInfoByTypeId(const void* typeId);
    

}