#ifndef LUA_API_ADAPT
#define LUA_API_ADAPT
#include "stdint.h"

#ifdef __cplusplus
#define EXTERN_C_START \
    extern "C"         \
    {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

EXTERN_C_START


#define LUA_TNONE		(-1)
#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8

#define LUA_NUMTYPES		9

#define LUA_NUMBER	double

/* type of numbers in Lua */
typedef LUA_NUMBER lua_Number;
typedef struct lua_State lua_State;

typedef int (*lua_CFunction) (lua_State* L);

typedef void (*lapi_func_ptr)(void);

typedef struct {
    int fake_id;
    unsigned int len;
    char data[1];
    void* typeId;
} CSharpStruct;

//genBegin
//lua_gettop
typedef int   ((*lapi_lua_gettopType)) (lua_State *L);
extern lapi_lua_gettopType lapi_lua_gettop;

//lua_settop
typedef void  ((*lapi_lua_settopType)) (lua_State *L, int idx);
extern lapi_lua_settopType lapi_lua_settop;

//lua_remove
typedef void (*lapi_lua_removeType) (lua_State *L, int idx);
extern lapi_lua_removeType lapi_lua_remove;

//lua_insert
typedef void (*lapi_lua_insertType) (lua_State *L, int idx);
extern lapi_lua_insertType lapi_lua_insert;

//lua_replace
typedef void (*lapi_lua_replaceType) (lua_State *L, int idx);
extern lapi_lua_replaceType lapi_lua_replace;

//lua_copy
typedef void  ((*lapi_lua_copyType)) (lua_State *L, int fromidx, int toidx);
extern lapi_lua_copyType lapi_lua_copy;

//lua_pushvalue
typedef void  ((*lapi_lua_pushvalueType)) (lua_State *L, int idx);
extern lapi_lua_pushvalueType lapi_lua_pushvalue;

//lua_type
typedef int             ((*lapi_lua_typeType)) (lua_State *L, int idx);
extern lapi_lua_typeType lapi_lua_type;

//lua_typename
typedef const char     *((*lapi_lua_typenameType)) (lua_State *L, int tp);
extern lapi_lua_typenameType lapi_lua_typename;

//lapi_lua_upvalueindex
typedef int (*lapi_lapi_lua_upvalueindexType)(int index);
extern lapi_lapi_lua_upvalueindexType lapi_lapi_lua_upvalueindex;

//lua_isnumber
typedef int             ((*lapi_lua_isnumberType)) (lua_State *L, int idx);
extern lapi_lua_isnumberType lapi_lua_isnumber;

//lua_isstring
typedef int             ((*lapi_lua_isstringType)) (lua_State *L, int idx);
extern lapi_lua_isstringType lapi_lua_isstring;

//lua_iscfunction
typedef int             ((*lapi_lua_iscfunctionType)) (lua_State *L, int idx);
extern lapi_lua_iscfunctionType lapi_lua_iscfunction;

//lua_isinteger
typedef int             ((*lapi_lua_isintegerType)) (lua_State *L, int idx);
extern lapi_lua_isintegerType lapi_lua_isinteger;

//lua_isuserdata
typedef int             ((*lapi_lua_isuserdataType)) (lua_State *L, int idx);
extern lapi_lua_isuserdataType lapi_lua_isuserdata;

//lua_tonumber
typedef lua_Number (*lapi_lua_tonumberType) (lua_State *L, int idx);
extern lapi_lua_tonumberType lapi_lua_tonumber;

//lua_touserdata
typedef void	       *((*lapi_lua_touserdataType)) (lua_State *L, int idx);
extern lapi_lua_touserdataType lapi_lua_touserdata;

//lua_tolstring
typedef const char     *((*lapi_lua_tolstringType)) (lua_State *L, int idx, size_t *len);
extern lapi_lua_tolstringType lapi_lua_tolstring;

//lua_toboolean
typedef int             ((*lapi_lua_tobooleanType)) (lua_State *L, int idx);
extern lapi_lua_tobooleanType lapi_lua_toboolean;

//lua_topointer
typedef const void     *((*lapi_lua_topointerType)) (lua_State *L, int idx);
extern lapi_lua_topointerType lapi_lua_topointer;

//xlua_pushinteger
typedef void (*lapi_xlua_pushintegerType) (lua_State *L, int n);
extern lapi_xlua_pushintegerType lapi_xlua_pushinteger;

//xlua_tointeger
typedef int (*lapi_xlua_tointegerType) (lua_State *L, int idx);
extern lapi_xlua_tointegerType lapi_xlua_tointeger;

//xlua_touint
typedef uint32_t (*lapi_xlua_touintType) (lua_State *L, int idx);
extern lapi_xlua_touintType lapi_xlua_touint;

//xlua_pushuint
typedef void (*lapi_xlua_pushuintType) (lua_State *L, uint32_t n);
extern lapi_xlua_pushuintType lapi_xlua_pushuint;

//lua_pushint64
typedef void (*lapi_lua_pushint64Type)(lua_State* L, int64_t n);
extern lapi_lua_pushint64Type lapi_lua_pushint64;

//lua_pushuint64
typedef void (*lapi_lua_pushuint64Type)(lua_State* L, uint64_t n);
extern lapi_lua_pushuint64Type lapi_lua_pushuint64;

//lua_isint64
typedef int (*lapi_lua_isint64Type)(lua_State* L, int pos);
extern lapi_lua_isint64Type lapi_lua_isint64;

//lua_isuint64
typedef int (*lapi_lua_isuint64Type)(lua_State* L, int pos);
extern lapi_lua_isuint64Type lapi_lua_isuint64;

//lua_toint64
typedef int64_t (*lapi_lua_toint64Type)(lua_State* L, int pos);
extern lapi_lua_toint64Type lapi_lua_toint64;

//lua_touint64
typedef uint64_t (*lapi_lua_touint64Type)(lua_State* L, int pos);
extern lapi_lua_touint64Type lapi_lua_touint64;

//lua_setupvalue
typedef const char *((*lapi_lua_setupvalueType)) (lua_State *L, int funcindex, int n);
extern lapi_lua_setupvalueType lapi_lua_setupvalue;

//lua_getupvalue
typedef const char *((*lapi_lua_getupvalueType)) (lua_State *L, int funcindex, int n);
extern lapi_lua_getupvalueType lapi_lua_getupvalue;

//xlua_call
typedef void (*lapi_xlua_callType)(lua_State*L, int nargs, int nresults);
extern lapi_xlua_callType lapi_xlua_call;

//lua_pcall
typedef int (*lapi_lua_pcallType) (lua_State *L, int nargs, int nresults, int errfunc);
extern lapi_lua_pcallType lapi_lua_pcall;

//pcall_prepare
typedef int (*lapi_pcall_prepareType)(lua_State *L, int error_func_ref, int func_ref);
extern lapi_pcall_prepareType lapi_pcall_prepare;

//luaL_error
typedef int ((*lapi_luaL_errorType)) (lua_State *L, const char *fmt, ...);
extern lapi_luaL_errorType lapi_luaL_error;

//load_error_func
typedef int (*lapi_load_error_funcType)(lua_State *L, int ref);
extern lapi_load_error_funcType lapi_load_error_func;

//lua_gettable
typedef int ((*lapi_lua_gettableType)) (lua_State *L, int idx);
extern lapi_lua_gettableType lapi_lua_gettable;

//lua_settable
typedef void  ((*lapi_lua_settableType)) (lua_State *L, int idx);
extern lapi_lua_settableType lapi_lua_settable;

//lua_createtable
typedef void  ((*lapi_lua_createtableType)) (lua_State *L, int narr, int nrec);
extern lapi_lua_createtableType lapi_lua_createtable;

//lua_pushcclosure
typedef void  ((*lapi_lua_pushcclosureType)) (lua_State *L, lua_CFunction fn, int n);
extern lapi_lua_pushcclosureType lapi_lua_pushcclosure;

//lua_pushlightuserdata
typedef void  ((*lapi_lua_pushlightuserdataType)) (lua_State *L, void *p);
extern lapi_lua_pushlightuserdataType lapi_lua_pushlightuserdata;

//lua_pushboolean
typedef void  ((*lapi_lua_pushbooleanType)) (lua_State *L, int b);
extern lapi_lua_pushbooleanType lapi_lua_pushboolean;

//lua_pushstring
typedef const char *((*lapi_lua_pushstringType)) (lua_State *L, const char *s);
extern lapi_lua_pushstringType lapi_lua_pushstring;

//lua_pushlstring
typedef const char *((*lapi_lua_pushlstringType)) (lua_State *L, const char *s, size_t len);
extern lapi_lua_pushlstringType lapi_lua_pushlstring;

//lua_pushnumber
typedef void        ((*lapi_lua_pushnumberType)) (lua_State *L, lua_Number n);
extern lapi_lua_pushnumberType lapi_lua_pushnumber;

//lua_pushnil
typedef void        ((*lapi_lua_pushnilType)) (lua_State *L);
extern lapi_lua_pushnilType lapi_lua_pushnil;

//lua_setmetatable
typedef int   ((*lapi_lua_setmetatableType)) (lua_State *L, int objindex);
extern lapi_lua_setmetatableType lapi_lua_setmetatable;

//xlua_getglobal
typedef int (*lapi_xlua_getglobalType) (lua_State *L, const char *name);
extern lapi_xlua_getglobalType lapi_xlua_getglobal;

//xlua_setglobal
typedef int (*lapi_xlua_setglobalType) (lua_State *L, const char *name);
extern lapi_xlua_setglobalType lapi_xlua_setglobal;

//xlua_get_registry_index
typedef int (*lapi_xlua_get_registry_indexType)();
extern lapi_xlua_get_registry_indexType lapi_xlua_get_registry_index;

//xlua_rawgeti
typedef void (*lapi_xlua_rawgetiType) (lua_State *L, int idx, int64_t n);
extern lapi_xlua_rawgetiType lapi_xlua_rawgeti;

//xlua_rawseti
typedef void (*lapi_xlua_rawsetiType) (lua_State *L, int idx, int64_t n);
extern lapi_xlua_rawsetiType lapi_xlua_rawseti;

//lua_rawget
typedef int ((*lapi_lua_rawgetType)) (lua_State *L, int idx);
extern lapi_lua_rawgetType lapi_lua_rawget;

//lua_rawset
typedef void  ((*lapi_lua_rawsetType)) (lua_State *L, int idx);
extern lapi_lua_rawsetType lapi_lua_rawset;

//luaL_ref
typedef int ((*lapi_luaL_refType)) (lua_State *L, int t);
extern lapi_luaL_refType lapi_luaL_ref;

//xlua_tag
typedef void *(*lapi_xlua_tagType) ();
extern lapi_xlua_tagType lapi_xlua_tag;

//luaL_newmetatable
typedef int   ((*lapi_luaL_newmetatableType)) (lua_State *L, const char *tname);
extern lapi_luaL_newmetatableType lapi_luaL_newmetatable;

//xlua_gl
typedef void* (*lapi_xlua_glType)(lua_State *L);
extern lapi_xlua_glType lapi_xlua_gl;

//xlua_mainthread
typedef void* (*lapi_xlua_mainthreadType)(lua_State *L);
extern lapi_xlua_mainthreadType lapi_xlua_mainthread;

//xlua_objlen
typedef uint32_t (*lapi_xlua_objlenType) (lua_State *L, int idx);
extern lapi_xlua_objlenType lapi_xlua_objlen;

//lua_newuserdatauv
typedef void *((*lapi_lua_newuserdatauvType)) (lua_State *L, size_t sz, int nuvalue);
extern lapi_lua_newuserdatauvType lapi_lua_newuserdatauv;

//xlua_tryget_cachedud
typedef int (*lapi_xlua_tryget_cachedudType)(lua_State *L, int key, int cache_ref);
extern lapi_xlua_tryget_cachedudType lapi_xlua_tryget_cachedud;

//genEnd

#define lapi_lua_isboolean(L,n)	(lapi_lua_type(L, (n)) == LUA_TBOOLEAN)
#define lapi_lua_isfunction(L,n)(lapi_lua_type(L, (n)) == LUA_TFUNCTION)
//lua_typenamestackIdx
#define lua_typename_stackIdx(L,n)(lapi_lua_typename(L, lapi_lua_type(L,n)))
// lua_isnil
#define lapi_lua_isnil(L,n)		(lapi_lua_type(L, (n)) == LUA_TNIL)
#define lapi_lua_pop(L,n)		lapi_lua_settop(L, -(n)-1);

#define lapi_lua_tostring(L,i)	lapi_lua_tolstring(L, (i), NULL)

#define lapi_luaL_getmetatable(L, meta) lapi_lua_pushstring(L, meta); \
lapi_lua_rawget(L, lapi_xlua_get_registry_index())

#define lapi_lua_getref(L,ref) lapi_xlua_rawgeti(L, lapi_xlua_get_registry_index(), ref);

#define lapi_lua_newuserdata(L,s)	lapi_lua_newuserdatauv(L,s,1)

void lapi_init(lapi_func_ptr* func_array);


EXTERN_C_END
#endif