#include "lua_api_adapt.h"
#include "stddef.h"
EXTERN_C_START
//lua_touserdata
typedef void* (*lapi_lua_touserdataType)(lua_State*L, int idx);

static lapi_lua_touserdataType lapi_lua_touserdata_ptr;

void* lapi_lua_touserdata(lua_State*L, int idx){
    return lapi_lua_touserdata_ptr(L, idx);
}
//lua_type
typedef int (*lapi_lua_typeType)(lua_State*L, int idx);

static lapi_lua_typeType lapi_lua_type_ptr;

int lapi_lua_type(lua_State*L, int idx){
    return lapi_lua_type_ptr(L, idx);
}

//lua_gettop
typedef int (*lapi_lua_gettopType)(lua_State*L);

static lapi_lua_gettopType lapi_lua_gettop_ptr;

int lapi_lua_gettop(lua_State*L){
    return lapi_lua_gettop_ptr(L);
}

//lua_upvalueindex
typedef int (*lapi_lua_upvalueindexType)(int idx);

static lapi_lua_upvalueindexType lapi_lua_upvalueindex_ptr;

int lapi_lua_upvalueindex(int idx){
    return lapi_lua_upvalueindex_ptr(idx);
}

//xlua_pushcsobj_ptr
typedef void (*lapi_xlua_pushcsobj_ptrType)(lua_State*L, void* ptr, int meta_ref, int key, int need_cache, int cache_ref);

static lapi_xlua_pushcsobj_ptrType lapi_xlua_pushcsobj_ptr_ptr;

void lapi_xlua_pushcsobj_ptr(lua_State*L, void* ptr, int meta_ref, int key, int need_cache, int cache_ref){
    return lapi_xlua_pushcsobj_ptr_ptr(L, ptr, meta_ref, key, need_cache, cache_ref);
}



//lua_isnumber
typedef int (*lapi_lua_isnumberType)(lua_State*L, int idx);

static lapi_lua_isnumberType lapi_lua_isnumber_ptr;

int lapi_lua_isnumber(lua_State*L, int idx){
    return lapi_lua_isnumber_ptr(L, idx);
}

//lua_isstring
typedef int (*lapi_lua_isstringType)(lua_State*L, int idx);

static lapi_lua_isstringType lapi_lua_isstring_ptr;

int lapi_lua_isstring(lua_State*L, int idx){
    return lapi_lua_isstring_ptr(L, idx);
}

//lua_iscfunction
typedef int (*lapi_lua_iscfunctionType)(lua_State*L, int idx);

static lapi_lua_iscfunctionType lapi_lua_iscfunction_ptr;

int lapi_lua_iscfunction(lua_State*L, int idx){
    return lapi_lua_iscfunction_ptr(L, idx);
}


//lua_isinteger
typedef int (*lapi_lua_isintegerType)(lua_State*L, int idx);

static lapi_lua_isintegerType lapi_lua_isinteger_ptr;

int lapi_lua_isinteger(lua_State*L, int idx){
    return lapi_lua_isinteger_ptr(L, idx);
}

//lua_isuserdata
typedef int (*lapi_lua_isuserdataType)(lua_State*L, int idx);

static lapi_lua_isuserdataType lapi_lua_isuserdata_ptr;

int lapi_lua_isuserdata(lua_State*L, int idx){
    return lapi_lua_isuserdata_ptr(L, idx);
}

//lua_typename
typedef const char* (*lapi_lua_typenameType)(lua_State*L, int idx);

static lapi_lua_typenameType lapi_lua_typename_ptr;

const char * lapi_lua_typename(lua_State*L, int idx){
    return lapi_lua_typename_ptr(L, idx);
}

//lua_tonumber
typedef lua_Number (*lapi_lua_tonumberType)(lua_State*L, int idx);

static lapi_lua_tonumberType lapi_lua_tonumber_ptr;

lua_Number lapi_lua_tonumber(lua_State*L, int idx){
    return lapi_lua_tonumber_ptr(L, idx);
}

//lua_tolstring
typedef const char * (*lapi_lua_tolstringType)(lua_State*L, int idx, size_t *len);

static lapi_lua_tolstringType lapi_lua_tolstring_ptr;

const char * lapi_lua_tolstring(lua_State*L, int idx){
    return lapi_lua_tolstring_ptr(L, idx, NULL);
}

//lua_toboolean
typedef int (*lapi_lua_tobooleanType)(lua_State*L, int idx);

static lapi_lua_tobooleanType lapi_lua_toboolean_ptr;

int lapi_lua_toboolean(lua_State*L, int idx){
    return lapi_lua_toboolean_ptr(L, idx);
}

//lua_topointer
typedef const void * (*lapi_lua_topointerType)(lua_State*L, int idx);

static lapi_lua_topointerType lapi_lua_topointer_ptr;

const void * lapi_lua_topointer(lua_State*L, int idx){
    return lapi_lua_topointer_ptr(L, idx);
}

//xlua_tryget_cachedud
typedef int (*lapi_xlua_tryget_cachedudType)(lua_State*L, int key, int cache_ref);

static lapi_xlua_tryget_cachedudType lapi_xlua_tryget_cachedud_ptr;

int lapi_xlua_tryget_cachedud(lua_State*L, int key, int cache_ref){
    return lapi_xlua_tryget_cachedud_ptr(L, key, cache_ref);
}

//xlua_getcsobj_ptr
typedef void* (*lapi_xlua_getcsobj_ptrType)(lua_State*L, int index);

static lapi_xlua_getcsobj_ptrType lapi_xlua_getcsobj_ptr_ptr;

void* lapi_xlua_getcsobj_ptr(lua_State*L, int index){
    return lapi_xlua_getcsobj_ptr_ptr(L, index);
}

void lapi_init(lapi_func_ptr* func_array){
    lapi_lua_touserdata_ptr = (lapi_lua_touserdataType)func_array[0];
    lapi_lua_type_ptr = (lapi_lua_typeType)func_array[1];
    lapi_lua_gettop_ptr = (lapi_lua_gettopType)func_array[2];
    lapi_lua_upvalueindex_ptr = (lapi_lua_upvalueindexType)func_array[3];
    lapi_xlua_pushcsobj_ptr_ptr = (lapi_xlua_pushcsobj_ptrType)func_array[4];
    lapi_lua_isnumber_ptr = (lapi_lua_isnumberType)func_array[5];
    lapi_lua_isstring_ptr = (lapi_lua_isstringType)func_array[6];
    lapi_lua_iscfunction_ptr = (lapi_lua_iscfunctionType)func_array[7];
    lapi_lua_isinteger_ptr = (lapi_lua_isintegerType)func_array[8];
    lapi_lua_isuserdata_ptr = (lapi_lua_isuserdataType)func_array[9];
    lapi_lua_typename_ptr = (lapi_lua_typenameType)func_array[10];
    lapi_lua_tonumber_ptr = (lapi_lua_tonumberType)func_array[11];
    lapi_lua_tolstring_ptr = (lapi_lua_tolstringType)func_array[12];
    lapi_lua_toboolean_ptr = (lapi_lua_tobooleanType)func_array[13];
    lapi_lua_topointer_ptr = (lapi_lua_topointerType)func_array[14];
    lapi_xlua_tryget_cachedud_ptr = (lapi_xlua_tryget_cachedudType)func_array[15];
    lapi_xlua_getcsobj_ptr_ptr = (lapi_xlua_getcsobj_ptrType)func_array[16];
}

EXTERN_C_END