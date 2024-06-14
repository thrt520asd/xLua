#pragma once
#include "xlua_il2cpp_def.h"
#include "il2cpp-api.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
#include <string>
#include "unordered_map"

namespace xlua
{
    class LuaClassRegister
    {
    public:
        LuaClassRegister(/* args */);
        ~LuaClassRegister();

        LuaClassInfo *GetOrLoadLuaClsInfoByTypeId(const Il2CppClass *klass, lua_State *L);
        LuaClassInfo *GetLuaClsInfoByTypeId(const void *typeId);


        int RegisterClass(LuaClassInfo *luaClsInfo);
        WrapData** GetMemberWrapData(int typeId, int memberHash);
        EventWrapData* GetMemberEventWrapData(int typeId, int memberHash);
        vector<LuaClassInfo*> HashClsInfos;

    private:
        std::unordered_map<const void *, LuaClassInfo *> clsId2ClsDef;
        void RegisterClass_Hash(LuaClassInfo* luaClsInfo);
        // std::unordered_map<void *, int32_t> ilclass2luaMetaId;
    };

    LuaClassRegister *GetLuaClassRegister();

}