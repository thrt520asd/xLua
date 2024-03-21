#include "lua_api_adapt.h"
#include "stddef.h"
EXTERN_C_START
//genBegin
//lua_touserdata
typedef void	       *((*lapi_lua_touserdataType)) (lua_State *L, int idx);
static lapi_lua_touserdataType lapi_lua_touserdata_ptr;
 void	       *(lapi_lua_touserdata) (lua_State *L, int idx){
    return lapi_lua_touserdata_ptr(L,idx);
}

//lua_type
typedef int             ((*lapi_lua_typeType)) (lua_State *L, int idx);
static lapi_lua_typeType lapi_lua_type_ptr;
 int             (lapi_lua_type) (lua_State *L, int idx){
    return lapi_lua_type_ptr(L,idx);
}

//lua_gettop
typedef int   ((*lapi_lua_gettopType)) (lua_State *L);
static lapi_lua_gettopType lapi_lua_gettop_ptr;
 int   (lapi_lua_gettop) (lua_State *L){
    return lapi_lua_gettop_ptr(L);
}

//lapi_lua_upvalueindex
typedef int (*lapi_lapi_lua_upvalueindexType)(int index);
static lapi_lapi_lua_upvalueindexType lapi_lapi_lua_upvalueindex_ptr;
int lapi_lapi_lua_upvalueindex(int index){
    return lapi_lapi_lua_upvalueindex_ptr(index);
}

//xlua_pushcsobj_ptr
typedef void (*lapi_xlua_pushcsobj_ptrType)(lua_State* L, void* ptr, int meta_ref, int key, int need_cache, int cache_ref, int poolIdx);
static lapi_xlua_pushcsobj_ptrType lapi_xlua_pushcsobj_ptr_ptr;
 void lapi_xlua_pushcsobj_ptr(lua_State* L, void* ptr, int meta_ref, int key, int need_cache, int cache_ref, int poolIdx){
    return lapi_xlua_pushcsobj_ptr_ptr(L,ptr,meta_ref,key,need_cache,cache_ref,poolIdx);
}

//lua_isnumber
typedef int             ((*lapi_lua_isnumberType)) (lua_State *L, int idx);
static lapi_lua_isnumberType lapi_lua_isnumber_ptr;
 int             (lapi_lua_isnumber) (lua_State *L, int idx){
    return lapi_lua_isnumber_ptr(L,idx);
}

//lua_isstring
typedef int             ((*lapi_lua_isstringType)) (lua_State *L, int idx);
static lapi_lua_isstringType lapi_lua_isstring_ptr;
 int             (lapi_lua_isstring) (lua_State *L, int idx){
    return lapi_lua_isstring_ptr(L,idx);
}

//lua_iscfunction
typedef int             ((*lapi_lua_iscfunctionType)) (lua_State *L, int idx);
static lapi_lua_iscfunctionType lapi_lua_iscfunction_ptr;
 int             (lapi_lua_iscfunction) (lua_State *L, int idx){
    return lapi_lua_iscfunction_ptr(L,idx);
}

//lua_isinteger
typedef int             ((*lapi_lua_isintegerType)) (lua_State *L, int idx);
static lapi_lua_isintegerType lapi_lua_isinteger_ptr;
 int             (lapi_lua_isinteger) (lua_State *L, int idx){
    return lapi_lua_isinteger_ptr(L,idx);
}

//lua_isuserdata
typedef int             ((*lapi_lua_isuserdataType)) (lua_State *L, int idx);
static lapi_lua_isuserdataType lapi_lua_isuserdata_ptr;
 int             (lapi_lua_isuserdata) (lua_State *L, int idx){
    return lapi_lua_isuserdata_ptr(L,idx);
}

//lua_typename
typedef const char     *((*lapi_lua_typenameType)) (lua_State *L, int tp);
static lapi_lua_typenameType lapi_lua_typename_ptr;
 const char     *(lapi_lua_typename) (lua_State *L, int tp){
    return lapi_lua_typename_ptr(L,tp);
}

