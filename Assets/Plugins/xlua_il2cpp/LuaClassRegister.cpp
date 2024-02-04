#include "LuaClassRegister.h"
#include <map>
#include <utils/StringUtils.h>
#include "vm/Reflection.h"
namespace xlua
{
    
    LuaClassRegister::LuaClassRegister(/* args */)
    {
    }
    
    LuaClassRegister::~LuaClassRegister()
    {
        //#TODO@benp 清理
    }

    LuaClassInfo* LuaClassRegister::GetLuaClsInfoByTypeId(const void* typeId)
    {
        auto Iter = clsId2ClsDef.find(typeId);
        if (Iter == clsId2ClsDef.end())
        {
            return nullptr;
        }
        else
        {
            return Iter->second;
        }
    }

    LuaClassInfo* LuaClassRegister::GetOrLoadLuaClsInfoByTypeId(const Il2CppClass *klass, lua_State *L){
        auto clsInfo = GetLuaClsInfoByTypeId(klass);
        if(!clsInfo){
            auto type = il2cpp::vm::Reflection::GetTypeObject(&klass->byval_arg);
            if(type){
                // call C# to register 
                if (CSharpRegister(L, type)) {
                    clsInfo = GetLuaClsInfoByTypeId(klass);
                }
            }
        }
        return clsInfo;

    }

    int LuaClassRegister::CSharpRegister(lua_State *L, Il2CppReflectionType* reflectionType) {
        if (cSharpGetTypeMethodPtr) {
             int typeId = cSharpGetTypeMethodPtr(nullptr, L, reflectionType, cSharpGetTypeMethod);
             return typeId;
        }
        else {
            xlua::GLogFormatted("[error] cSharpGetTypeMethodPtr hasn't register");
        }
        return 0;
    }

    void LuaClassRegister::SetGetTypeIdFuncPtr(CSharpGetTypeId methodPtr, void* method) {
        cSharpGetTypeMethodPtr = methodPtr;
        cSharpGetTypeMethod = method;
    }


    int LuaClassRegister::RegisterClass(LuaClassInfo *luaClsInfo)
    {
        xlua::GLogFormatted(luaClsInfo->Name.c_str());
        auto iter = clsId2ClsDef.find(luaClsInfo->TypeId);
        if(iter == clsId2ClsDef.end()){
            
            for(auto& method: luaClsInfo->Methods){

                method.OverloadDatas.push_back(nullptr);
                luaClsInfo->MethodsMap[method.Name] = &method;
            }

            for(auto& field: luaClsInfo->Fields){
                luaClsInfo->FieldMap[field.Name] = &field;
            }
            luaClsInfo->Ctors.push_back(nullptr);
            luaClsInfo->CtorWrapDatas = luaClsInfo->Ctors.data();
            clsId2ClsDef[luaClsInfo->TypeId] = luaClsInfo;
            clsName2ClsInfo[luaClsInfo->Name] = luaClsInfo;
            return 1;
        }else{
            //#TODO@benp throw error
            return 0;
        }
    }

    void LuaClassRegister::UnRegisterClass(std::string name){
        //#TODO@benp 清理
    }


    LuaClassRegister* GetLuaClassRegister()
    {
        static LuaClassRegister S_LuaClassRegister;
        return &S_LuaClassRegister;
    }

    void RegisterLuaClass(xlua::LuaClassInfo *luaClsInfo)
    {
        xlua::GetLuaClassRegister()->RegisterClass(luaClsInfo);
    }

    void UnRegisterLuaClass(Il2CppString* ilstring){
        const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(ilstring);
        auto name = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
        xlua::GetLuaClassRegister()->UnRegisterClass(name);
    }

    LuaClassInfo *GetLuaClsInfoByTypeId(const void *typeId)
    {
        return xlua::GetLuaClassRegister()->GetLuaClsInfoByTypeId(typeId);
    }

} 

