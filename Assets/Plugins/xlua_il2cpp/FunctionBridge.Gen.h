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
struct S_i4i4_ {
    int32_t p0;
    int32_t p1;
};
struct o {
    struct S_i4i4_ p0;
    int32_t p1;
    int32_t p2;
    int32_t p3;
    int32_t p4;
    void* p5;
    int32_t p6;
};
struct S_r4r4r4_ {
    float p0;
    float p1;
    float p2;
};
struct i4 {
    int32_t p0;
};
struct s {
    int32_t p0;
    Il2CppChar p1;
};
struct O {

};
struct S__ {

};
struct S_p_ {
    void* p0;
};
struct v {

};


//System.Object ToObject(System.Type, System.Object)
static bool w_OoO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;

    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    // object
    void* p1 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp1, L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, void* p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object ToObject(System.Type, SByte)
static bool w_Ooi1(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature i1
    int8_t p1 = lapi_xlua_tointeger(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, int8_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object ToObject(System.Type, Int16)
static bool w_Ooi2(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature i2
    int16_t p1 = lapi_xlua_tointeger(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, int16_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object ToObject(System.Type, Int32)
static bool w_Ooi4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature i4
    int32_t p1 = lapi_xlua_tointeger(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, int32_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object ToObject(System.Type, Int64)
static bool w_Ooi8(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature i8
    int64_t p1 = lapi_xlua_tointeger(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, int64_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object Parse(System.Type, System.String)
static bool w_Oos(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isstring(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    // string s
    void* p1 = LuaStr2CSharpString(L, 1);

    typedef void* (*FuncToCall)(void* p0, void* p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object Parse(System.Type, System.String, Boolean)
static bool w_Oosb(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 2) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isstring(L, 1 + paramOffset)) return false;
        if (!lapi_lua_isboolean(L, 2 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    // string s
    void* p1 = LuaStr2CSharpString(L, 1);
    //signature b
    bool p2 = lapi_lua_toboolean(L, 2 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, void* p1, bool p2, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, p2, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object ToObject(System.Type, Byte)
static bool w_Oou1(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature u1
    uint8_t p1 = lapi_xlua_tointeger(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, uint8_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object ToObject(System.Type, UInt16)
static bool w_Oou2(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature u2
    uint16_t p1 = lapi_xlua_tointeger(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, uint16_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object ToObject(System.Type, UInt32)
static bool w_Oou4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature u4
    uint32_t p1 = lapi_xlua_tointeger(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, uint32_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//System.Object ToObject(System.Type, UInt64)
static bool w_Oou8(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature u8
    uint64_t p1 = lapi_xlua_tointeger(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, uint64_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//Il2CppTestStruct StaticGetStruct()
static bool w_S_i4i4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + -1) return false;

    }



    typedef struct S_i4i4_(*FuncToCall)(const void* method);
    struct S_i4i4_ ret = ((FuncToCall)methodPointer)(method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_i4i4_));
    return true;
}


//UnityEngine.Vector3 get_zero()
static bool w_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + -1) return false;

    }



    typedef struct S_r4r4r4_(*FuncToCall)(const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 Normalize(UnityEngine.Vector3)
static bool w_S_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 Scale(UnityEngine.Vector3, UnityEngine.Vector3)
static bool w_S_r4r4r4_S_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 SmoothDamp(UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.Vector3 ByRef, Single)
static bool w_S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto TIp2 = wrapData->TypeInfos[3];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 3) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isuserdata(L, 2 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 3 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);
    //LuaValToCSVal Pstruct todo 容错保护
    S_r4r4r4_* p2 = (S_r4r4r4_*)GetCSharpStructPointer(L, 2 + paramOffset, TIp2);
    if (!p2) {
        S_r4r4r4_ up2;
        memset(&up2, 0, sizeof(S_r4r4r4_));
        p2 = &up2;
    }
    //signature r4
    float p3 = lapi_lua_tonumber(L, 3 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, struct S_r4r4r4_* p2, float p3, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, p3, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 SmoothDamp(UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.Vector3 ByRef, Single, Single)
static bool w_S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto TIp2 = wrapData->TypeInfos[3];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 4) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isuserdata(L, 2 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 3 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 4 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);
    //LuaValToCSVal Pstruct todo 容错保护
    S_r4r4r4_* p2 = (S_r4r4r4_*)GetCSharpStructPointer(L, 2 + paramOffset, TIp2);
    if (!p2) {
        S_r4r4r4_ up2;
        memset(&up2, 0, sizeof(S_r4r4r4_));
        p2 = &up2;
    }
    //signature r4
    float p3 = lapi_lua_tonumber(L, 3 + paramOffset);
    //signature r4
    float p4 = lapi_lua_tonumber(L, 4 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, struct S_r4r4r4_* p2, float p3, float p4, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, p3, p4, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 SmoothDamp(UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.Vector3 ByRef, Single, Single, Single)
static bool w_S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4r4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto TIp2 = wrapData->TypeInfos[3];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 5) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isuserdata(L, 2 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 3 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 4 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 5 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);
    //LuaValToCSVal Pstruct todo 容错保护
    S_r4r4r4_* p2 = (S_r4r4r4_*)GetCSharpStructPointer(L, 2 + paramOffset, TIp2);
    if (!p2) {
        S_r4r4r4_ up2;
        memset(&up2, 0, sizeof(S_r4r4r4_));
        p2 = &up2;
    }
    //signature r4
    float p3 = lapi_lua_tonumber(L, 3 + paramOffset);
    //signature r4
    float p4 = lapi_lua_tonumber(L, 4 + paramOffset);
    //signature r4
    float p5 = lapi_lua_tonumber(L, 5 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, struct S_r4r4r4_* p2, float p3, float p4, float p5, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, p3, p4, p5, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 Slerp(UnityEngine.Vector3, UnityEngine.Vector3, Single)
static bool w_S_r4r4r4_S_r4r4r4_S_r4r4r4_r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 2) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isnumber(L, 2 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);
    //signature r4
    float p2 = lapi_lua_tonumber(L, 2 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, float p2, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 RotateTowards(UnityEngine.Vector3, UnityEngine.Vector3, Single, Single)
static bool w_S_r4r4r4_S_r4r4r4_S_r4r4r4_r4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 3) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isnumber(L, 2 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 3 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);
    //signature r4
    float p2 = lapi_lua_tonumber(L, 2 + paramOffset);
    //signature r4
    float p3 = lapi_lua_tonumber(L, 3 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, float p2, float p3, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, p3, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 ClampMagnitude(UnityEngine.Vector3, Single)
static bool w_S_r4r4r4_S_r4r4r4_r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //signature r4
    float p1 = lapi_lua_tonumber(L, 1 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, float p1, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 op_Multiply(Single, UnityEngine.Vector3)
static bool w_S_r4r4r4_r4S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
    }

    //signature r4
    float p0 = lapi_lua_tonumber(L, 0 + paramOffset);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);

    typedef struct S_r4r4r4_(*FuncToCall)(float p0, struct S_r4r4r4_ p1, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//UnityEngine.Vector3 get_normalized()
static bool w_S_r4r4r4_t(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef struct S_r4r4r4_(*FuncToCall)(void*, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(self, method);


    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));
    return true;
}


//Boolean op_Equality(UnityEngine.Vector3, UnityEngine.Vector3)
static bool w_bS_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);

    typedef bool (*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, const void* method);
    bool ret = ((FuncToCall)methodPointer)(p0, p1, method);


    lapi_lua_pushboolean(L, ret);
    return true;
}


//Boolean IsDefined(System.Type, System.Object)
static bool w_boO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;

    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    // object
    void* p1 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp1, L, 1 + paramOffset);

    typedef bool (*FuncToCall)(void* p0, void* p1, const void* method);
    bool ret = ((FuncToCall)methodPointer)(p0, p1, method);


    lapi_lua_pushboolean(L, ret);
    return true;
}


//Boolean Equals(System.Object)
static bool w_btO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef bool (*FuncToCall)(void*, void* p0, const void* method);
    bool ret = ((FuncToCall)methodPointer)(self, p0, method);


    lapi_lua_pushboolean(L, ret);
    return true;
}


//Boolean Equals(UnityEngine.Vector3)
static bool w_btS_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);

    typedef bool (*FuncToCall)(void*, struct S_r4r4r4_ p0, const void* method);
    bool ret = ((FuncToCall)methodPointer)(self, p0, method);


    lapi_lua_pushboolean(L, ret);
    return true;
}


//Boolean HasFlag(System.Enum)
static bool w_bto(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef bool (*FuncToCall)(void*, void* p0, const void* method);
    bool ret = ((FuncToCall)methodPointer)(self, p0, method);


    lapi_lua_pushboolean(L, ret);
    return true;
}


//Int32 get_StaticPropertyInt()
static bool w_i4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + -1) return false;

    }



    typedef int32_t(*FuncToCall)(const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(method);


    lapi_lua_pushinteger(L, (long long)ret);
    return true;
}


//Int32 get_PropertyInt()
static bool w_i4t(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef int32_t(*FuncToCall)(void*, const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(self, method);


    lapi_lua_pushinteger(L, (long long)ret);
    return true;
}


//Int32 CompareTo(System.Object)
static bool w_i4tO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef int32_t(*FuncToCall)(void*, void* p0, const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(self, p0, method);


    lapi_lua_pushinteger(L, (long long)ret);
    return true;
}


//Int32 get_Item(Int32)
static bool w_i4ti4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 0 + paramOffset);

    typedef int32_t(*FuncToCall)(void*, int32_t p0, const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(self, p0, method);


    lapi_lua_pushinteger(L, (long long)ret);
    return true;
}


//System.Type GetUnderlyingType(System.Type)
static bool w_oo(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, method);


    GetCppObjMapper()->TryPushObject(L, ret);
    return true;
}


//Single Magnitude(UnityEngine.Vector3)
static bool w_r4S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);

    typedef float (*FuncToCall)(struct S_r4r4r4_ p0, const void* method);
    float ret = ((FuncToCall)methodPointer)(p0, method);


    lapi_lua_pushnumber(L, (double)ret);
    return true;
}


//Single Dot(UnityEngine.Vector3, UnityEngine.Vector3)
static bool w_r4S_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);

    typedef float (*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, const void* method);
    float ret = ((FuncToCall)methodPointer)(p0, p1, method);


    lapi_lua_pushnumber(L, (double)ret);
    return true;
}


