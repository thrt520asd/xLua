#ifndef LUA_API_ADAPT
#define LUA_API_ADAPT
typedef void* lua_State;
typedef int (*lua_CFunction) (lua_State *L);
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

//lua_typenamestackIdx
#define lua_typename_stackIdx(L,n)(lapi_lua_typename(L, lapi_lua_type(L,n)))

//lua_tonumber
lua_Number lapi_lua_tonumber(lua_State*L, int idx);

//xlua_tointeger
int lapi_xlua_tointeger(lua_State* L, int idx);

//lua_tolstring
const char * lapi_lua_tolstring(lua_State*L, int idx);

//lua_toboolean
int lapi_lua_toboolean(lua_State*L, int idx);

//lua_topointer
const void * lapi_lua_topointer(lua_State*L, int idx);

//xlua_tryget_cachedud
int lapi_xlua_tryget_cachedud(lua_State*L, int key, int cache_ref);

//xlua_getcsobj_ptr
void* lapi_xlua_getcsobj_ptr(lua_State*L, int index);

//lua_pushcclosure
void lapi_lua_pushcclosure(lua_State* L, lua_CFunction fn, int n);

//lua_setupvalue
const char* lapi_lua_setupvalue(lua_State* L, int funcindex, int n);

//lua_getupvalue
const char* lapi_lua_getupvalue(lua_State* L, int index);

// lua_isnil
#define lapi_lua_isnil(L,n)		(lapi_lua_type(L, (n)) == LUA_TNIL)

//lua_pushvalue
void lapi_lua_pushvalue(lua_State* L, int index);

//lua_gettable
int lapi_lua_gettable(lua_State* L, int index);

//lua_call
void lapi_lua_call(lua_State* L, int nargs, int nresults);

//lua_settop
void lapi_lua_settop(lua_State* L, int amount);

//lua_pop
#define lapi_lua_pop(L,n)		lapi_lua_settop(L, -(n)-1);

//lua_pushlightuserdata
void lapi_lua_pushlightuserdata(lua_State* L, void*p);

//lua_settable
void lapi_lua_settable(lua_State* L, int idx);

//lua_createtable
void lapi_lua_createtable(lua_State* L,int narr, int nrec);

// lua_pushboolean
void lapi_lua_pushboolean(lua_State* L,int b);

// lua_pushstring
void lapi_lua_pushstring(lua_State* L,const char * c);

// lua_pushlstring
void lapi_lua_pushlstring(lua_State* L,const char * c, size_t len);

// lua_pushnumber
void lapi_lua_pushnumber(lua_State* L, double d);

// lua_pushinteger
void lapi_lua_pushinteger(lua_State* L, long long d);

// lua_pushnil
void lapi_lua_pushnil(lua_State* L);

void lapi_init(lapi_func_ptr* func_array);

EXTERN_C_END
#endif