//lua_tonumber
typedef lua_Number (*lapi_lua_tonumberType) (lua_State *L, int idx);
static lapi_lua_tonumberType lapi_lua_tonumber_ptr;
 lua_Number lapi_lua_tonumber (lua_State *L, int idx){
    return lapi_lua_tonumber_ptr(L,idx);
}

//lua_tolstring
typedef const char     *((*lapi_lua_tolstringType)) (lua_State *L, int idx, size_t *len);
static lapi_lua_tolstringType lapi_lua_tolstring_ptr;
 const char     *(lapi_lua_tolstring) (lua_State *L, int idx, size_t *len){
    return lapi_lua_tolstring_ptr(L,idx,len);
}

//lua_toboolean
typedef int             ((*lapi_lua_tobooleanType)) (lua_State *L, int idx);
static lapi_lua_tobooleanType lapi_lua_toboolean_ptr;
 int             (lapi_lua_toboolean) (lua_State *L, int idx){
    return lapi_lua_toboolean_ptr(L,idx);
}

//lua_topointer
typedef const void     *((*lapi_lua_topointerType)) (lua_State *L, int idx);
static lapi_lua_topointerType lapi_lua_topointer_ptr;
 const void     *(lapi_lua_topointer) (lua_State *L, int idx){
    return lapi_lua_topointer_ptr(L,idx);
}

//xlua_tryget_cachedud
typedef int (*lapi_xlua_tryget_cachedudType)(lua_State *L, int key, int cache_ref);
static lapi_xlua_tryget_cachedudType lapi_xlua_tryget_cachedud_ptr;
 int lapi_xlua_tryget_cachedud(lua_State *L, int key, int cache_ref){
    return lapi_xlua_tryget_cachedud_ptr(L,key,cache_ref);
}

//xlua_getcsobj_ptr
typedef void* (*lapi_xlua_getcsobj_ptrType)(lua_State* L,int index);
static lapi_xlua_getcsobj_ptrType lapi_xlua_getcsobj_ptr_ptr;
 void* lapi_xlua_getcsobj_ptr(lua_State* L,int index){
    return lapi_xlua_getcsobj_ptr_ptr(L,index);
}

//lua_pushcclosure
typedef void  ((*lapi_lua_pushcclosureType)) (lua_State *L, lua_CFunction fn, int n);
static lapi_lua_pushcclosureType lapi_lua_pushcclosure_ptr;
 void  (lapi_lua_pushcclosure) (lua_State *L, lua_CFunction fn, int n){
    return lapi_lua_pushcclosure_ptr(L,fn,n);
}

//lua_setupvalue
typedef const char *((*lapi_lua_setupvalueType)) (lua_State *L, int funcindex, int n);
static lapi_lua_setupvalueType lapi_lua_setupvalue_ptr;
 const char *(lapi_lua_setupvalue) (lua_State *L, int funcindex, int n){
    return lapi_lua_setupvalue_ptr(L,funcindex,n);
}

//lua_getupvalue
typedef const char *((*lapi_lua_getupvalueType)) (lua_State *L, int funcindex, int n);
static lapi_lua_getupvalueType lapi_lua_getupvalue_ptr;
 const char *(lapi_lua_getupvalue) (lua_State *L, int funcindex, int n){
    return lapi_lua_getupvalue_ptr(L,funcindex,n);
}

//lua_pushvalue
typedef void  ((*lapi_lua_pushvalueType)) (lua_State *L, int idx);
static lapi_lua_pushvalueType lapi_lua_pushvalue_ptr;
 void  (lapi_lua_pushvalue) (lua_State *L, int idx){
    return lapi_lua_pushvalue_ptr(L,idx);
}