//Single SignedAngle(UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.Vector3)
static bool w_r4S_r4r4r4_S_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    auto TIp2 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 2) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!CheckIsStruct(L, 2 + paramOffset, TIp2)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    //LuaValToCSVal struct
    S_r4r4r4_ p1 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);
    //LuaValToCSVal struct
    S_r4r4r4_ p2 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 2 + paramOffset, TIp2);

    typedef float (*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, struct S_r4r4r4_ p2, const void* method);
    float ret = ((FuncToCall)methodPointer)(p0, p1, p2, method);


    lapi_lua_pushnumber(L, (double)ret);
    return true;
}


//Single get_magnitude()
static bool w_r4t(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef float (*FuncToCall)(void*, const void* method);
    float ret = ((FuncToCall)methodPointer)(self, method);


    lapi_lua_pushnumber(L, (double)ret);
    return true;
}


//Single get_Item(Int32)
static bool w_r4ti4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 0 + paramOffset);

    typedef float (*FuncToCall)(void*, int32_t p0, const void* method);
    float ret = ((FuncToCall)methodPointer)(self, p0, method);


    lapi_lua_pushnumber(L, (double)ret);
    return true;
}


//System.String GetName(System.Type, System.Object)
static bool w_soO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;

    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    // object
    void* p1 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp1, L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, void* p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);


    const Il2CppChar* utf16Ret = il2cpp::utils::StringUtils::GetChars((Il2CppString*)ret);
    std::string retStr = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16Ret);
    lapi_lua_pushstring(L, retStr.c_str());
    return true;
}


