///LuaClassReigster 应该是全局唯一的  CppMapper是绑定LuaEnv的
#include "LuaClassRegister.h"
#include <map>
#include <utils/StringUtils.h>
#include "vm/Reflection.h"
#include "CppObjMapper.h"
#include "ClassLuaCallCS_instance.h"


// static int MethoCallBackHash_4_23(lua_State *L)
// {
//     WrapData **wrapData = GetLuaClassRegister()->GetMemberWrapData(4, 23);
//     if (wrapData)
//     {
//         return MethodCallback(L, wrapData, 2);
//     }
//     else
//     {
//         // #TODO@benp
//     }
//     return 0;
// }

namespace xlua
{
    #include "memberNameHash.h"
    LuaClassRegister::LuaClassRegister(/* args */)
    {
    }
    
    LuaClassRegister::~LuaClassRegister()
    {
        for(auto iter : clsId2ClsDef){
            iter.second->~LuaClassInfo();
        }
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
                 GetCppObjMapper()->CSharpGetTypeId(L, type);
                 clsInfo = GetLuaClsInfoByTypeId(klass);
             }
         }
         return clsInfo;

     }

    int LuaClassRegister::RegisterClass(LuaClassInfo *luaClsInfo)
    {
        auto iter = clsId2ClsDef.find(luaClsInfo->klass);
        if(iter == clsId2ClsDef.end()){
            bool hasHash = false;
            MemberHashType hashFunc = nullptr;
            //#TODO@benp find hash func
            for(auto& method: luaClsInfo->Methods){
                method.OverloadDatas.push_back(nullptr);
                if (method.IsStatic && !method.IsGetter && !method.IsSetter) {
                    // luaClsInfo->StaticMethodsMap[method.Name.c_str()] = &method;
                    luaClsInfo->ClsGetMap.insert(std::pair<const char *, MemberWrapData>(method.Name.c_str(), {method.OverloadDatas.data(), XLUA_MemberType_Method}));
                }
                else if(!method.IsGetter && !method.IsSetter){
                    // luaClsInfo->MethodsMap[method.Name.c_str()] = &method;
                    luaClsInfo->ObjGetMap.insert(std::pair<const char *, MemberWrapData>(method.Name.c_str(), {method.OverloadDatas.data(), XLUA_MemberType_Method}));
                    
                }
                if(!method.IsGetter && !method.IsSetter && hasHash){
                    unsigned int hash = hashFunc(method.Name.c_str(), method.Name.length());
                    luaClsInfo->memberWarpDatas[hash].data = method.OverloadDatas.data();
                    luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Method;
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
                        
                    }
                }
                if(hasHash){
                    unsigned int hash = hashFunc(field.Name.c_str(), field.Name.length());
                    luaClsInfo->memberWarpDatas[hash].data = &field;
                    luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Field;
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
                    if(hasHash){
                        unsigned int hash = hashFunc(propertyInfo.Name.c_str(), propertyInfo.Name.length());
                        luaClsInfo->memberWarpDatas[hash].data = &propertyInfo;
                        luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Property;
                    }
                }
            }
            
            for (auto& event : luaClsInfo->Events) {
                if(event.IsStatic){
                    luaClsInfo->ClsGetMap.insert(std::pair<const char *, MemberWrapData>(event.Name.c_str(), {&event, XLUA_MemberType_Event}));
                }else{
                    luaClsInfo->ObjGetMap.insert(std::pair<const char *, MemberWrapData>(event.Name.c_str(), {&event, XLUA_MemberType_Event}));
                }
                if(hasHash){
                    unsigned int hash = hashFunc(event.Name.c_str(), event.Name.length());
                    luaClsInfo->memberWarpDatas[hash].data = &event;
                    luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Event;
                }
            }

            luaClsInfo->Ctors.push_back(nullptr);
            luaClsInfo->CtorWrapDatas = luaClsInfo->Ctors.data();
            clsId2ClsDef[luaClsInfo->klass] = luaClsInfo;
            return 1;
        }else{
            
            return 0;
        }
    }


    WrapData** LuaClassRegister::GetMemberWrapData(int typeId, int memberHash){
        auto clsInfo = HashClsInfos[typeId];
        if(clsInfo){
            MemberWrapData wrapData = clsInfo->memberWarpDatas[memberHash];
            return (WrapData**)wrapData.data;
        }
        return nullptr;
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

} 