//lua_gettable
typedef int ((*lapi_lua_gettableType)) (lua_State *L, int idx);
static lapi_lua_gettableType lapi_lua_gettable_ptr;
 int (lapi_lua_gettable) (lua_State *L, int idx){
    return lapi_lua_gettable_ptr(L,idx);
}

//xlua_call
typedef void (*lapi_xlua_callType)(lua_State*L, int nargs, int nresults);
static lapi_xlua_callType lapi_xlua_call_ptr;
void lapi_xlua_call(lua_State*L, int nargs, int nresults){
    return lapi_xlua_call_ptr(L,nargs,nresults);
}

//lua_settop
typedef void  ((*lapi_lua_settopType)) (lua_State *L, int idx);
static lapi_lua_settopType lapi_lua_settop_ptr;
 void  (lapi_lua_settop) (lua_State *L, int idx){
    return lapi_lua_settop_ptr(L,idx);
}

//lua_pushlightuserdata
typedef void  ((*lapi_lua_pushlightuserdataType)) (lua_State *L, void *p);
static lapi_lua_pushlightuserdataType lapi_lua_pushlightuserdata_ptr;
 void  (lapi_lua_pushlightuserdata) (lua_State *L, void *p){
    return lapi_lua_pushlightuserdata_ptr(L,p);
}

//lua_settable
typedef void  ((*lapi_lua_settableType)) (lua_State *L, int idx);
static lapi_lua_settableType lapi_lua_settable_ptr;
 void  (lapi_lua_settable) (lua_State *L, int idx){
    return lapi_lua_settable_ptr(L,idx);
}

//lua_createtable
typedef void  ((*lapi_lua_createtableType)) (lua_State *L, int narr, int nrec);
static lapi_lua_createtableType lapi_lua_createtable_ptr;
 void  (lapi_lua_createtable) (lua_State *L, int narr, int nrec){
    return lapi_lua_createtable_ptr(L,narr,nrec);
}

//lua_pushboolean
typedef void  ((*lapi_lua_pushbooleanType)) (lua_State *L, int b);
static lapi_lua_pushbooleanType lapi_lua_pushboolean_ptr;
 void  (lapi_lua_pushboolean) (lua_State *L, int b){
    return lapi_lua_pushboolean_ptr(L,b);
}

//lua_pushstring
typedef const char *((*lapi_lua_pushstringType)) (lua_State *L, const char *s);
static lapi_lua_pushstringType lapi_lua_pushstring_ptr;
 const char *(lapi_lua_pushstring) (lua_State *L, const char *s){
    return lapi_lua_pushstring_ptr(L,s);
}

//lua_pushlstring
typedef const char *((*lapi_lua_pushlstringType)) (lua_State *L, const char *s, size_t len);
static lapi_lua_pushlstringType lapi_lua_pushlstring_ptr;
 const char *(lapi_lua_pushlstring) (lua_State *L, const char *s, size_t len){
    return lapi_lua_pushlstring_ptr(L,s,len);
}

//lua_pushnumber
typedef void        ((*lapi_lua_pushnumberType)) (lua_State *L, lua_Number n);
static lapi_lua_pushnumberType lapi_lua_pushnumber_ptr;
 void        (lapi_lua_pushnumber) (lua_State *L, lua_Number n){
    return lapi_lua_pushnumber_ptr(L,n);
}

//lua_pushnil
typedef void        ((*lapi_lua_pushnilType)) (lua_State *L);
static lapi_lua_pushnilType lapi_lua_pushnil_ptr;
 void        (lapi_lua_pushnil) (lua_State *L){
    return lapi_lua_pushnil_ptr(L);
}

//lua_pushint64
typedef void (*lapi_lua_pushint64Type)(lua_State* L, int64_t n);
static lapi_lua_pushint64Type lapi_lua_pushint64_ptr;
 void lapi_lua_pushint64(lua_State* L, int64_t n){
    return lapi_lua_pushint64_ptr(L,n);
}