//System.String Format(System.Type, System.Object, System.String)
static bool w_soOs(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto TIp2 = wrapData->TypeInfos[3];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 2) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;

        if (!lapi_lua_isstring(L, 2 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    // object
    void* p1 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp1, L, 1 + paramOffset);
    // string s
    void* p2 = LuaStr2CSharpString(L, 2);

    typedef void* (*FuncToCall)(void* p0, void* p1, void* p2, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, p2, method);


    const Il2CppChar* utf16Ret = il2cpp::utils::StringUtils::GetChars((Il2CppString*)ret);
    std::string retStr = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16Ret);
    lapi_lua_pushstring(L, retStr.c_str());
    return true;
}


//System.String ToString()
static bool w_st(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef void* (*FuncToCall)(void*, const void* method);
    void* ret = ((FuncToCall)methodPointer)(self, method);


    const Il2CppChar* utf16Ret = il2cpp::utils::StringUtils::GetChars((Il2CppString*)ret);
    std::string retStr = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16Ret);
    lapi_lua_pushstring(L, retStr.c_str());
    return true;
}


//System.String ToString(System.IFormatProvider)
static bool w_sto(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef void* (*FuncToCall)(void*, void* p0, const void* method);
    void* ret = ((FuncToCall)methodPointer)(self, p0, method);


    const Il2CppChar* utf16Ret = il2cpp::utils::StringUtils::GetChars((Il2CppString*)ret);
    std::string retStr = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16Ret);
    lapi_lua_pushstring(L, retStr.c_str());
    return true;
}


