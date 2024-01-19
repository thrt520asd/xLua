#pragma once
#include "unordered_map"
#include <il2cpp-api-types.h>
#include "lua_api_adpt.h"
#include "CppObjMapper.h"
#include <vm/Exception.h>

CppObjMapper::CppObjMapper(/* args */)
{
}

CppObjMapper::~CppObjMapper()
{
    
}

void CppObjMapper::SetTypeId(void* kclass, int32_t metaId){
    ilclass2luaMetaId[kclass] = metaId;
}

int32_t CppObjMapper::GetTypeId(void* kclass){
    auto iter = ilclass2luaMetaId.find(kclass);
    if(iter != ilclass2luaMetaId.end()){
        return iter->second;
    }

    return -1;
}
int cacheRef = 0;
void CppObjMapper::TryPushObject(lua_State *L, void * obj){
    auto iter = objCache.find(obj);
    if(cacheRef == 0){
        //#TODO@benp throw error
        return;
    }
    if(iter != objCache.end()){
        int32_t key = iter->second;
        if (lapi_xlua_tryget_cachedud(L, key, cacheRef) == 1)
        {
            return;
        }
    }
    int32_t key = objCache.size();
    objCache[obj] = key;
    void* kclass = *reinterpret_cast<void**>(obj);
    int32_t metaId = GetTypeId(kclass);
    if(metaId != -1){
        lapi_xlua_pushcsobj_ptr(L, obj, metaId, key, 1, cacheRef);
    }else{
        //#TODO@benp throw error
    }
}

CppObjMapper* GetCppObjMapper(){
    static CppObjMapper s_cppObjMapper;
    return &s_cppObjMapper;
}