//lua_pushuint64
typedef void (*lapi_lua_pushuint64Type)(lua_State* L, uint64_t n);
static lapi_lua_pushuint64Type lapi_lua_pushuint64_ptr;
 void lapi_lua_pushuint64(lua_State* L, uint64_t n){
    return lapi_lua_pushuint64_ptr(L,n);
}

//luaL_error
typedef int ((*lapi_luaL_errorType)) (lua_State *L, const char *fmt, ...);
static lapi_luaL_errorType lapi_luaL_error_ptr;
 int (lapi_luaL_error) (lua_State *L, const char *fmt, ...){
    return lapi_luaL_error_ptr(L,fmt);
}

//lua_remove
typedef void (*lapi_lua_removeType) (lua_State *L, int idx);
static lapi_lua_removeType lapi_lua_remove_ptr;
 void lapi_lua_remove (lua_State *L, int idx){
    return lapi_lua_remove_ptr(L,idx);
}

//lua_insert
typedef void (*lapi_lua_insertType) (lua_State *L, int idx);
static lapi_lua_insertType lapi_lua_insert_ptr;
 void lapi_lua_insert (lua_State *L, int idx){
    return lapi_lua_insert_ptr(L,idx);
}

//lua_replace
typedef void (*lapi_lua_replaceType) (lua_State *L, int idx);
static lapi_lua_replaceType lapi_lua_replace_ptr;
 void lapi_lua_replace (lua_State *L, int idx){
    return lapi_lua_replace_ptr(L,idx);
}

//lua_copy
typedef void  ((*lapi_lua_copyType)) (lua_State *L, int fromidx, int toidx);
static lapi_lua_copyType lapi_lua_copy_ptr;
 void  (lapi_lua_copy) (lua_State *L, int fromidx, int toidx){
    return lapi_lua_copy_ptr(L,fromidx,toidx);
}

//xlua_createstruct_pointer
typedef CSharpStruct* (*lapi_xlua_createstruct_pointerType)(lua_State *L, unsigned int size, int meta_ref, void * typePointer);
static lapi_xlua_createstruct_pointerType lapi_xlua_createstruct_pointer_ptr;
 CSharpStruct* lapi_xlua_createstruct_pointer(lua_State *L, unsigned int size, int meta_ref, void * typePointer){
    return lapi_xlua_createstruct_pointer_ptr(L,size,meta_ref,typePointer);
}

//xlua_pushstruct_pointer
typedef CSharpStruct* (*lapi_xlua_pushstruct_pointerType)(lua_State *L, unsigned int size, void* pointer, int meta_ref, void * typePointer);
static lapi_xlua_pushstruct_pointerType lapi_xlua_pushstruct_pointer_ptr;
 CSharpStruct* lapi_xlua_pushstruct_pointer(lua_State *L, unsigned int size, void* pointer, int meta_ref, void * typePointer){
    return lapi_xlua_pushstruct_pointer_ptr(L,size,pointer,meta_ref,typePointer);
}

//xlua_tocss
typedef CSharpStruct* (*lapi_xlua_tocssType)(lua_State *L, int index);
static lapi_xlua_tocssType lapi_xlua_tocss_ptr;
 CSharpStruct* lapi_xlua_tocss(lua_State *L, int index){
    return lapi_xlua_tocss_ptr(L,index);
}

//xlua_getglobal
typedef int (*lapi_xlua_getglobalType) (lua_State *L, const char *name);
static lapi_xlua_getglobalType lapi_xlua_getglobal_ptr;
 int lapi_xlua_getglobal (lua_State *L, const char *name){
    return lapi_xlua_getglobal_ptr(L,name);
}

//xlua_setglobal
typedef int (*lapi_xlua_setglobalType) (lua_State *L, const char *name);
static lapi_xlua_setglobalType lapi_xlua_setglobal_ptr;
 int lapi_xlua_setglobal (lua_State *L, const char *name){
    return lapi_xlua_setglobal_ptr(L,name);
}