//System.String ToString(System.String)
static bool w_sts(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isstring(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // string s
    void* p0 = LuaStr2CSharpString(L, 0);

    typedef void* (*FuncToCall)(void*, void* p0, const void* method);
    void* ret = ((FuncToCall)methodPointer)(self, p0, method);


    const Il2CppChar* utf16Ret = il2cpp::utils::StringUtils::GetChars((Il2CppString*)ret);
    std::string retStr = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16Ret);
    lapi_lua_pushstring(L, retStr.c_str());
    return true;
}


//System.String ToString(System.String, System.IFormatProvider)
static bool w_stso(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isstring(L, 0 + paramOffset)) return false;
        if (!CheckIsClass(L, 1 + paramOffset, TIp1)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // string s
    void* p0 = LuaStr2CSharpString(L, 0);
    // object
    void* p1 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp1, L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void*, void* p0, void* p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(self, p0, p1, method);


    const Il2CppChar* utf16Ret = il2cpp::utils::StringUtils::GetChars((Il2CppString*)ret);
    std::string retStr = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16Ret);
    lapi_lua_pushstring(L, retStr.c_str());
    return true;
}


//Void OrthoNormalize(UnityEngine.Vector3 ByRef, UnityEngine.Vector3 ByRef)
static bool w_vPS_r4r4r4_PS_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isuserdata(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isuserdata(L, 1 + paramOffset)) return false;
    }

    //LuaValToCSVal Pstruct todo 容错保护
    S_r4r4r4_* p0 = (S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    if (!p0) {
        S_r4r4r4_ up0;
        memset(&up0, 0, sizeof(S_r4r4r4_));
        p0 = &up0;
    }
    //LuaValToCSVal Pstruct todo 容错保护
    S_r4r4r4_* p1 = (S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);
    if (!p1) {
        S_r4r4r4_ up1;
        memset(&up1, 0, sizeof(S_r4r4r4_));
        p1 = &up1;
    }

    typedef void (*FuncToCall)(struct S_r4r4r4_* p0, struct S_r4r4r4_* p1, const void* method);
    ((FuncToCall)methodPointer)(p0, p1, method);



    return true;
}


//Void OrthoNormalize(UnityEngine.Vector3 ByRef, UnityEngine.Vector3 ByRef, UnityEngine.Vector3 ByRef)
static bool w_vPS_r4r4r4_PS_r4r4r4_PS_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    auto TIp2 = wrapData->TypeInfos[2];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 2) return false;
        if (!lapi_lua_isuserdata(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isuserdata(L, 1 + paramOffset)) return false;
        if (!lapi_lua_isuserdata(L, 2 + paramOffset)) return false;
    }

    //LuaValToCSVal Pstruct todo 容错保护
    S_r4r4r4_* p0 = (S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);
    if (!p0) {
        S_r4r4r4_ up0;
        memset(&up0, 0, sizeof(S_r4r4r4_));
        p0 = &up0;
    }
    //LuaValToCSVal Pstruct todo 容错保护
    S_r4r4r4_* p1 = (S_r4r4r4_*)GetCSharpStructPointer(L, 1 + paramOffset, TIp1);
    if (!p1) {
        S_r4r4r4_ up1;
        memset(&up1, 0, sizeof(S_r4r4r4_));
        p1 = &up1;
    }
    //LuaValToCSVal Pstruct todo 容错保护
    S_r4r4r4_* p2 = (S_r4r4r4_*)GetCSharpStructPointer(L, 2 + paramOffset, TIp2);
    if (!p2) {
        S_r4r4r4_ up2;
        memset(&up2, 0, sizeof(S_r4r4r4_));
        p2 = &up2;
    }

    typedef void (*FuncToCall)(struct S_r4r4r4_* p0, struct S_r4r4r4_* p1, struct S_r4r4r4_* p2, const void* method);
    ((FuncToCall)methodPointer)(p0, p1, p2, method);



    return true;
}


