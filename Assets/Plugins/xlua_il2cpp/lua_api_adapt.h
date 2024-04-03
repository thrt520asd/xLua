#ifndef LUA_API_ADAPT
#define LUA_API_ADAPT
#include "stdint.h"

typedef void* lua_State;
typedef int (*lua_CFunction) (lua_State *L);
// lua中的C#Struct
typedef struct {
	int fake_id;
    unsigned int len;
    void* typeId;
	char data[1];
} CSharpStruct;

// lua中的C#object
typedef struct {
	int poolIdx;
	void* pointer;
} CSharpObject;


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
/* type of numbers in Lua */
typedef LUA_NUMBER lua_Number;


typedef void (*lapi_func_ptr)(void);

//genBegin
//lua_touserdata
void	       *(lapi_lua_touserdata) (lua_State *L, int idx);
//lua_type
int             (lapi_lua_type) (lua_State *L, int idx);
//lua_gettop
int   (lapi_lua_gettop) (lua_State *L);
//lapi_lua_upvalueindex
int lapi_lapi_lua_upvalueindex(int index);
//xlua_pushcsobj_ptr
void lapi_xlua_pushcsobj_ptr(lua_State* L, void* ptr, int meta_ref, int key, int need_cache, int cache_ref, int poolIdx);
//lua_isnumber
int             (lapi_lua_isnumber) (lua_State *L, int idx);
//lua_isstring
int             (lapi_lua_isstring) (lua_State *L, int idx);
//lua_iscfunction
int             (lapi_lua_iscfunction) (lua_State *L, int idx);
//lua_isinteger
int             (lapi_lua_isinteger) (lua_State *L, int idx);
//lua_isuserdata
int             (lapi_lua_isuserdata) (lua_State *L, int idx);
//lua_typename
const char     *(lapi_lua_typename) (lua_State *L, int tp);
//lua_tonumber
lua_Number lapi_lua_tonumber (lua_State *L, int idx);
//lua_tolstring
const char     *(lapi_lua_tolstring) (lua_State *L, int idx, size_t *len);
//lua_toboolean
int             (lapi_lua_toboolean) (lua_State *L, int idx);
//lua_topointer
const void     *(lapi_lua_topointer) (lua_State *L, int idx);
//xlua_tryget_cachedud
int lapi_xlua_tryget_cachedud(lua_State *L, int key, int cache_ref);
//xlua_getcsobj_ptr
void* lapi_xlua_getcsobj_ptr(lua_State* L,int index);
//lua_pushcclosure
void  (lapi_lua_pushcclosure) (lua_State *L, lua_CFunction fn, int n);
//lua_setupvalue
const char *(lapi_lua_setupvalue) (lua_State *L, int funcindex, int n);
//lua_getupvalue
const char *(lapi_lua_getupvalue) (lua_State *L, int funcindex, int n);
//lua_pushvalue
void  (lapi_lua_pushvalue) (lua_State *L, int idx);
//lua_gettable
int (lapi_lua_gettable) (lua_State *L, int idx);
//xlua_call
void lapi_xlua_call(lua_State*L, int nargs, int nresults);
//lua_settop
void  (lapi_lua_settop) (lua_State *L, int idx);
//lua_pushlightuserdata
void  (lapi_lua_pushlightuserdata) (lua_State *L, void *p);
//lua_settable
void  (lapi_lua_settable) (lua_State *L, int idx);
//lua_createtable
void  (lapi_lua_createtable) (lua_State *L, int narr, int nrec);
//lua_pushboolean
void  (lapi_lua_pushboolean) (lua_State *L, int b);
//lua_pushstring
const char *(lapi_lua_pushstring) (lua_State *L, const char *s);
//lua_pushlstring
const char *(lapi_lua_pushlstring) (lua_State *L, const char *s, size_t len);
//lua_pushnumber
void        (lapi_lua_pushnumber) (lua_State *L, lua_Number n);
//lua_pushnil
void        (lapi_lua_pushnil) (lua_State *L);
//lua_pushint64
void lapi_lua_pushint64(lua_State* L, int64_t n);
//lua_pushuint64
void lapi_lua_pushuint64(lua_State* L, uint64_t n);
//luaL_error
int (lapi_luaL_error) (lua_State *L, const char *fmt, ...);
//lua_remove
void lapi_lua_remove (lua_State *L, int idx);
//lua_insert
void lapi_lua_insert (lua_State *L, int idx);
//lua_replace
void lapi_lua_replace (lua_State *L, int idx);
//lua_copy
void  (lapi_lua_copy) (lua_State *L, int fromidx, int toidx);
//xlua_createstruct_pointer
CSharpStruct* lapi_xlua_createstruct_pointer(lua_State *L, unsigned int size, int meta_ref, void * typePointer);
//xlua_pushstruct_pointer
CSharpStruct* lapi_xlua_pushstruct_pointer(lua_State *L, unsigned int size, void* pointer, int meta_ref, void * typePointer);
//xlua_tocss
CSharpStruct* lapi_xlua_tocss(lua_State *L, int index);
//xlua_getglobal
int lapi_xlua_getglobal (lua_State *L, const char *name);
//xlua_setglobal
int lapi_xlua_setglobal (lua_State *L, const char *name);
//lua_isint64
int lapi_lua_isint64(lua_State* L, int pos);
//lua_isuint64
int lapi_lua_isuint64(lua_State* L, int pos);
//lua_toint64
int64_t lapi_lua_toint64(lua_State* L, int pos);
//lua_touint64
uint64_t lapi_lua_touint64(lua_State* L, int pos);
//load_error_func
int lapi_load_error_func(lua_State *L, int ref);
//xlua_get_registry_index
int lapi_xlua_get_registry_index();
//xlua_rawgeti
void lapi_xlua_rawgeti (lua_State *L, int idx, int64_t n);
//xlua_rawseti
void lapi_xlua_rawseti (lua_State *L, int idx, int64_t n);
//lua_pcall
int lapi_lua_pcall (lua_State *L, int nargs, int nresults, int errfunc);
//pcall_prepare
int lapi_pcall_prepare(lua_State *L, int error_func_ref, int func_ref);
//luaL_ref
int (lapi_luaL_ref) (lua_State *L, int t);
//xlua_tag
void *lapi_xlua_tag ();
//lua_rawget
int (lapi_lua_rawget) (lua_State *L, int idx);
//lua_rawset
void  (lapi_lua_rawset) (lua_State *L, int idx);
//luaL_newmetatable
int   (lapi_luaL_newmetatable) (lua_State *L, const char *tname);
//xlua_pushinteger
void lapi_xlua_pushinteger (lua_State *L, int n);
//xlua_tointeger
int lapi_xlua_tointeger (lua_State *L, int idx);
//xlua_pushuint
void lapi_xlua_pushuint (lua_State *L, uint32_t n);
//xlua_touint
uint32_t lapi_xlua_touint (lua_State *L, int idx);
//xlua_gl
void* lapi_xlua_gl(lua_State *L);
//xlua_mainthread
void* lapi_xlua_mainthread(lua_State *L);
//xlua_objlen
uint32_t lapi_xlua_objlen (lua_State *L, int idx);
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

void lapi_init(lapi_func_ptr* func_array);

EXTERN_C_END
#endif