//lua_isint64
typedef int (*lapi_lua_isint64Type)(lua_State* L, int pos);
static lapi_lua_isint64Type lapi_lua_isint64_ptr;
 int lapi_lua_isint64(lua_State* L, int pos){
    return lapi_lua_isint64_ptr(L,pos);
}

//lua_isuint64
typedef int (*lapi_lua_isuint64Type)(lua_State* L, int pos);
static lapi_lua_isuint64Type lapi_lua_isuint64_ptr;
 int lapi_lua_isuint64(lua_State* L, int pos){
    return lapi_lua_isuint64_ptr(L,pos);
}

//lua_toint64
typedef int64_t (*lapi_lua_toint64Type)(lua_State* L, int pos);
static lapi_lua_toint64Type lapi_lua_toint64_ptr;
 int64_t lapi_lua_toint64(lua_State* L, int pos){
    return lapi_lua_toint64_ptr(L,pos);
}

//lua_touint64
typedef uint64_t (*lapi_lua_touint64Type)(lua_State* L, int pos);
static lapi_lua_touint64Type lapi_lua_touint64_ptr;
 uint64_t lapi_lua_touint64(lua_State* L, int pos){
    return lapi_lua_touint64_ptr(L,pos);
}

//load_error_func
typedef int (*lapi_load_error_funcType)(lua_State *L, int ref);
static lapi_load_error_funcType lapi_load_error_func_ptr;
 int lapi_load_error_func(lua_State *L, int ref){
    return lapi_load_error_func_ptr(L,ref);
}

//xlua_get_registry_index
typedef int (*lapi_xlua_get_registry_indexType)();
static lapi_xlua_get_registry_indexType lapi_xlua_get_registry_index_ptr;
 int lapi_xlua_get_registry_index(){
    return lapi_xlua_get_registry_index_ptr();
}

//xlua_rawgeti
typedef void (*lapi_xlua_rawgetiType) (lua_State *L, int idx, int64_t n);
static lapi_xlua_rawgetiType lapi_xlua_rawgeti_ptr;
 void lapi_xlua_rawgeti (lua_State *L, int idx, int64_t n){
    return lapi_xlua_rawgeti_ptr(L,idx,n);
}

//xlua_rawseti
typedef void (*lapi_xlua_rawsetiType) (lua_State *L, int idx, int64_t n);
static lapi_xlua_rawsetiType lapi_xlua_rawseti_ptr;
 void lapi_xlua_rawseti (lua_State *L, int idx, int64_t n){
    return lapi_xlua_rawseti_ptr(L,idx,n);
}

//lua_pcall
typedef int (*lapi_lua_pcallType) (lua_State *L, int nargs, int nresults, int errfunc);
static lapi_lua_pcallType lapi_lua_pcall_ptr;
 int lapi_lua_pcall (lua_State *L, int nargs, int nresults, int errfunc){
    return lapi_lua_pcall_ptr(L,nargs,nresults,errfunc);
}

//pcall_prepare
typedef int (*lapi_pcall_prepareType)(lua_State *L, int error_func_ref, int func_ref);
static lapi_pcall_prepareType lapi_pcall_prepare_ptr;
 int lapi_pcall_prepare(lua_State *L, int error_func_ref, int func_ref){
    return lapi_pcall_prepare_ptr(L,error_func_ref,func_ref);
}

//luaL_ref
typedef int ((*lapi_luaL_refType)) (lua_State *L, int t);
static lapi_luaL_refType lapi_luaL_ref_ptr;
 int (lapi_luaL_ref) (lua_State *L, int t){
    return lapi_luaL_ref_ptr(L,t);
}

//xlua_tag
typedef void *(*lapi_xlua_tagType) ();
static lapi_xlua_tagType lapi_xlua_tag_ptr;
 void *lapi_xlua_tag () {
    return lapi_xlua_tag_ptr();
}

