#pragma once
#include "unordered_map"
#include <il2cpp-api-types.h>
#include "lua_api_adapt.h"
#include "xlua_il2cpp_def.h"
#include "CppObjMapper.h"
#include "LuaClassRegister.h"
#include <vm/Exception.h>
#include "Il2CppTools.h"
CppObjMapper::CppObjMapper(/* args */)
{
}

CppObjMapper::~CppObjMapper()
{
    
}


void CppObjMapper::SetCacheRef(int32_t cache_ref) {
    cacheRef = cache_ref;
}

void CppObjMapper::InitObjPoolMethod(Il2CppReflectionMethod* addObjMethod, Il2CppMethodPointer* addObjMethodPointer, Il2CppReflectionMethod* removeObjMethod, Il2CppMethodPointer* removeObjMethodPointer)
{
    addObjFunc = (AddObjFunc)addObjMethodPointer;
    addObjReflectionMethod = addObjMethod;
    removeObjFunc = (RemoveObjFunc)removeObjMethodPointer;
    removeObjReflectionMethod = removeObjMethod;
}

int32_t CppObjMapper::AddToPool(Il2CppObject* obj ) {
    int32_t idx = addObjFunc(obj, addObjReflectionMethod);
    return idx;
}

Il2CppObject* CppObjMapper::RemoveFromPool(int index) {
    auto obj = removeObjFunc(index, removeObjReflectionMethod);
    return obj;
}

void* GetCSharpStructPointerWithOffset(lua_State* L, int index, int offset) {
    auto css = lapi_xlua_tocss(L, index);
    if (css && css->fake_id == -1) {
        char* data = &css->data[0];
        data -= offset;
        return data;
    }
    return nullptr;
}

void* GetCSharpStructPointer(lua_State* L, int index) {
    auto css = lapi_xlua_tocss(L, index);
    if (css && css->fake_id == -1) {
        return &css->data[0];
    }
    return nullptr;
}


/// 包含引用类型和值类型 引用类型返回指针，结构体返回存在userData内指针
void* CppObjMapper::ToCppObj(lua_State *L, int index){
    auto ptr = lapi_xlua_getcsobj_ptr(L, index);
    if(ptr){
        return ptr;
    }
   
    return GetCSharpStructPointerWithOffset(L, index, sizeof(RuntimeObject));
}

/// 包含引用类型和值类型 引用类型返回指针，结构体返回存在userData内指针
void* CppObjMapper::ToCppObj_Field(lua_State* L, int index) {
    auto ptr = lapi_xlua_getcsobj_ptr(L, index);
    if (ptr) {
        return ptr;
    }

    return GetCSharpStructPointer(L, index);
}



bool CppObjMapper::TryPushStruct(lua_State *L, void* typeId, void* pointer, unsigned int size){
    int32_t metaId = xlua::GetLuaClassRegister()->GetTypeIdByIl2cppClass(L, (Il2CppClass*)typeId);
    xlua::GLogFormatted("CppObjMapper::TryPushStruct %p size %d  metaId %d  ", pointer, size, metaId);
    if (metaId != -1){
        lapi_xlua_pushstruct_pointer(L, size, pointer, metaId, typeId);
        return true;
    }else{
        //#TODO@benp throw exception
    }

    return false;
}

bool CppObjMapper::TryPushObject(lua_State *L, void * obj){
    auto iter = objCache.find(obj);
    if(cacheRef == 0){
        xlua::GLogFormatted("[error]please set cacheRef first");
        //#TODO@benp throw error
        return false;
    }
    if(iter != objCache.end()){
        int32_t key = iter->second;
        xlua::GLogFormatted("find cache obj %d", key);
        if (lapi_xlua_tryget_cachedud(L, key, cacheRef) == 1)
        {
            xlua::GLogFormatted("put cache obj %d success", key);
            return true;
        }
    }
    int32_t key = objCache.size();
    objCache[obj] = key;
    Il2CppClass* kclass = (Il2CppClass*)*reinterpret_cast<void**>(obj);
    int32_t metaId = -1;
    /*if(IsDelegate(kclass)){
        int a = 1;
    }else{*/
        metaId = xlua::GetLuaClassRegister()->GetTypeIdByIl2cppClass(L, kclass);
    //}
    
    
    if(metaId != -1){
        auto poolIdx = AddToPool((Il2CppObject*)obj);

        xlua::GLogFormatted("lapi_xlua_pushcsobj_ptr %p  metaId %d key %d cacheRef %d", obj, metaId, key, cacheRef);
        lapi_xlua_pushcsobj_ptr(L, obj, metaId, key, 1, cacheRef, poolIdx);
        return true;
    }else{
        return false;
        //#TODO@benp throw error
    }
}

void CppObjMapper::FreeObj(Il2CppObject* obj)
{
    //todo 清理obj
}



CppObjMapper* GetCppObjMapper(){
    static CppObjMapper s_cppObjMapper;
    return &s_cppObjMapper;
}
