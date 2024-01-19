// #include "LuaClassRegister.h"
// #include <map>
// #include <utils/StringUtils.h>
// namespace xlua
// {
//     class LuaClassRegister
//     {
//     public:
//         LuaClassRegister(/* args */);
//         ~LuaClassRegister();

//         LuaClassDefinition *GetLuaDefByTypeId(const void *typeId);

//         void RegisterClass( LuaClassInfo *luaClsInfo);
//         void UnRegisterClass( std::string name);
    
//     private:
//         std::map<std::string, LuaClassDefinition*> clsName2ClsInfo;
//         std::map<const void*, LuaClassDefinition*> clsId2ClsDef;
//     };
    
//     LuaClassRegister::LuaClassRegister(/* args */)
//     {
//     }
    
//     LuaClassRegister::~LuaClassRegister()
//     {
//         //#TODO@benp 清理
//     }

//     LuaClassDefinition* LuaClassRegister::GetLuaDefByTypeId(const void* typeId)
//     {
//         auto Iter = clsId2ClsDef.find(typeId);
//         if (Iter == clsId2ClsDef.end())
//         {
//             return nullptr;
//         }
//         else
//         {
//             return Iter->second;
//         }
//     }

//     void LuaClassRegister::RegisterClass(LuaClassInfo *luaClsInfo)
//     {
//         xlua::GLog(luaClsInfo->Name.c_str());
//         if(luaClsInfo->TypeId){
//             auto iter = clsId2ClsDef.find(luaClsInfo->TypeId);
//             if(iter != clsId2ClsDef.end()){

//                 LuaClassDefinition* def = new LuaClassDefinition;
//                 def->TypeId = luaClsInfo->TypeId;
//                 def->clsInfo = luaClsInfo;
                
//                 //#TODO@benp 初始化
//                 clsName2ClsInfo[luaClsInfo->Name] = def;
//                 clsId2ClsDef[luaClsInfo->TypeId] = def;
//             }
//         }
//     }

//     void LuaClassRegister::UnRegisterClass(std::string name){
//         //#TODO@benp 清理
//     }

//     LuaClassRegister* GetLuaClassRegister()
//     {
//         static LuaClassRegister S_LuaClassRegister;
//         return &S_LuaClassRegister;
//     }

//     void RegisterLuaClass(xlua::LuaClassInfo *luaClsInfo)
//     {
//         xlua::GetLuaClassRegister()->RegisterClass(luaClsInfo);
//     }

//     void UnRegisterLuaClass(Il2CppString* ilstring){
//         const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(ilstring);
//         auto name = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
//         xlua::GetLuaClassRegister()->UnRegisterClass(name);
//     }

//     xlua::LuaClassDefinition *GetLuaDefinitionByTypeId(const void *typeId)
//     {
//         return xlua::GetLuaClassRegister()->GetLuaDefByTypeId(typeId);
//     }
// } 

