#ifndef LUA_API_ADAPT
#define LUA_API_ADAPT
typedef void* lua_State;
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
#define LUA_INTEGER		long long

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


/* type for integer functions */
typedef LUA_INTEGER lua_Integer;


typedef void (*lapi_func_ptr)(void);

//lua_touserdata
void* lapi_lua_touserdata(lua_State*L, int idx);
//lua_type
int lapi_lua_type(lua_State*L, int idx);

//lua_gettop
int lapi_lua_gettop(lua_State*L);

//lua_upvalueindex
int lapi_lua_upvalueindex(int idx);

//xlua_pushcsobj_ptr
void lapi_xlua_pushcsobj_ptr(lua_State*L, void* ptr, int meta_ref, int key, int need_cache, int cache_ref);

//lua_isnumber
int lapi_lua_isnumber(lua_State*L, int idx);

//lua_isstring
int lapi_lua_isstring(lua_State*L, int idx);

//lua_iscfunction
int lapi_lua_iscfunction(lua_State*L, int idx);


//lua_isinteger
int lapi_lua_isinteger(lua_State*L, int idx);

//lua_isuserdata
int lapi_lua_isuserdata(lua_State*L, int idx);

//lua_typename
const char * lapi_lua_typename(lua_State*L, int idx);

//lua_tonumber
lua_Number lapi_lua_tonumber(lua_State*L, int idx);

//lua_tolstring
const char * lapi_lua_tolstring(lua_State*L, int idx);

//lua_toboolean
int lapi_lua_toboolean(lua_State*L, int idx);

//lua_topointer
const void * lapi_lua_topointer(lua_State*L, int idx);

//xlua_tryget_cachedud
int lapi_xlua_tryget_cachedud(lua_State*L, int key, int cache_ref);

void lapi_init(lapi_func_ptr* func_array);
EXTERN_C_END
#endif