#include "LuaClassRegister.h"
#include <map>
#include <utils/StringUtils.h>
#include "vm/Reflection.h"
#include "CppObjMapper.h"
#include "ClassLuaCallCS_instance.h"
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
                CSharpGetTypeId(L, type);
                clsInfo = GetLuaClsInfoByTypeId(klass);
            }
        }
        return clsInfo;

    }

    int32_t LuaClassRegister::GetTypeIdByIl2cppClass(lua_State *L, const Il2CppClass *klass){   
        auto metaId = GetClassMetaId((void*)klass);
        if(metaId == -1){
            auto reflectType = il2cpp::vm::Reflection::GetTypeObject(&klass->byval_arg);
            if(reflectType){
                return CSharpGetTypeId(L, reflectType);
            }
        }
        return metaId;
    }
    

    int LuaClassRegister::CSharpGetTypeId(lua_State *L, Il2CppReflectionType* reflectionType) {
        if (cSharpGetTypeMethodPtr) {
             int typeId = cSharpGetTypeMethodPtr(L, reflectionType, cSharpGetTypeMethod);
             return typeId;
        }
        else {
            xlua::GLogFormatted("[error] cSharpGetTypeMethodPtr hasn't register");
        }
        return -1;
    }

    void LuaClassRegister::SetGetTypeIdFuncPtr(CSharpGetTypeIdFunc methodPtr, void* method) {
        cSharpGetTypeMethodPtr = methodPtr;
        cSharpGetTypeMethod = method;
    }


    int LuaClassRegister::RegisterClass(LuaClassInfo *luaClsInfo)
    {
        auto iter = clsId2ClsDef.find(luaClsInfo->klass);
        if(iter == clsId2ClsDef.end()){
            bool hasHash = false;
            if(strcmp(luaClsInfo->Name.c_str(), "ClassLuaCallCS") == 0 ){
                luaClsInfo->memberHash = (MemberHash)ClassLuaCallCS_instance_hash;
                luaClsInfo->memberWarpDatas = new MemberWrapData[GetClassLuaCallCS_instance_MaxLength()];
                hasHash = true;
            }
            for(auto& method: luaClsInfo->Methods){
                method.OverloadDatas.push_back(nullptr);
                if (method.IsStatic && !method.IsGetter && !method.IsSetter) {
                    // luaClsInfo->StaticMethodsMap[method.Name.c_str()] = &method;
                    luaClsInfo->ClsGetMap.insert(std::pair<const char *, MemberWrapData>(method.Name.c_str(), {method.OverloadDatas.data(), XLUA_MemberType_Method}));
                }
                else if(!method.IsGetter && !method.IsSetter){
                    // luaClsInfo->MethodsMap[method.Name.c_str()] = &method;
                    luaClsInfo->ObjGetMap.insert(std::pair<const char *, MemberWrapData>(method.Name.c_str(), {method.OverloadDatas.data(), XLUA_MemberType_Method}));
                    unsigned int hash = ClassLuaCallCS_instance_hash(method.Name.c_str(), method.Name.length());
                    if(hasHash){
                        luaClsInfo->memberWarpDatas[hash].data = method.OverloadDatas.data();
                        luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Method;
                    }
                }
            }

            for(auto& field: luaClsInfo->Fields){
                // luaClsInfo->FieldMap[field.Name.c_str()] = &field;
                if(field.IsStatic){
                    if(field.Data->Getter){
                        luaClsInfo->ClsGetMap.insert(std::pair<const char *, MemberWrapData>(field.Name.c_str(), {field.Data, XLUA_MemberType_Field}));
                    }
                    if(field.Data->Setter){
                        luaClsInfo->ClsSetMap.insert(std::pair<const char *, MemberWrapData>(field.Name.c_str(), {field.Data, XLUA_MemberType_Field}));
                    }
                }else{
                    if(field.Data->Getter){
                        luaClsInfo->ObjGetMap.insert(std::pair<const char *, MemberWrapData>(field.Name.c_str(), {field.Data, XLUA_MemberType_Field}));
                    }
                    if(field.Data->Setter){
                        luaClsInfo->ObjSetMap.insert(std::pair<const char *, MemberWrapData>(field.Name.c_str(), {field.Data, XLUA_MemberType_Field}));
                        if(hasHash){
                            unsigned int hash = ClassLuaCallCS_instance_hash(field.Name.c_str(), field.Name.length());
                            luaClsInfo->memberWarpDatas[hash].data = field.Data;
                            luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Field;
                        }
                    }
                }
            }

            for (auto& propertyInfo : luaClsInfo->Properties) {
                if (propertyInfo.Name == "Item") {
                    luaClsInfo->Indexer = &propertyInfo;
                }
                else {
                    if(propertyInfo.IsStatic){
                        if(propertyInfo.GetWrapData){
                            luaClsInfo->ClsGetMap.insert(std::pair<const char *, MemberWrapData>(propertyInfo.Name.c_str(), {propertyInfo.GetWrapData, XLUA_MemberType_Property}));
                        } 
                        if(propertyInfo.SetWrapData){
                            luaClsInfo->ClsSetMap.insert(std::pair<const char *, MemberWrapData>(propertyInfo.Name.c_str(), {propertyInfo.SetWrapData, XLUA_MemberType_Property}));
                        }
                    }else{
                        if(propertyInfo.GetWrapData){
                            luaClsInfo->ObjGetMap.insert(std::pair<const char *, MemberWrapData>(propertyInfo.Name.c_str(), {propertyInfo.GetWrapData, XLUA_MemberType_Property}));
                            if(hasHash){
                                unsigned int hash = ClassLuaCallCS_instance_hash(propertyInfo.Name.c_str(), propertyInfo.Name.length());
                                luaClsInfo->memberWarpDatas[hash].data = propertyInfo.GetWrapData;
                                luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Method;
                            }
                        } 
                        if(propertyInfo.SetWrapData){
                            luaClsInfo->ObjSetMap.insert(std::pair<const char *, MemberWrapData>(propertyInfo.Name.c_str(), {propertyInfo.SetWrapData, XLUA_MemberType_Property}));
                        }
                    }
                }
            }
            

            luaClsInfo->Ctors.push_back(nullptr);
            luaClsInfo->CtorWrapDatas = luaClsInfo->Ctors.data();
            clsId2ClsDef[luaClsInfo->klass] = luaClsInfo;
            clsName2ClsInfo[luaClsInfo->Name] = luaClsInfo;
            return 1;
        }else{
            
            return 0;
        }
    }

    void LuaClassRegister::UnRegisterClass(std::string name){
        //#TODO@benp 清理
    }

    void LuaClassRegister::SetTypeId(void* kclass, int32_t metaId){
        auto result = ilclass2luaMetaId.insert({kclass, metaId});
        if(result.second){
            // xlua::GLogFormatted("set type id insert success %p $d", kclass, metaId);
        }else{
            // xlua::GLogFormatted("set type id insert success %p $d", kclass, metaId);

        }
    }

    int32_t LuaClassRegister::GetClassMetaId(void* kclass){
        auto iter = ilclass2luaMetaId.find(kclass);
        if(iter != ilclass2luaMetaId.end()){
            return iter->second;
        }
        return -1;
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


} 

