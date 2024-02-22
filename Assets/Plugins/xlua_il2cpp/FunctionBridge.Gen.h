// Auto Gen
#if !__SNC__
#ifndef __has_feature 
#define __has_feature(x) 0 
#endif
#endif

#if _MSC_VER
typedef wchar_t Il2CppChar;
#elif __has_feature(cxx_unicode_literals)
typedef char16_t Il2CppChar;
#else
typedef uint16_t Il2CppChar;
#endif
struct o{
    int32_t p0;
	int32_t p1;
	int32_t p2;
	void* p3;
	int32_t p4;
};
struct s{
    int32_t p0;
	Il2CppChar p1;
};
struct v{
    
};


//Int32 get_StaticPropertyInt()
static bool w_i4(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int index = 1) {
    
    if(checkLuaArgument){
        auto length = lapi_lua_gettop(L);
        if (length != 0) return false;
        
    }
    
    
    
    typedef int32_t (*FuncToCall)(const void* method);
    int32_t ret = ((FuncToCall)methodPointer)( method);
    

    lapi_lua_pushinteger(L, (long long)ret);
    return true;
}


//Int32 get_PropertyInt()
static bool w_i4t(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int index = 1) {
    
    if(checkLuaArgument){
        auto length = lapi_lua_gettop(L);
        if (length != 1) return false;
        
    }
    auto self = lapi_xlua_getcsobj_ptr(L, 1);
    
    
    typedef int32_t (*FuncToCall)(void*, const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(self,  method);
    

    lapi_lua_pushinteger(L, (long long)ret);
    return true;
}


//Int32 get_Item(Int32)
static bool w_i4ti4(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int index = 1) {
    
    if(checkLuaArgument){
        auto length = lapi_lua_gettop(L);
        if (length != 2) return false;
        if (!lapi_lua_isinteger(L, 2)) return false;
    }
    auto self = lapi_xlua_getcsobj_ptr(L, 1);
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 2);
    
    typedef int32_t (*FuncToCall)(void*, int32_t p0, const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(self,  p0, method);
    

    lapi_lua_pushinteger(L, (long long)ret);
    return true;
}


//Void set_StaticPropertyInt(Int32)
static bool w_vi4(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int index = 1) {
    
    if(checkLuaArgument){
        auto length = lapi_lua_gettop(L);
        if (length != 1) return false;
        if (!lapi_lua_isinteger(L, 1)) return false;
    }
    
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 1);
    
    typedef void (*FuncToCall)(int32_t p0, const void* method);
    ((FuncToCall)methodPointer)( p0, method);
    

    
    return true;
}


//Void StaticPrint(System.String)
static bool w_vs(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int index = 1) {
    auto TIp0 = wrapData->TypeInfos[0];
    if(checkLuaArgument){
        auto length = lapi_lua_gettop(L);
        if (length != 1) return false;
        if (!lapi_lua_isstring(L, 1)) return false;
    }
    
    // string s
    const char* str1 =  lapi_lua_tolstring(L, 1);
    auto p0 = il2cpp::vm::String::New(str1);

    
    typedef void (*FuncToCall)(void* p0, const void* method);
    ((FuncToCall)methodPointer)( p0, method);
    

    
    return true;
}


//Void set_PropertyInt(Int32)
static bool w_vti4(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int index = 1) {
    
    if(checkLuaArgument){
        auto length = lapi_lua_gettop(L);
        if (length != 2) return false;
        if (!lapi_lua_isinteger(L, 2)) return false;
    }
    auto self = lapi_xlua_getcsobj_ptr(L, 1);
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 2);
    
    typedef void (*FuncToCall)(void*, int32_t p0, const void* method);
    ((FuncToCall)methodPointer)(self,  p0, method);
    

    
    return true;
}


//Void set_Item(Int32, Int32)
static bool w_vti4i4(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int index = 1) {
    
    if(checkLuaArgument){
        auto length = lapi_lua_gettop(L);
        if (length != 3) return false;
        if (!lapi_lua_isinteger(L, 2)) return false;
		if (!lapi_lua_isinteger(L, 3)) return false;
    }
    auto self = lapi_xlua_getcsobj_ptr(L, 1);
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 2);
	//signature i4
    int32_t p1 = lapi_xlua_tointeger(L, 3);
    
    typedef void (*FuncToCall)(void*, int32_t p0, int32_t p1, const void* method);
    ((FuncToCall)methodPointer)(self,  p0, p1, method);
    

    
    return true;
}


//Void Print(System.String)
static bool w_vts(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int index = 1) {
    auto TIp0 = wrapData->TypeInfos[0];
    if(checkLuaArgument){
        auto length = lapi_lua_gettop(L);
        if (length != 2) return false;
        if (!lapi_lua_isstring(L, 2)) return false;
    }
    auto self = lapi_xlua_getcsobj_ptr(L, 1);
    // string s
    const char* str2 =  lapi_lua_tolstring(L, 2);
    auto p0 = il2cpp::vm::String::New(str2);

    
    typedef void (*FuncToCall)(void*, void* p0, const void* method);
    ((FuncToCall)methodPointer)(self,  p0, method);
    

    
    return true;
}


static WrapFuncInfo g_wrapFuncInfos[] = {
    {"i4", (WrapFuncPtr)w_i4},
	{"i4t", (WrapFuncPtr)w_i4t},
	{"i4ti4", (WrapFuncPtr)w_i4ti4},
	{"vi4", (WrapFuncPtr)w_vi4},
	{"vs", (WrapFuncPtr)w_vs},
	{"vti4", (WrapFuncPtr)w_vti4},
	{"vti4i4", (WrapFuncPtr)w_vti4i4},
	{"vts", (WrapFuncPtr)w_vts},
    {nullptr, nullptr}
};

;
static BridgeFuncInfo g_bridgeFuncInfos[] = {
    
    {nullptr, nullptr}
};


static void ifg_i4(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {
    
    

    int32_t ret;

    xlua:GetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &ret);
    
    lapi_lua_pushinteger(L, (long long)ret);
}

static void ifs_i4(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {
    
    

    //signature i4
    int32_t p = lapi_xlua_tointeger(L, 3);
    xlua:SetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &p);
}

static void ifg_ti4(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {
    
    auto self = lapi_xlua_getcsobj_ptr(L, 1);

    int32_t ret;

    xlua:GetFieldValue(self, (FieldInfo*)fieldInfo, offset, &ret);
    
    lapi_lua_pushinteger(L, (long long)ret);
}

static void ifs_ti4(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {
    
    auto self = lapi_xlua_getcsobj_ptr(L, 1);

    //signature i4
    int32_t p = lapi_xlua_tointeger(L, 3);
    xlua:SetFieldValue(self, (FieldInfo*)fieldInfo, offset, &p);
};

static FieldWrapFuncInfo g_fieldWrapFuncInfos[] = {
    {"i4", (FieldWrapFuncPtr)ifg_i4, (FieldWrapFuncPtr)ifs_i4},
	{"ti4", (FieldWrapFuncPtr)ifg_ti4, (FieldWrapFuncPtr)ifs_ti4},
    {nullptr, nullptr, nullptr}
};
            