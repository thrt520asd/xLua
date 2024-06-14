///LuaClassReigster 应该是全局唯一的  CppMapper是绑定LuaEnv的
#include "LuaClassRegister.h"
#include <map>
#include <utils/StringUtils.h>
#include "vm/Reflection.h"
#include "CppObjMapper.h"



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
	#include "HashWrap.h"
    LuaClassRegister::LuaClassRegister(/* args */)
    {
        HashClsInfos.resize(sizeof(s_classMemberHashFunc) / sizeof(ClassMemberHashFunc));
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

     int getClassId(const char* name) {
         auto iter = classMap.find(name);
         if (iter != classMap.end()) {
             return iter->second;
         }
         return -1;
     }

     int getMethodId(int clsId, int hashId) {
         return clsId * 100000 + hashId;
     }

     void LuaClassRegister::RegisterClass_Hash(LuaClassInfo * luaClsInfo) {
         int clsId = getClassId(luaClsInfo->Name.c_str());
         if (clsId >= 0) {
             HashClsInfos[clsId] = luaClsInfo;
             HashFuncs hashFuncs = s_classMemberHashFunc[clsId];
             ClassMemberHashFunc hashFunc = nullptr;
             hashFunc = hashFuncs.classMemberHashFunc;
             int hashCount = hashFuncs.HashLengthFunc();
             luaClsInfo->memberHash = hashFunc;
             luaClsInfo->memberWarpDatas = new MemberWrapData[hashCount]();
             luaClsInfo->hashMemberCount = hashCount;
             GWarnFormatted("HashClsInfos id %d name %s length %d ", clsId, luaClsInfo->Name.c_str(), hashFuncs.HashLengthFunc());

             for (auto& method : luaClsInfo->Methods) {
                 if (!method.IsGetter && !method.IsSetter) {
                     unsigned int hash = hashFunc(method.Name.c_str(), method.Name.length());
                     if (hash < hashCount) {
                         luaClsInfo->memberWarpDatas[hash].data = &method;
                         luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Method;
                         int methodId = getMethodId(clsId, hash);
                         auto iter = s_HashWrapDic.find(methodId);
                         if (iter != s_HashWrapDic.end()) {
                             method.hashWrapFunc = iter->second;
                         }
                         else {
                             ErrorFormatted("memberWarpDatas find hash wrap func failed %d %s", hash, method.Name.c_str());
                         }
                     }
                     else {
                         ErrorFormatted("memberWarpDatas hash out of range %d %s", hash, method.Name.c_str());
                     }
                 }
             }

             for (auto& field : luaClsInfo->Fields) {
                unsigned int hash = hashFunc(field.Name.c_str(), field.Name.length());
                if (hash < hashCount) {
                    luaClsInfo->memberWarpDatas[hash].data = &field;
                    luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Field;
                    // GWarnFormatted("memberWarpDatas %d %s", hash, field.Name.c_str());
                }
                else {
                    ErrorFormatted("memberWarpDatas hash out of range %d %s", hash, field.Name.c_str());
                }
             }

             for (auto& propertyInfo : luaClsInfo->Properties) {
                unsigned int hash = hashFunc(propertyInfo.Name.c_str(), propertyInfo.Name.length());
                if (hash < hashCount) {
                    luaClsInfo->memberWarpDatas[hash].data = &propertyInfo;
                    luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Property;
                    // GWarnFormatted("memberWarpDatas %d %s", hash, propertyInfo.Name);
                }
                else {
                    ErrorFormatted("memberWarpDatas hash out of range %d %s", hash, propertyInfo.Name.c_str());
                }
             }

             for (auto& eventInfo : luaClsInfo->Events) {
                 unsigned int hash = hashFunc(eventInfo.Name.c_str(), eventInfo.Name.length());
                if (hash < hashCount) {
                    luaClsInfo->memberWarpDatas[hash].data = &eventInfo;
                    luaClsInfo->memberWarpDatas[hash].type = XLUA_MemberType_Event;
                    int methodId = getMethodId(clsId, hash);
                    auto iter = s_HashWrapDic.find(methodId);
                    if (iter != s_HashWrapDic.end()) {
                        eventInfo.hashWrapFunc = iter->second;
                    }
                    else {
                        ErrorFormatted("event find hash wrap func failed %d %s", hash, eventInfo.Name.c_str());
                    }
                    // GWarnFormatted("memberWarpDatas %d %s", hash, eventInfo.Name);
                }
                else {
                    ErrorFormatted("memberWarpDatas hash out of range %d %s", hash, eventInfo.Name.c_str());
                }
             }
         }
     }

    int LuaClassRegister::RegisterClass(LuaClassInfo *luaClsInfo)
    {   
        // GWarnFormatted("RegisterClass %s", luaClsInfo->Name.c_str());
        auto iter = clsId2ClsDef.find(luaClsInfo->klass);
        if(iter == clsId2ClsDef.end()){
            for(auto& method: luaClsInfo->Methods){
                method.OverloadDatas.push_back(nullptr);
                if (method.IsStatic && !method.IsGetter && !method.IsSetter) {
                    // luaClsInfo->StaticMethodsMap[method.Name.c_str()] = &method;
                    luaClsInfo->ClsGetMap.insert(std::pair<const char *, MemberWrapData>(method.Name.c_str(), {method.OverloadDatas.data(), XLUA_MemberType_Method}));
                }
                else if(!method.IsGetter && !method.IsSetter){
                    
                    luaClsInfo->ObjGetMap.insert(std::pair<const char *, MemberWrapData>(method.Name.c_str(), {method.OverloadDatas.data(), XLUA_MemberType_Method}));
                    
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
                        } 
                        if(propertyInfo.SetWrapData){
                            luaClsInfo->ObjSetMap.insert(std::pair<const char *, MemberWrapData>(propertyInfo.Name.c_str(), {propertyInfo.SetWrapData, XLUA_MemberType_Property}));
                        }
                    }
                }
            }
            
            for (auto& event : luaClsInfo->Events) {
                if(event.IsStatic){
                    luaClsInfo->ClsGetMap.insert(std::pair<const char *, MemberWrapData>(event.Name.c_str(), {&event, XLUA_MemberType_Event}));
                }else{
                    luaClsInfo->ObjGetMap.insert(std::pair<const char *, MemberWrapData>(event.Name.c_str(), {&event, XLUA_MemberType_Event}));
                }
            }

            luaClsInfo->Ctors.push_back(nullptr);
            luaClsInfo->CtorWrapDatas = luaClsInfo->Ctors.data();
            clsId2ClsDef[luaClsInfo->klass] = luaClsInfo;

            RegisterClass_Hash(luaClsInfo);

            // GWarnFormatted("Register finish");
            return 1;
        }else{
            
            return 0;
        }
    }


    WrapData** LuaClassRegister::GetMemberWrapData(int typeId, int memberHash){
        auto clsInfo = HashClsInfos[typeId];
        if(clsInfo && memberHash < clsInfo->hashMemberCount){
            MemberWrapData wrapData = clsInfo->memberWarpDatas[memberHash];
            if (wrapData.data) {
                CSharpMethodInfo* cSharpMethodInfo = (CSharpMethodInfo*)wrapData.data;
                return cSharpMethodInfo->OverloadDatas.data();
            }
        }
        return nullptr;
    }

    EventWrapData* LuaClassRegister::GetMemberEventWrapData(int typeId, int memberHash) {
        auto clsInfo = HashClsInfos[typeId];
        if (clsInfo && memberHash < clsInfo->hashMemberCount) {
            MemberWrapData wrapData = clsInfo->memberWarpDatas[memberHash];
            if (wrapData.data) {
                return (EventWrapData*)wrapData.data;
            }
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

