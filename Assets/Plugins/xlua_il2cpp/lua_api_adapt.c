#include "lua_api_adapt.h"
#include "stddef.h"
EXTERN_C_START
//genBegin
lapi_lua_gettopType lapi_lua_gettop;
lapi_lua_settopType lapi_lua_settop;
lapi_lua_removeType lapi_lua_remove;
lapi_lua_insertType lapi_lua_insert;
lapi_lua_replaceType lapi_lua_replace;
lapi_lua_copyType lapi_lua_copy;
lapi_lua_pushvalueType lapi_lua_pushvalue;
lapi_lua_typeType lapi_lua_type;
lapi_lua_typenameType lapi_lua_typename;
lapi_lapi_lua_upvalueindexType lapi_lapi_lua_upvalueindex;
lapi_lua_isnumberType lapi_lua_isnumber;
lapi_lua_isstringType lapi_lua_isstring;
lapi_lua_iscfunctionType lapi_lua_iscfunction;
lapi_lua_isintegerType lapi_lua_isinteger;
lapi_lua_isuserdataType lapi_lua_isuserdata;
lapi_lua_tonumberType lapi_lua_tonumber;
lapi_lua_touserdataType lapi_lua_touserdata;
lapi_lua_tolstringType lapi_lua_tolstring;
lapi_lua_tobooleanType lapi_lua_toboolean;
lapi_lua_topointerType lapi_lua_topointer;
lapi_xlua_pushintegerType lapi_xlua_pushinteger;
lapi_xlua_tointegerType lapi_xlua_tointeger;
lapi_xlua_touintType lapi_xlua_touint;
lapi_xlua_pushuintType lapi_xlua_pushuint;
lapi_lua_pushint64Type lapi_lua_pushint64;
lapi_lua_pushuint64Type lapi_lua_pushuint64;
lapi_lua_isint64Type lapi_lua_isint64;
lapi_lua_isuint64Type lapi_lua_isuint64;
lapi_lua_toint64Type lapi_lua_toint64;
lapi_lua_touint64Type lapi_lua_touint64;
lapi_lua_setupvalueType lapi_lua_setupvalue;
lapi_lua_getupvalueType lapi_lua_getupvalue;
lapi_xlua_callType lapi_xlua_call;
lapi_lua_pcallType lapi_lua_pcall;
lapi_pcall_prepareType lapi_pcall_prepare;
lapi_luaL_errorType lapi_luaL_error;
lapi_load_error_funcType lapi_load_error_func;
lapi_lua_gettableType lapi_lua_gettable;
lapi_lua_settableType lapi_lua_settable;
lapi_lua_createtableType lapi_lua_createtable;
lapi_lua_pushcclosureType lapi_lua_pushcclosure;
lapi_lua_pushlightuserdataType lapi_lua_pushlightuserdata;
lapi_lua_pushbooleanType lapi_lua_pushboolean;
lapi_lua_pushstringType lapi_lua_pushstring;
lapi_lua_pushlstringType lapi_lua_pushlstring;
lapi_lua_pushnumberType lapi_lua_pushnumber;
lapi_lua_pushnilType lapi_lua_pushnil;
lapi_lua_setmetatableType lapi_lua_setmetatable;
lapi_xlua_getglobalType lapi_xlua_getglobal;
lapi_xlua_setglobalType lapi_xlua_setglobal;
lapi_xlua_get_registry_indexType lapi_xlua_get_registry_index;
lapi_xlua_rawgetiType lapi_xlua_rawgeti;
lapi_xlua_rawsetiType lapi_xlua_rawseti;
lapi_lua_rawgetType lapi_lua_rawget;
lapi_lua_rawsetType lapi_lua_rawset;
lapi_luaL_refType lapi_luaL_ref;
lapi_xlua_tagType lapi_xlua_tag;
lapi_luaL_newmetatableType lapi_luaL_newmetatable;
lapi_xlua_glType lapi_xlua_gl;
lapi_xlua_mainthreadType lapi_xlua_mainthread;
lapi_xlua_objlenType lapi_xlua_objlen;
lapi_lua_newuserdatauvType lapi_lua_newuserdatauv;
lapi_xlua_tryget_cachedudType lapi_xlua_tryget_cachedud;
//genEnd