//lua_rawget
typedef int ((*lapi_lua_rawgetType)) (lua_State *L, int idx);
static lapi_lua_rawgetType lapi_lua_rawget_ptr;
 int (lapi_lua_rawget) (lua_State *L, int idx){
    return lapi_lua_rawget_ptr(L,idx);
}

//lua_rawset
typedef void  ((*lapi_lua_rawsetType)) (lua_State *L, int idx);
static lapi_lua_rawsetType lapi_lua_rawset_ptr;
 void  (lapi_lua_rawset) (lua_State *L, int idx){
    return lapi_lua_rawset_ptr(L,idx);
}

//luaL_newmetatable
typedef int   ((*lapi_luaL_newmetatableType)) (lua_State *L, const char *tname);
static lapi_luaL_newmetatableType lapi_luaL_newmetatable_ptr;
 int   (lapi_luaL_newmetatable) (lua_State *L, const char *tname){
    return lapi_luaL_newmetatable_ptr(L,tname);
}

//xlua_pushinteger
typedef void (*lapi_xlua_pushintegerType) (lua_State *L, int n);
static lapi_xlua_pushintegerType lapi_xlua_pushinteger_ptr;
 void lapi_xlua_pushinteger (lua_State *L, int n){
    return lapi_xlua_pushinteger_ptr(L,n);
}

//xlua_tointeger
typedef int (*lapi_xlua_tointegerType) (lua_State *L, int idx);
static lapi_xlua_tointegerType lapi_xlua_tointeger_ptr;
 int lapi_xlua_tointeger (lua_State *L, int idx){
    return lapi_xlua_tointeger_ptr(L,idx);
}

//xlua_pushuint
typedef void (*lapi_xlua_pushuintType) (lua_State *L, uint32_t n);
static lapi_xlua_pushuintType lapi_xlua_pushuint_ptr;
 void lapi_xlua_pushuint (lua_State *L, uint32_t n){
    return lapi_xlua_pushuint_ptr(L,n);
}

//xlua_touint
typedef uint32_t (*lapi_xlua_touintType) (lua_State *L, int idx);
static lapi_xlua_touintType lapi_xlua_touint_ptr;
 uint32_t lapi_xlua_touint (lua_State *L, int idx){
    return lapi_xlua_touint_ptr(L,idx);
}

//xlua_gl
typedef void* (*lapi_xlua_glType)(lua_State *L);
static lapi_xlua_glType lapi_xlua_gl_ptr;
 void* lapi_xlua_gl(lua_State *L){
    return lapi_xlua_gl_ptr(L);
}

//xlua_mainthread
typedef void* (*lapi_xlua_mainthreadType)(lua_State *L);
static lapi_xlua_mainthreadType lapi_xlua_mainthread_ptr;
 void* lapi_xlua_mainthread(lua_State *L){
    return lapi_xlua_mainthread_ptr(L);
}

//genEnd