//Void set_StaticPropertyInt(Int32)
static bool w_vi4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
    }

    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 0 + paramOffset);

    typedef void (*FuncToCall)(int32_t p0, const void* method);
    ((FuncToCall)methodPointer)(p0, method);



    return true;
}


//Void StaticPrint(System.String)
static bool w_vs(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isstring(L, 0 + paramOffset)) return false;
    }

    // string s
    void* p0 = LuaStr2CSharpString(L, 0);

    typedef void (*FuncToCall)(void* p0, const void* method);
    ((FuncToCall)methodPointer)(p0, method);



    return true;
}


//Void MethodInvoke()
static bool w_vt(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef void (*FuncToCall)(void*, const void* method);
    ((FuncToCall)methodPointer)(self, method);



    return true;
}


//Void Scale(UnityEngine.Vector3)
static bool w_vtS_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //LuaValToCSVal struct
    S_r4r4r4_ p0 = *(S_r4r4r4_*)GetCSharpStructPointer(L, 0 + paramOffset, TIp0);

    typedef void (*FuncToCall)(void*, struct S_r4r4r4_ p0, const void* method);
    ((FuncToCall)methodPointer)(self, p0, method);



    return true;
}


//Void set_PropertyInt(Int32)
static bool w_vti4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 0 + paramOffset);

    typedef void (*FuncToCall)(void*, int32_t p0, const void* method);
    ((FuncToCall)methodPointer)(self, p0, method);



    return true;
}


//Void DefaultParam(Int32, System.String)
static bool w_vti4Ds(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp1 = wrapData->TypeInfos[0];
    if (true) {
        auto length = lapi_lua_gettop(L);
        if (length < paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (length > 1 + paramOffset && !lapi_lua_isstring(L, 1 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 0 + paramOffset);
    //LuaValToCSVal string default
    
    void* p1 = xlua::OptionalParameter<void*>::GetString(L, 1 + paramOffset, method, wrapData, 1);

    typedef void (*FuncToCall)(void*, int32_t p0, void* p1, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, method);

    return true;
}


//Void set_Item(Int32, Int32)
static bool w_vti4i4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 0 + paramOffset);
    //signature i4
    int32_t p1 = lapi_xlua_tointeger(L, 1 + paramOffset);

    typedef void (*FuncToCall)(void*, int32_t p0, int32_t p1, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, method);



    return true;
}


//Void set_Item(Int32, Single)
static bool w_vti4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = lapi_xlua_tointeger(L, 0 + paramOffset);
    //signature r4
    float p1 = lapi_lua_tonumber(L, 1 + paramOffset);

    typedef void (*FuncToCall)(void*, int32_t p0, float p1, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, method);



    return true;
}


//Void .ctor(Single, Single)
static bool w_vtr4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature r4
    float p0 = lapi_lua_tonumber(L, 0 + paramOffset);
    //signature r4
    float p1 = lapi_lua_tonumber(L, 1 + paramOffset);

    typedef void (*FuncToCall)(void*, float p0, float p1, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, method);



    return true;
}


//Void Set(Single, Single, Single)
static bool w_vtr4r4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 2) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 2 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature r4
    float p0 = lapi_lua_tonumber(L, 0 + paramOffset);
    //signature r4
    float p1 = lapi_lua_tonumber(L, 1 + paramOffset);
    //signature r4
    float p2 = lapi_lua_tonumber(L, 2 + paramOffset);

    typedef void (*FuncToCall)(void*, float p0, float p1, float p2, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, p2, method);



    return true;
}


