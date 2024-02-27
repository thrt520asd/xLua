#pragma once
#include "unordered_map"
#include <il2cpp-api-types.h>
#include "lua_api_adapt.h"
#include "xlua_il2cpp_def.h"
#include "CppObjMapper.h"
#include <vm/Exception.h>

CppObjMapper::CppObjMapper(/* args */)
{
}

CppObjMapper::~CppObjMapper()
{
    
}

void CppObjMapper::SetTypeId(void* kclass, int32_t metaId){
    auto result = ilclass2luaMetaId.insert({kclass, metaId});
    if(result.second){
        xlua::GLogFormatted("set type id insert success %p $d", kclass, metaId);
    }else{
        xlua::GLogFormatted("set type id insert success %p $d", kclass, metaId);

    }
}

int32_t CppObjMapper::GetTypeId(void* kclass){
    auto iter = ilclass2luaMetaId.find(kclass);
    if(iter != ilclass2luaMetaId.end()){
        return iter->second;
    }
    return -1;
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
    void* kclass = *reinterpret_cast<void**>(obj);
    int32_t metaId = GetTypeId(kclass);
    
    if(metaId != -1){
        //todo 释放引用
        //C#侧保持引用
        auto poolIdx = AddToPool((Il2CppObject*)obj);

        xlua::GLogFormatted("lapi_xlua_pushcsobj_ptr %p  metaId %d key %d cacheRef %d", obj, metaId, key, cacheRef);
        lapi_xlua_pushcsobj_ptr(L, obj, metaId, key, 1, cacheRef);
        return true;
    }else{
        return false;
        //#TODO@benp throw error
    }
}

void CppObjMapper::FreeObj(Il2CppObject* obj) {
    //todo free obj
}

CppObjMapper* GetCppObjMapper(){
    static CppObjMapper s_cppObjMapper;
    return &s_cppObjMapper;
}