void lapi_init(lapi_func_ptr* func_array){
//gen1
lapi_lua_gettop = (lapi_lua_gettopType)func_array[0];
lapi_lua_settop = (lapi_lua_settopType)func_array[1];
lapi_lua_remove = (lapi_lua_removeType)func_array[2];
lapi_lua_insert = (lapi_lua_insertType)func_array[3];
lapi_lua_replace = (lapi_lua_replaceType)func_array[4];
lapi_lua_copy = (lapi_lua_copyType)func_array[5];
lapi_lua_pushvalue = (lapi_lua_pushvalueType)func_array[6];
lapi_lua_type = (lapi_lua_typeType)func_array[7];
lapi_lua_typename = (lapi_lua_typenameType)func_array[8];
lapi_lapi_lua_upvalueindex = (lapi_lapi_lua_upvalueindexType)func_array[9];
lapi_lua_isnumber = (lapi_lua_isnumberType)func_array[10];
lapi_lua_isstring = (lapi_lua_isstringType)func_array[11];
lapi_lua_iscfunction = (lapi_lua_iscfunctionType)func_array[12];
lapi_lua_isinteger = (lapi_lua_isintegerType)func_array[13];
lapi_lua_isuserdata = (lapi_lua_isuserdataType)func_array[14];
lapi_lua_tonumber = (lapi_lua_tonumberType)func_array[15];
lapi_lua_touserdata = (lapi_lua_touserdataType)func_array[16];
lapi_lua_tolstring = (lapi_lua_tolstringType)func_array[17];
lapi_lua_toboolean = (lapi_lua_tobooleanType)func_array[18];
lapi_lua_topointer = (lapi_lua_topointerType)func_array[19];
lapi_xlua_pushinteger = (lapi_xlua_pushintegerType)func_array[20];
lapi_xlua_tointeger = (lapi_xlua_tointegerType)func_array[21];
lapi_xlua_touint = (lapi_xlua_touintType)func_array[22];
lapi_xlua_pushuint = (lapi_xlua_pushuintType)func_array[23];
lapi_lua_pushint64 = (lapi_lua_pushint64Type)func_array[24];
lapi_lua_pushuint64 = (lapi_lua_pushuint64Type)func_array[25];
lapi_lua_isint64 = (lapi_lua_isint64Type)func_array[26];
lapi_lua_isuint64 = (lapi_lua_isuint64Type)func_array[27];
lapi_lua_toint64 = (lapi_lua_toint64Type)func_array[28];
lapi_lua_touint64 = (lapi_lua_touint64Type)func_array[29];
lapi_lua_setupvalue = (lapi_lua_setupvalueType)func_array[30];
lapi_lua_getupvalue = (lapi_lua_getupvalueType)func_array[31];
lapi_xlua_call = (lapi_xlua_callType)func_array[32];
lapi_lua_pcall = (lapi_lua_pcallType)func_array[33];
lapi_pcall_prepare = (lapi_pcall_prepareType)func_array[34];
lapi_luaL_error = (lapi_luaL_errorType)func_array[35];
lapi_load_error_func = (lapi_load_error_funcType)func_array[36];
lapi_lua_gettable = (lapi_lua_gettableType)func_array[37];
lapi_lua_settable = (lapi_lua_settableType)func_array[38];
lapi_lua_createtable = (lapi_lua_createtableType)func_array[39];
lapi_lua_pushcclosure = (lapi_lua_pushcclosureType)func_array[40];
lapi_lua_pushlightuserdata = (lapi_lua_pushlightuserdataType)func_array[41];
lapi_lua_pushboolean = (lapi_lua_pushbooleanType)func_array[42];
lapi_lua_pushstring = (lapi_lua_pushstringType)func_array[43];
lapi_lua_pushlstring = (lapi_lua_pushlstringType)func_array[44];
lapi_lua_pushnumber = (lapi_lua_pushnumberType)func_array[45];
lapi_lua_pushnil = (lapi_lua_pushnilType)func_array[46];
lapi_lua_setmetatable = (lapi_lua_setmetatableType)func_array[47];
lapi_xlua_getglobal = (lapi_xlua_getglobalType)func_array[48];
lapi_xlua_setglobal = (lapi_xlua_setglobalType)func_array[49];
lapi_xlua_get_registry_index = (lapi_xlua_get_registry_indexType)func_array[50];
lapi_xlua_rawgeti = (lapi_xlua_rawgetiType)func_array[51];
lapi_xlua_rawseti = (lapi_xlua_rawsetiType)func_array[52];
lapi_lua_rawget = (lapi_lua_rawgetType)func_array[53];
lapi_lua_rawset = (lapi_lua_rawsetType)func_array[54];
lapi_luaL_ref = (lapi_luaL_refType)func_array[55];
lapi_xlua_tag = (lapi_xlua_tagType)func_array[56];
lapi_luaL_newmetatable = (lapi_luaL_newmetatableType)func_array[57];
lapi_xlua_gl = (lapi_xlua_glType)func_array[58];
lapi_xlua_mainthread = (lapi_xlua_mainthreadType)func_array[59];
lapi_xlua_objlen = (lapi_xlua_objlenType)func_array[60];
lapi_lua_newuserdatauv = (lapi_lua_newuserdatauvType)func_array[61];
lapi_xlua_tryget_cachedud = (lapi_xlua_tryget_cachedudType)func_array[62];
//end1

}

EXTERN_C_END