//Void Print(System.String)
static bool w_vts(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    if (checkLuaArgument) {
        auto length = lapi_lua_gettop(L);
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isstring(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // string s
    void* p0 = LuaStr2CSharpString(L, 0);

    typedef void (*FuncToCall)(void*, void* p0, const void* method);
    ((FuncToCall)methodPointer)(self, p0, method);



    return true;
}


static WrapFuncInfo g_wrapFuncInfos[] = {
    {"OoO", (WrapFuncPtr)w_OoO},
    {"Ooi1", (WrapFuncPtr)w_Ooi1},
    {"Ooi2", (WrapFuncPtr)w_Ooi2},
    {"Ooi4", (WrapFuncPtr)w_Ooi4},
    {"Ooi8", (WrapFuncPtr)w_Ooi8},
    {"Oos", (WrapFuncPtr)w_Oos},
    {"Oosb", (WrapFuncPtr)w_Oosb},
    {"Oou1", (WrapFuncPtr)w_Oou1},
    {"Oou2", (WrapFuncPtr)w_Oou2},
    {"Oou4", (WrapFuncPtr)w_Oou4},
    {"Oou8", (WrapFuncPtr)w_Oou8},
    {"S_i4i4_", (WrapFuncPtr)w_S_i4i4_},
    {"S_r4r4r4_", (WrapFuncPtr)w_S_r4r4r4_},
    {"S_r4r4r4_S_r4r4r4_", (WrapFuncPtr)w_S_r4r4r4_S_r4r4r4_},
    {"S_r4r4r4_S_r4r4r4_S_r4r4r4_", (WrapFuncPtr)w_S_r4r4r4_S_r4r4r4_S_r4r4r4_},
    {"S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4", (WrapFuncPtr)w_S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4},
    {"S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4r4", (WrapFuncPtr)w_S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4r4},
    {"S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4r4r4", (WrapFuncPtr)w_S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4r4r4},
    {"S_r4r4r4_S_r4r4r4_S_r4r4r4_r4", (WrapFuncPtr)w_S_r4r4r4_S_r4r4r4_S_r4r4r4_r4},
    {"S_r4r4r4_S_r4r4r4_S_r4r4r4_r4r4", (WrapFuncPtr)w_S_r4r4r4_S_r4r4r4_S_r4r4r4_r4r4},
    {"S_r4r4r4_S_r4r4r4_r4", (WrapFuncPtr)w_S_r4r4r4_S_r4r4r4_r4},
    {"S_r4r4r4_r4S_r4r4r4_", (WrapFuncPtr)w_S_r4r4r4_r4S_r4r4r4_},
    {"S_r4r4r4_t", (WrapFuncPtr)w_S_r4r4r4_t},
    {"bS_r4r4r4_S_r4r4r4_", (WrapFuncPtr)w_bS_r4r4r4_S_r4r4r4_},
    {"boO", (WrapFuncPtr)w_boO},
    {"btO", (WrapFuncPtr)w_btO},
    {"btS_r4r4r4_", (WrapFuncPtr)w_btS_r4r4r4_},
    {"bto", (WrapFuncPtr)w_bto},
    {"i4", (WrapFuncPtr)w_i4},
    {"i4t", (WrapFuncPtr)w_i4t},
    {"i4tO", (WrapFuncPtr)w_i4tO},
    {"i4ti4", (WrapFuncPtr)w_i4ti4},
    {"oo", (WrapFuncPtr)w_oo},
    {"r4S_r4r4r4_", (WrapFuncPtr)w_r4S_r4r4r4_},
    {"r4S_r4r4r4_S_r4r4r4_", (WrapFuncPtr)w_r4S_r4r4r4_S_r4r4r4_},
    {"r4S_r4r4r4_S_r4r4r4_S_r4r4r4_", (WrapFuncPtr)w_r4S_r4r4r4_S_r4r4r4_S_r4r4r4_},
    {"r4t", (WrapFuncPtr)w_r4t},
    {"r4ti4", (WrapFuncPtr)w_r4ti4},
    {"soO", (WrapFuncPtr)w_soO},
    {"soOs", (WrapFuncPtr)w_soOs},
    {"st", (WrapFuncPtr)w_st},
    {"sto", (WrapFuncPtr)w_sto},
    {"sts", (WrapFuncPtr)w_sts},
    {"stso", (WrapFuncPtr)w_stso},
    {"vPS_r4r4r4_PS_r4r4r4_", (WrapFuncPtr)w_vPS_r4r4r4_PS_r4r4r4_},
    {"vPS_r4r4r4_PS_r4r4r4_PS_r4r4r4_", (WrapFuncPtr)w_vPS_r4r4r4_PS_r4r4r4_PS_r4r4r4_},
    {"vi4", (WrapFuncPtr)w_vi4},
    {"vs", (WrapFuncPtr)w_vs},
    {"vt", (WrapFuncPtr)w_vt},
    {"vtS_r4r4r4_", (WrapFuncPtr)w_vtS_r4r4r4_},
    {"vti4", (WrapFuncPtr)w_vti4},
    {"vti4Ds", (WrapFuncPtr)w_vti4Ds},
    {"vti4i4", (WrapFuncPtr)w_vti4i4},
    {"vti4r4", (WrapFuncPtr)w_vti4r4},
    {"vtr4r4", (WrapFuncPtr)w_vtr4r4},
    {"vtr4r4r4", (WrapFuncPtr)w_vtr4r4r4},
    {"vts", (WrapFuncPtr)w_vts},
    {nullptr, nullptr}
};

;
static BridgeFuncInfo g_bridgeFuncInfos[] = {

    {nullptr, nullptr}
};


static void ifg_S_i4i4_(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {



    struct S_i4i4_ ret;
xlua:GetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &ret);
    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_i4i4_));

}