void lapi_init(lapi_func_ptr* func_array){
//gen1
lapi_lua_touserdata_ptr = (lapi_lua_touserdataType)func_array[0];
lapi_lua_type_ptr = (lapi_lua_typeType)func_array[1];
lapi_lua_gettop_ptr = (lapi_lua_gettopType)func_array[2];
lapi_lapi_lua_upvalueindex_ptr = (lapi_lapi_lua_upvalueindexType)func_array[3];
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
lapi_lua_pushcclosure_ptr = (lapi_lua_pushcclosureType)func_array[17];
lapi_lua_setupvalue_ptr = (lapi_lua_setupvalueType)func_array[18];
lapi_lua_getupvalue_ptr = (lapi_lua_getupvalueType)func_array[19];
lapi_lua_pushvalue_ptr = (lapi_lua_pushvalueType)func_array[20];
lapi_lua_gettable_ptr = (lapi_lua_gettableType)func_array[21];
lapi_xlua_call_ptr = (lapi_xlua_callType)func_array[22];
lapi_lua_settop_ptr = (lapi_lua_settopType)func_array[23];
lapi_lua_pushlightuserdata_ptr = (lapi_lua_pushlightuserdataType)func_array[24];
lapi_lua_settable_ptr = (lapi_lua_settableType)func_array[25];
lapi_lua_createtable_ptr = (lapi_lua_createtableType)func_array[26];
lapi_lua_pushboolean_ptr = (lapi_lua_pushbooleanType)func_array[27];
lapi_lua_pushstring_ptr = (lapi_lua_pushstringType)func_array[28];
lapi_lua_pushlstring_ptr = (lapi_lua_pushlstringType)func_array[29];
lapi_lua_pushnumber_ptr = (lapi_lua_pushnumberType)func_array[30];
lapi_lua_pushnil_ptr = (lapi_lua_pushnilType)func_array[31];
lapi_lua_pushint64_ptr = (lapi_lua_pushint64Type)func_array[32];
lapi_lua_pushuint64_ptr = (lapi_lua_pushuint64Type)func_array[33];
lapi_luaL_error_ptr = (lapi_luaL_errorType)func_array[34];
lapi_lua_remove_ptr = (lapi_lua_removeType)func_array[35];
lapi_lua_insert_ptr = (lapi_lua_insertType)func_array[36];
lapi_lua_replace_ptr = (lapi_lua_replaceType)func_array[37];
lapi_lua_copy_ptr = (lapi_lua_copyType)func_array[38];
lapi_xlua_createstruct_pointer_ptr = (lapi_xlua_createstruct_pointerType)func_array[39];
lapi_xlua_pushstruct_pointer_ptr = (lapi_xlua_pushstruct_pointerType)func_array[40];
lapi_xlua_tocss_ptr = (lapi_xlua_tocssType)func_array[41];
lapi_xlua_getglobal_ptr = (lapi_xlua_getglobalType)func_array[42];
lapi_xlua_setglobal_ptr = (lapi_xlua_setglobalType)func_array[43];
lapi_lua_isint64_ptr = (lapi_lua_isint64Type)func_array[44];
lapi_lua_isuint64_ptr = (lapi_lua_isuint64Type)func_array[45];
lapi_lua_toint64_ptr = (lapi_lua_toint64Type)func_array[46];
lapi_lua_touint64_ptr = (lapi_lua_touint64Type)func_array[47];
lapi_load_error_func_ptr = (lapi_load_error_funcType)func_array[48];
lapi_xlua_get_registry_index_ptr = (lapi_xlua_get_registry_indexType)func_array[49];
lapi_xlua_rawgeti_ptr = (lapi_xlua_rawgetiType)func_array[50];
lapi_xlua_rawseti_ptr = (lapi_xlua_rawsetiType)func_array[51];
lapi_lua_pcall_ptr = (lapi_lua_pcallType)func_array[52];
lapi_pcall_prepare_ptr = (lapi_pcall_prepareType)func_array[53];
lapi_luaL_ref_ptr = (lapi_luaL_refType)func_array[54];
lapi_xlua_tag_ptr = (lapi_xlua_tagType)func_array[55];
lapi_lua_rawget_ptr = (lapi_lua_rawgetType)func_array[56];
lapi_lua_rawset_ptr = (lapi_lua_rawsetType)func_array[57];
lapi_luaL_newmetatable_ptr = (lapi_luaL_newmetatableType)func_array[58];
lapi_xlua_pushinteger_ptr = (lapi_xlua_pushintegerType)func_array[59];
lapi_xlua_tointeger_ptr = (lapi_xlua_tointegerType)func_array[60];
lapi_xlua_pushuint_ptr = (lapi_xlua_pushuintType)func_array[61];
lapi_xlua_touint_ptr = (lapi_xlua_touintType)func_array[62];
lapi_xlua_gl_ptr = (lapi_xlua_glType)func_array[63];
lapi_xlua_mainthread_ptr = (lapi_xlua_mainthreadType)func_array[64];
//end1
}

EXTERN_C_END