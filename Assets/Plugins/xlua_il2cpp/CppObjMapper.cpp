#pragma once
#include "unordered_map"
#include <il2cpp-api-types.h>
#include "lua_api_adapt.h"
#include "xlua_il2cpp_def.h"
#include "CppObjMapper.h"
#include "LuaClassRegister.h"
#include <vm/Exception.h>
#include "Il2CppTools.h"
#include "vm/Object.h"
using namespace il2cpp::vm;

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

int32_t CppObjMapper::AddToPool(lua_State* L, Il2CppObject* obj ) {
    int32_t idx = addObjFunc(L, obj, addObjReflectionMethod);
    return idx;
}

Il2CppObject* CppObjMapper::RemoveFromPool(lua_State* L, int index) {
    auto obj = removeObjFunc(L,index, removeObjReflectionMethod);
    return obj;
}


void* GetCSharpStructPointer(lua_State* L, int index) {
    auto css = lapi_xlua_tocss(L, index);
    if (css && css->fake_id == -1) {
        return &css->data[0];
    }

    void* ptr = lapi_xlua_getcsobj_ptr(L, index);

    if (ptr) {
        //以装箱的形式传递到lua的struct
        Il2CppClass* klass = (Il2CppClass*)*reinterpret_cast<void**>(ptr);
        if (klass->valuetype) {
            return Object::Unbox((Il2CppObject*)ptr);
        }
    }


    return nullptr;
}


/// 包含引用类型和值类型 引用类型返回指针，结构体返回存在userData内指针
void* CppObjMapper::ToCppObj(lua_State *L, int index){
    auto ptr = lapi_xlua_getcsobj_ptr(L, index);
    if(ptr){
        return ptr;
    }
    auto css = lapi_xlua_tocss(L, index);
    if (css && css->fake_id == -1) {
        char* data = &css->data[0];
        data -= sizeof(RuntimeObject);
        return data;
    }

    return nullptr;
}

/// 包含引用类型和值类型 引用类型返回指针，结构体返回存在userData内指针
void* CppObjMapper::ToCppObj_Field(lua_State* L, int index) {
    auto ptr = lapi_xlua_getcsobj_ptr(L, index);
    if (ptr) {
        Il2CppClass* klass = (Il2CppClass*)*reinterpret_cast<void**>(ptr);
        if (klass->valuetype) {
            return Object::Unbox((Il2CppObject*)ptr);
        }
        else {
            return ptr;
        }
        
    }

    return GetCSharpStructPointer(L, index);
}



bool CppObjMapper::TryPushStruct(lua_State *L, Il2CppClass* klass, void* pointer, unsigned int size){
    if (!klass->has_references) {

        int32_t metaId = xlua::GetLuaClassRegister()->GetTypeIdByIl2cppClass(L, (Il2CppClass*)klass);
        // xlua::GLogFormatted("CppObjMapper::TryPushStruct %p size %d  metaId %d  ", pointer, size, metaId);
        if (metaId != -1){
            lapi_xlua_pushstruct_pointer(L, size, pointer, metaId, klass);
            return true;
        }else{
            //#TODO@benp throw exception
        }
    }
    else {
        Il2CppObject* obj = il2cpp::vm::Object::Box(klass, pointer);
        CppObjMapper::TryPushObject(L, obj);
    }

    return false;
}

bool CppObjMapper::TryPushObject(lua_State *L, void * obj){
    if (!obj) {
        lapi_lua_pushnil(L);
        return true;
    }
    auto iter = objCache.find(obj);
    if(cacheRef == 0){
        return false;
    }
    int32_t key = objCache.size();
    if(iter != objCache.end()){
        //已经有了一份了
        key = iter->second;
        // xlua::GLogFormatted("find cache obj %d", key);
        if (lapi_xlua_tryget_cachedud(L, key, cacheRef) == 1)
        {
            // xlua::GLogFormatted("put cache obj %d success", key);
            return true;
        }
    }
    else {
        objCache[obj] = key;
    }
    
    Il2CppClass* kclass = (Il2CppClass*)*reinterpret_cast<void**>(obj);
    int32_t metaId = xlua::GetLuaClassRegister()->GetTypeIdByIl2cppClass(L, kclass);;
    
    if(metaId != -1){
        auto poolIdx = AddToPool(L, (Il2CppObject*)obj);

        // xlua::GLogFormatted("lapi_xlua_pushcsobj_ptr %s %p  metaId %d key %d poolIdx %d ", kclass->name, obj, metaId, key, cacheRef, poolIdx);
        lapi_xlua_pushcsobj_ptr(L, obj, metaId, key, 1, cacheRef, poolIdx);
        return true;
    }else{
        return false;
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