static void ifs_S_i4i4_(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {



    //LuaValToCSVal struct
    S_i4i4_ p = *(S_i4i4_*)GetCSharpStructPointer(L, 3, TIp);
xlua:SetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &p);
}

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

static void ifg_r4(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {



    float ret;

xlua:GetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &ret);

    lapi_lua_pushnumber(L, (double)ret);
}

static void ifs_r4(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {



    //signature r4
    float p = lapi_lua_tonumber(L, 3);
xlua:SetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &p);
}

static void ifg_tS_i4i4_(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {

    auto self = GetCppObjMapper()->ToCppObj_Field(L, 1);

    struct S_i4i4_ ret;
xlua:GetFieldValue(self, (FieldInfo*)fieldInfo, offset, &ret);
    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_i4i4_));

}

static void ifs_tS_i4i4_(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {

    auto self = GetCppObjMapper()->ToCppObj_Field(L, 1);

    //LuaValToCSVal struct
    S_i4i4_ p = *(S_i4i4_*)GetCSharpStructPointer(L, 3, TIp);
xlua:SetFieldValue(self, (FieldInfo*)fieldInfo, offset, &p);
}

static void ifg_ti4(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {

    auto self = GetCppObjMapper()->ToCppObj_Field(L, 1);

    int32_t ret;

xlua:GetFieldValue(self, (FieldInfo*)fieldInfo, offset, &ret);

    lapi_lua_pushinteger(L, (long long)ret);
}

static void ifs_ti4(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {

    auto self = GetCppObjMapper()->ToCppObj_Field(L, 1);

    //signature i4
    int32_t p = lapi_xlua_tointeger(L, 3);
xlua:SetFieldValue(self, (FieldInfo*)fieldInfo, offset, &p);
}

static void ifg_tr4(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {

    auto self = GetCppObjMapper()->ToCppObj_Field(L, 1);

    float ret;

xlua:GetFieldValue(self, (FieldInfo*)fieldInfo, offset, &ret);

    lapi_lua_pushnumber(L, (double)ret);
}

static void ifs_tr4(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {

    auto self = GetCppObjMapper()->ToCppObj_Field(L, 1);

    //signature r4
    float p = lapi_lua_tonumber(L, 3);
xlua:SetFieldValue(self, (FieldInfo*)fieldInfo, offset, &p);
};

static FieldWrapFuncInfo g_fieldWrapFuncInfos[] = {
    {"S_i4i4_", (FieldWrapFuncPtr)ifg_S_i4i4_, (FieldWrapFuncPtr)ifs_S_i4i4_},
    {"i4", (FieldWrapFuncPtr)ifg_i4, (FieldWrapFuncPtr)ifs_i4},
    {"r4", (FieldWrapFuncPtr)ifg_r4, (FieldWrapFuncPtr)ifs_r4},
    {"tS_i4i4_", (FieldWrapFuncPtr)ifg_tS_i4i4_, (FieldWrapFuncPtr)ifs_tS_i4i4_},
    {"ti4", (FieldWrapFuncPtr)ifg_ti4, (FieldWrapFuncPtr)ifs_ti4},
    {"tr4", (FieldWrapFuncPtr)ifg_tr4, (FieldWrapFuncPtr)ifs_tr4},
    {nullptr, nullptr, nullptr}
};
