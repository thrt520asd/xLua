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
struct S_i4s_ {
    int32_t p0;
    void* p1;
};
struct v {

};

//System.Object ToObject(System.Type, System.Object)
static int w_OoO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
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


    return 1;
}

//System.Object ToObject(System.Type, SByte)
static int w_Ooi1(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature i1
    int8_t p1 = converter::Converter<int8_t>::toCpp(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, int8_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//System.Object ToObject(System.Type, Int16)
static int w_Ooi2(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature i2
    int16_t p1 = converter::Converter<int16_t>::toCpp(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, int16_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//System.Object ToObject(System.Type, Int32)
static int w_Ooi4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature i4
    int32_t p1 = converter::Converter<int32_t>::toCpp(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, int32_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//System.Object ToObject(System.Type, Int64)
static int w_Ooi8(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature i8
    int64_t p1 = converter::Converter<int64_t>::toCpp(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, int64_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//System.Object Parse(System.Type, System.String)
static int w_Oos(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
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


    return 1;
}

//System.Object Parse(System.Type, System.String, Boolean)
static int w_Oosb(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
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
    bool p2 = converter::Converter<bool>::toCpp(L, 2 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, void* p1, bool p2, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, p2, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//System.Object ToObject(System.Type, Byte)
static int w_Oou1(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature u1
    uint8_t p1 = converter::Converter<uint8_t>::toCpp(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, uint8_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//System.Object ToObject(System.Type, UInt16)
static int w_Oou2(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature u2
    uint16_t p1 = converter::Converter<uint16_t>::toCpp(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, uint16_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//System.Object ToObject(System.Type, UInt32)
static int w_Oou4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature u4
    uint32_t p1 = converter::Converter<uint32_t>::toCpp(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, uint32_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//System.Object ToObject(System.Type, UInt64)
static int w_Oou8(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    //signature u8
    uint64_t p1 = converter::Converter<uint64_t>::toCpp(L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, uint64_t p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//Il2CppTestStruct StaticGetStruct()
static int w_S_i4i4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }



    typedef struct S_i4i4_(*FuncToCall)(const void* method);
    struct S_i4i4_ ret = ((FuncToCall)methodPointer)(method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_i4i4_));


    return 1;
}

//UnityEngine.Vector3 get_zero()
static int w_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }



    typedef struct S_r4r4r4_(*FuncToCall)(const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));


    return 1;
}

//UnityEngine.Vector3 Normalize(UnityEngine.Vector3)
static int w_S_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));


    return 1;
}

//UnityEngine.Vector3 Scale(UnityEngine.Vector3, UnityEngine.Vector3)
static int w_S_r4r4r4_S_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));


    return 1;
}

//UnityEngine.Vector3 SmoothDamp(UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.Vector3 ByRef, Single)
static int w_S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto TIp2 = wrapData->TypeInfos[3];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 3) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isuserdata(L, 2 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 3 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};
    //LuaValToCSVal Pstruct
    S_r4r4r4_* p2 = nullptr;
    p2 = (S_r4r4r4_*)GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 2 + paramOffset, TIp2);
    if (!p2) {
        S_r4r4r4_ up2;
        memset(&up2, 0, sizeof(S_r4r4r4_));
        p2 = &up2;
    }
    //signature r4
    float p3 = converter::Converter<float>::toCpp(L, 3 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, struct S_r4r4r4_* p2, float p3, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, p3, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));

    CSValueToLuaValue(L, TIp2, p2, sizeof(S_r4r4r4_));//refsetback S_r4r4r4_

    return 2;
}

//UnityEngine.Vector3 SmoothDamp(UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.Vector3 ByRef, Single, Single)
static int w_S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto TIp2 = wrapData->TypeInfos[3];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 4) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isuserdata(L, 2 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 3 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 4 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};
    //LuaValToCSVal Pstruct
    S_r4r4r4_* p2 = nullptr;
    p2 = (S_r4r4r4_*)GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 2 + paramOffset, TIp2);
    if (!p2) {
        S_r4r4r4_ up2;
        memset(&up2, 0, sizeof(S_r4r4r4_));
        p2 = &up2;
    }
    //signature r4
    float p3 = converter::Converter<float>::toCpp(L, 3 + paramOffset);
    //signature r4
    float p4 = converter::Converter<float>::toCpp(L, 4 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, struct S_r4r4r4_* p2, float p3, float p4, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, p3, p4, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));

    CSValueToLuaValue(L, TIp2, p2, sizeof(S_r4r4r4_));//refsetback S_r4r4r4_

    return 2;
}

//UnityEngine.Vector3 SmoothDamp(UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.Vector3 ByRef, Single, Single, Single)
static int w_S_r4r4r4_S_r4r4r4_S_r4r4r4_PS_r4r4r4_r4r4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto TIp2 = wrapData->TypeInfos[3];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 5) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isuserdata(L, 2 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 3 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 4 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 5 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};
    //LuaValToCSVal Pstruct
    S_r4r4r4_* p2 = nullptr;
    p2 = (S_r4r4r4_*)GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 2 + paramOffset, TIp2);
    if (!p2) {
        S_r4r4r4_ up2;
        memset(&up2, 0, sizeof(S_r4r4r4_));
        p2 = &up2;
    }
    //signature r4
    float p3 = converter::Converter<float>::toCpp(L, 3 + paramOffset);
    //signature r4
    float p4 = converter::Converter<float>::toCpp(L, 4 + paramOffset);
    //signature r4
    float p5 = converter::Converter<float>::toCpp(L, 5 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, struct S_r4r4r4_* p2, float p3, float p4, float p5, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, p3, p4, p5, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));

    CSValueToLuaValue(L, TIp2, p2, sizeof(S_r4r4r4_));//refsetback S_r4r4r4_

    return 2;
}

//UnityEngine.Vector3 Slerp(UnityEngine.Vector3, UnityEngine.Vector3, Single)
static int w_S_r4r4r4_S_r4r4r4_S_r4r4r4_r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 2) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isnumber(L, 2 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};
    //signature r4
    float p2 = converter::Converter<float>::toCpp(L, 2 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, float p2, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));


    return 1;
}

//UnityEngine.Vector3 RotateTowards(UnityEngine.Vector3, UnityEngine.Vector3, Single, Single)
static int w_S_r4r4r4_S_r4r4r4_S_r4r4r4_r4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 3) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!lapi_lua_isnumber(L, 2 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 3 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};
    //signature r4
    float p2 = converter::Converter<float>::toCpp(L, 2 + paramOffset);
    //signature r4
    float p3 = converter::Converter<float>::toCpp(L, 3 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, float p2, float p3, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, p2, p3, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));


    return 1;
}

//UnityEngine.Vector3 ClampMagnitude(UnityEngine.Vector3, Single)
static int w_S_r4r4r4_S_r4r4r4_r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //signature r4
    float p1 = converter::Converter<float>::toCpp(L, 1 + paramOffset);

    typedef struct S_r4r4r4_(*FuncToCall)(struct S_r4r4r4_ p0, float p1, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));


    return 1;
}

//UnityEngine.Vector3 op_Multiply(Single, UnityEngine.Vector3)
static int w_S_r4r4r4_r4S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
    }

    //signature r4
    float p0 = converter::Converter<float>::toCpp(L, 0 + paramOffset);
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};

    typedef struct S_r4r4r4_(*FuncToCall)(float p0, struct S_r4r4r4_ p1, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));


    return 1;
}

//UnityEngine.Vector3 get_normalized()
static int w_S_r4r4r4_t(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef struct S_r4r4r4_(*FuncToCall)(void*, const void* method);
    struct S_r4r4r4_ ret = ((FuncToCall)methodPointer)(self, method);

    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_r4r4r4_));


    return 1;
}

//Boolean op_Equality(UnityEngine.Vector3, UnityEngine.Vector3)
static int w_bS_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};

    typedef bool (*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, const void* method);
    bool ret = ((FuncToCall)methodPointer)(p0, p1, method);

    lapi_lua_pushboolean(L, ret);


    return 1;
}

//Boolean IsDefined(System.Type, System.Object)
static int w_boO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
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


    return 1;
}

//Boolean Equals(System.Object)
static int w_btO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef bool (*FuncToCall)(void*, void* p0, const void* method);
    bool ret = ((FuncToCall)methodPointer)(self, p0, method);

    lapi_lua_pushboolean(L, ret);


    return 1;
}

//Boolean Equals(UnityEngine.Vector3)
static int w_btS_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};

    typedef bool (*FuncToCall)(void*, struct S_r4r4r4_ p0, const void* method);
    bool ret = ((FuncToCall)methodPointer)(self, p0, method);

    lapi_lua_pushboolean(L, ret);


    return 1;
}

//Boolean HasFlag(System.Enum)
static int w_bto(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef bool (*FuncToCall)(void*, void* p0, const void* method);
    bool ret = ((FuncToCall)methodPointer)(self, p0, method);

    lapi_lua_pushboolean(L, ret);


    return 1;
}

//Int32 get_StaticPropertyInt()
static int w_i4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }



    typedef int32_t(*FuncToCall)(const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(method);

    lapi_lua_pushinteger(L, (long long)ret);


    return 1;
}

//Int32 get_PropertyInt()
static int w_i4t(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef int32_t(*FuncToCall)(void*, const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(self, method);

    lapi_lua_pushinteger(L, (long long)ret);


    return 1;
}

//Int32 CompareTo(System.Object)
static int w_i4tO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef int32_t(*FuncToCall)(void*, void* p0, const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(self, p0, method);

    lapi_lua_pushinteger(L, (long long)ret);


    return 1;
}

//Int32 get_Item(Int32)
static int w_i4ti4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = converter::Converter<int32_t>::toCpp(L, 0 + paramOffset);

    typedef int32_t(*FuncToCall)(void*, int32_t p0, const void* method);
    int32_t ret = ((FuncToCall)methodPointer)(self, p0, method);

    lapi_lua_pushinteger(L, (long long)ret);


    return 1;
}

//System.Type GetUnderlyingType(System.Type)
static int w_oo(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//Tutorial.DerivedClass op_Addition(Tutorial.DerivedClass, Tutorial.DerivedClass)
static int w_ooo(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsClass(L, 1 + paramOffset, TIp1)) return false;
    }

    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);
    // object
    void* p1 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp1, L, 1 + paramOffset);

    typedef void* (*FuncToCall)(void* p0, void* p1, const void* method);
    void* ret = ((FuncToCall)methodPointer)(p0, p1, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//Tutorial.ICalc GetCalc()
static int w_ot(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef void* (*FuncToCall)(void*, const void* method);
    void* ret = ((FuncToCall)methodPointer)(self, method);

    GetCppObjMapper()->TryPushObject(L, ret);


    return 1;
}

//Single Magnitude(UnityEngine.Vector3)
static int w_r4S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};

    typedef float (*FuncToCall)(struct S_r4r4r4_ p0, const void* method);
    float ret = ((FuncToCall)methodPointer)(p0, method);

    lapi_lua_pushnumber(L, (double)ret);


    return 1;
}

//Single Dot(UnityEngine.Vector3, UnityEngine.Vector3)
static int w_r4S_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};

    typedef float (*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, const void* method);
    float ret = ((FuncToCall)methodPointer)(p0, p1, method);

    lapi_lua_pushnumber(L, (double)ret);


    return 1;
}

//Single SignedAngle(UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.Vector3)
static int w_r4S_r4r4r4_S_r4r4r4_S_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    auto TIp2 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 2) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!CheckIsStruct(L, 1 + paramOffset, TIp1)) return false;
        if (!CheckIsStruct(L, 2 + paramOffset, TIp2)) return false;
    }

    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp1 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    S_r4r4r4_ p1 = pp1 ? *pp1 : S_r4r4r4_{};
    //LuaValToCSVal struct
    S_r4r4r4_* pp2 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 2 + paramOffset, TIp2);
    S_r4r4r4_ p2 = pp2 ? *pp2 : S_r4r4r4_{};

    typedef float (*FuncToCall)(struct S_r4r4r4_ p0, struct S_r4r4r4_ p1, struct S_r4r4r4_ p2, const void* method);
    float ret = ((FuncToCall)methodPointer)(p0, p1, p2, method);

    lapi_lua_pushnumber(L, (double)ret);


    return 1;
}

//Single get_magnitude()
static int w_r4t(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef float (*FuncToCall)(void*, const void* method);
    float ret = ((FuncToCall)methodPointer)(self, method);

    lapi_lua_pushnumber(L, (double)ret);


    return 1;
}

//Single get_Item(Int32)
static int w_r4ti4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = converter::Converter<int32_t>::toCpp(L, 0 + paramOffset);

    typedef float (*FuncToCall)(void*, int32_t p0, const void* method);
    float ret = ((FuncToCall)methodPointer)(self, p0, method);

    lapi_lua_pushnumber(L, (double)ret);


    return 1;
}

//Double ComplexFunc(Tutorial.Param1, Int32 ByRef, System.String ByRef, System.Action, System.Action ByRef)
static int w_r8tS_i4s_Pi4UPsoUPo(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp2 = wrapData->TypeInfos[1];
    auto TIp3 = wrapData->TypeInfos[2];
    auto TIp4 = wrapData->TypeInfos[3];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 4) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
        if (!lapi_lua_isuserdata(L, 1 + paramOffset)) return false;

        if (!CheckIsClass(L, 2 + paramOffset, TIp2)) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //LuaValToCSVal struct
    S_i4s_* pp0 = GetCSharpStructPointerFromLua<S_i4s_>(L, 0 + paramOffset, TIp0);
    S_i4s_ p0 = pp0 ? *pp0 : S_i4s_{};
    int32_t* p1 = nullptr; // Pi4
    int32_t up1 = converter::Converter<int32_t>::toCpp(L, 1 + paramOffset);
    p1 = &up1;
    void* up2 = nullptr; //out ref
    void** p2 = &up2;

    // object
    void* p3 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp3, L, 2 + paramOffset);
    void* up4 = nullptr; //out ref
    void** p4 = &up4;


    typedef double (*FuncToCall)(void*, struct S_i4s_ p0, int32_t* p1, void** p2, void* p3, void** p4, const void* method);
    double ret = ((FuncToCall)methodPointer)(self, p0, p1, p2, p3, p4, method);

    lapi_lua_pushnumber(L, (double)ret);

    converter::Converter<int32_t>::toScript(L, *p1);//refsetback i4
    CSAnyToLuaValue(L, *p2, TIp2);//refsetback s
    CSAnyToLuaValue(L, *p4, TIp4);//refsetback o

    return 4;
}

//System.String GetName(System.Type, System.Object)
static int w_soO(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
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


    return 1;
}

//System.String Format(System.Type, System.Object, System.String)
static int w_soOs(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto TIp2 = wrapData->TypeInfos[3];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
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


    return 1;
}

//System.String ToString()
static int w_st(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef void* (*FuncToCall)(void*, const void* method);
    void* ret = ((FuncToCall)methodPointer)(self, method);

    const Il2CppChar* utf16Ret = il2cpp::utils::StringUtils::GetChars((Il2CppString*)ret);
    std::string retStr = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16Ret);
    lapi_lua_pushstring(L, retStr.c_str());


    return 1;
}

//System.String ToString(System.IFormatProvider)
static int w_sto(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
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


    return 1;
}

//System.String ToString(System.String)
static int w_sts(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
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


    return 1;
}

//System.String ToString(System.String, System.IFormatProvider)
static int w_stso(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIret = wrapData->TypeInfos[0];
    auto TIp0 = wrapData->TypeInfos[1];
    auto TIp1 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
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


    return 1;
}

//UInt64 TestLong(Int64)
static int w_u8ti8(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i8
    int64_t p0 = converter::Converter<int64_t>::toCpp(L, 0 + paramOffset);

    typedef uint64_t(*FuncToCall)(void*, int64_t p0, const void* method);
    uint64_t ret = ((FuncToCall)methodPointer)(self, p0, method);

    lapi_lua_pushinteger(L, (long long)ret);


    return 1;
}

//Void BSFunc()
static int w_v(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }



    typedef void (*FuncToCall)(const void* method);
    ((FuncToCall)methodPointer)(method);




    return 0;
}

//Void OrthoNormalize(UnityEngine.Vector3 ByRef, UnityEngine.Vector3 ByRef)
static int w_vPS_r4r4r4_PS_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isuserdata(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isuserdata(L, 1 + paramOffset)) return false;
    }

    //LuaValToCSVal Pstruct
    S_r4r4r4_* p0 = nullptr;
    p0 = (S_r4r4r4_*)GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    if (!p0) {
        S_r4r4r4_ up0;
        memset(&up0, 0, sizeof(S_r4r4r4_));
        p0 = &up0;
    }
    //LuaValToCSVal Pstruct
    S_r4r4r4_* p1 = nullptr;
    p1 = (S_r4r4r4_*)GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    if (!p1) {
        S_r4r4r4_ up1;
        memset(&up1, 0, sizeof(S_r4r4r4_));
        p1 = &up1;
    }

    typedef void (*FuncToCall)(struct S_r4r4r4_* p0, struct S_r4r4r4_* p1, const void* method);
    ((FuncToCall)methodPointer)(p0, p1, method);



    CSValueToLuaValue(L, TIp0, p0, sizeof(S_r4r4r4_));//refsetback S_r4r4r4_
    CSValueToLuaValue(L, TIp1, p1, sizeof(S_r4r4r4_));//refsetback S_r4r4r4_

    return 2;
}

//Void OrthoNormalize(UnityEngine.Vector3 ByRef, UnityEngine.Vector3 ByRef, UnityEngine.Vector3 ByRef)
static int w_vPS_r4r4r4_PS_r4r4r4_PS_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto TIp1 = wrapData->TypeInfos[1];
    auto TIp2 = wrapData->TypeInfos[2];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 2) return false;
        if (!lapi_lua_isuserdata(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isuserdata(L, 1 + paramOffset)) return false;
        if (!lapi_lua_isuserdata(L, 2 + paramOffset)) return false;
    }

    //LuaValToCSVal Pstruct
    S_r4r4r4_* p0 = nullptr;
    p0 = (S_r4r4r4_*)GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    if (!p0) {
        S_r4r4r4_ up0;
        memset(&up0, 0, sizeof(S_r4r4r4_));
        p0 = &up0;
    }
    //LuaValToCSVal Pstruct
    S_r4r4r4_* p1 = nullptr;
    p1 = (S_r4r4r4_*)GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 1 + paramOffset, TIp1);
    if (!p1) {
        S_r4r4r4_ up1;
        memset(&up1, 0, sizeof(S_r4r4r4_));
        p1 = &up1;
    }
    //LuaValToCSVal Pstruct
    S_r4r4r4_* p2 = nullptr;
    p2 = (S_r4r4r4_*)GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 2 + paramOffset, TIp2);
    if (!p2) {
        S_r4r4r4_ up2;
        memset(&up2, 0, sizeof(S_r4r4r4_));
        p2 = &up2;
    }

    typedef void (*FuncToCall)(struct S_r4r4r4_* p0, struct S_r4r4r4_* p1, struct S_r4r4r4_* p2, const void* method);
    ((FuncToCall)methodPointer)(p0, p1, p2, method);



    CSValueToLuaValue(L, TIp0, p0, sizeof(S_r4r4r4_));//refsetback S_r4r4r4_
    CSValueToLuaValue(L, TIp1, p1, sizeof(S_r4r4r4_));//refsetback S_r4r4r4_
    CSValueToLuaValue(L, TIp2, p2, sizeof(S_r4r4r4_));//refsetback S_r4r4r4_

    return 3;
}

//Void set_StaticPropertyInt(Int32)
static int w_vi4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
    }

    //signature i4
    int32_t p0 = converter::Converter<int32_t>::toCpp(L, 0 + paramOffset);

    typedef void (*FuncToCall)(int32_t p0, const void* method);
    ((FuncToCall)methodPointer)(p0, method);




    return 0;
}

//Void StaticPrint(System.String)
static int w_vs(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isstring(L, 0 + paramOffset)) return false;
    }

    // string s
    void* p0 = LuaStr2CSharpString(L, 0);

    typedef void (*FuncToCall)(void* p0, const void* method);
    ((FuncToCall)methodPointer)(p0, method);




    return 0;
}

//Void MethodInvoke()
static int w_vt(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + -1) return false;

    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);


    typedef void (*FuncToCall)(void*, const void* method);
    ((FuncToCall)methodPointer)(self, method);




    return 0;
}

//Void DefaultValueFunc(Int32, System.String, System.String)
static int w_vtDi4DsDs(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp1 = wrapData->TypeInfos[0];
    auto TIp2 = wrapData->TypeInfos[1];
    auto length = lapi_lua_gettop(L);
    if (true) {
        if (length < paramOffset + -1) return false;
        if (length > 0 + paramOffset && !lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (length > 1 + paramOffset && !lapi_lua_isstring(L, 1 + paramOffset)) return false;
        if (length > 2 + paramOffset && !lapi_lua_isstring(L, 2 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //LuaValToCSVal primitive default
    int32_t p0 = xlua::OptionalParameter<int32_t>::GetPrimitive(L, 0 + paramOffset, method, wrapData, 0);
    //LuaValToCSVal string default
    void* p1 = xlua::OptionalParameter<void*>::GetString(L, 1 + paramOffset, method, wrapData, 1);
    //LuaValToCSVal string default
    void* p2 = xlua::OptionalParameter<void*>::GetString(L, 2 + paramOffset, method, wrapData, 2);

    typedef void (*FuncToCall)(void*, int32_t p0, void* p1, void* p2, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, p2, method);




    return 0;
}

//Void Scale(UnityEngine.Vector3)
static int w_vtS_r4r4r4_(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!CheckIsStruct(L, 0 + paramOffset, TIp0)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //LuaValToCSVal struct
    S_r4r4r4_* pp0 = GetCSharpStructPointerFromLua<S_r4r4r4_>(L, 0 + paramOffset, TIp0);
    S_r4r4r4_ p0 = pp0 ? *pp0 : S_r4r4r4_{};

    typedef void (*FuncToCall)(void*, struct S_r4r4r4_ p0, const void* method);
    ((FuncToCall)methodPointer)(self, p0, method);




    return 0;
}

//Void set_PropertyInt(Int32)
static int w_vti4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = converter::Converter<int32_t>::toCpp(L, 0 + paramOffset);

    typedef void (*FuncToCall)(void*, int32_t p0, const void* method);
    ((FuncToCall)methodPointer)(self, p0, method);




    return 0;
}

//Void DefaultParam(Int32, System.String)
static int w_vti4Ds(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp1 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (true) {
        if (length < paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (length > 1 + paramOffset && !lapi_lua_isstring(L, 1 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = converter::Converter<int32_t>::toCpp(L, 0 + paramOffset);
    //LuaValToCSVal string default
    void* p1 = xlua::OptionalParameter<void*>::GetString(L, 1 + paramOffset, method, wrapData, 1);

    typedef void (*FuncToCall)(void*, int32_t p0, void* p1, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, method);




    return 0;
}

//Void VariableParamsFunc(Int32, System.String[])
static int w_vti4Vs(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp1 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length < paramOffset + 0) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        //#TODO@benp check null
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = converter::Converter<int32_t>::toCpp(L, 0 + paramOffset);
    void* p1 = nullptr;//todo array

    typedef void (*FuncToCall)(void*, int32_t p0, void* p1, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, method);




    return 0;
}

//Void set_Item(Int32, Int32)
static int w_vti4i4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = converter::Converter<int32_t>::toCpp(L, 0 + paramOffset);
    //signature i4
    int32_t p1 = converter::Converter<int32_t>::toCpp(L, 1 + paramOffset);

    typedef void (*FuncToCall)(void*, int32_t p0, int32_t p1, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, method);




    return 0;
}

//Void set_Item(Int32, Single)
static int w_vti4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature i4
    int32_t p0 = converter::Converter<int32_t>::toCpp(L, 0 + paramOffset);
    //signature r4
    float p1 = converter::Converter<float>::toCpp(L, 1 + paramOffset);

    typedef void (*FuncToCall)(void*, int32_t p0, float p1, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, method);




    return 0;
}

//Void add_TestEvent(System.Action)
static int w_vto(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!CheckIsClass(L, 0 + paramOffset, TIp0)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // object
    void* p0 = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp0, L, 0 + paramOffset);

    typedef void (*FuncToCall)(void*, void* p0, const void* method);
    ((FuncToCall)methodPointer)(self, p0, method);




    return 0;
}

//Void .ctor(Single, Single)
static int w_vtr4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 1) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature r4
    float p0 = converter::Converter<float>::toCpp(L, 0 + paramOffset);
    //signature r4
    float p1 = converter::Converter<float>::toCpp(L, 1 + paramOffset);

    typedef void (*FuncToCall)(void*, float p0, float p1, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, method);




    return 0;
}

//Void Set(Single, Single, Single)
static int w_vtr4r4r4(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {

    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 2) return false;
        if (!lapi_lua_isnumber(L, 0 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 1 + paramOffset)) return false;
        if (!lapi_lua_isnumber(L, 2 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    //signature r4
    float p0 = converter::Converter<float>::toCpp(L, 0 + paramOffset);
    //signature r4
    float p1 = converter::Converter<float>::toCpp(L, 1 + paramOffset);
    //signature r4
    float p2 = converter::Converter<float>::toCpp(L, 2 + paramOffset);

    typedef void (*FuncToCall)(void*, float p0, float p1, float p2, const void* method);
    ((FuncToCall)methodPointer)(self, p0, p1, p2, method);




    return 0;
}

//Void Print(System.String)
static int w_vts(void* method, MethodPointer methodPointer, lua_State* L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {
    auto TIp0 = wrapData->TypeInfos[0];
    auto length = lapi_lua_gettop(L);
    if (checkLuaArgument) {
        if (length != paramOffset + 0) return false;
        if (!lapi_lua_isstring(L, 0 + paramOffset)) return false;
    }
    auto self = GetCppObjMapper()->ToCppObj(L, 1);
    // string s
    void* p0 = LuaStr2CSharpString(L, 0);

    typedef void (*FuncToCall)(void*, void* p0, const void* method);
    ((FuncToCall)methodPointer)(self, p0, method);




    return 0;
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
    {"ooo", (WrapFuncPtr)w_ooo},
    {"ot", (WrapFuncPtr)w_ot},
    {"r4S_r4r4r4_", (WrapFuncPtr)w_r4S_r4r4r4_},
    {"r4S_r4r4r4_S_r4r4r4_", (WrapFuncPtr)w_r4S_r4r4r4_S_r4r4r4_},
    {"r4S_r4r4r4_S_r4r4r4_S_r4r4r4_", (WrapFuncPtr)w_r4S_r4r4r4_S_r4r4r4_S_r4r4r4_},
    {"r4t", (WrapFuncPtr)w_r4t},
    {"r4ti4", (WrapFuncPtr)w_r4ti4},
    {"r8tS_i4s_Pi4UPsoUPo", (WrapFuncPtr)w_r8tS_i4s_Pi4UPsoUPo},
    {"soO", (WrapFuncPtr)w_soO},
    {"soOs", (WrapFuncPtr)w_soOs},
    {"st", (WrapFuncPtr)w_st},
    {"sto", (WrapFuncPtr)w_sto},
    {"sts", (WrapFuncPtr)w_sts},
    {"stso", (WrapFuncPtr)w_stso},
    {"u8ti8", (WrapFuncPtr)w_u8ti8},
    {"v", (WrapFuncPtr)w_v},
    {"vPS_r4r4r4_PS_r4r4r4_", (WrapFuncPtr)w_vPS_r4r4r4_PS_r4r4r4_},
    {"vPS_r4r4r4_PS_r4r4r4_PS_r4r4r4_", (WrapFuncPtr)w_vPS_r4r4r4_PS_r4r4r4_PS_r4r4r4_},
    {"vi4", (WrapFuncPtr)w_vi4},
    {"vs", (WrapFuncPtr)w_vs},
    {"vt", (WrapFuncPtr)w_vt},
    {"vtDi4DsDs", (WrapFuncPtr)w_vtDi4DsDs},
    {"vtS_r4r4r4_", (WrapFuncPtr)w_vtS_r4r4r4_},
    {"vti4", (WrapFuncPtr)w_vti4},
    {"vti4Ds", (WrapFuncPtr)w_vti4Ds},
    {"vti4Vs", (WrapFuncPtr)w_vti4Vs},
    {"vti4i4", (WrapFuncPtr)w_vti4i4},
    {"vti4r4", (WrapFuncPtr)w_vti4r4},
    {"vto", (WrapFuncPtr)w_vto},
    {"vtr4r4", (WrapFuncPtr)w_vtr4r4},
    {"vtr4r4r4", (WrapFuncPtr)w_vtr4r4r4},
    {"vts", (WrapFuncPtr)w_vts},
    {nullptr, nullptr}
};

static void b_v(void* target, MethodInfo* method) {



    PersistentObjectInfo* delegateInfo = GetDelegateInfo(target);
    auto L = delegateInfo->L;
    if (!L)
    {
        //todo thwrow exception
        return;
    }
    int oldTop = lapi_lua_gettop(L);
    int errFunc = lapi_pcall_prepare(L, errorFunc_ref, delegateInfo->reference);


    int n = lapi_lua_pcall(L, 0, 0, errFunc);
    if (!n) {

    }
    else {
        //todo throw exception 
    }
    lapi_lua_settop(L, errFunc - 1);
}

static void b_vs(void* target, void* p0, MethodInfo* method) {

    auto TIp0 = GetParameterType(method, 0);

    PersistentObjectInfo* delegateInfo = GetDelegateInfo(target);
    auto L = delegateInfo->L;
    if (!L)
    {
        //todo thwrow exception
        return;
    }
    int oldTop = lapi_lua_gettop(L);
    int errFunc = lapi_pcall_prepare(L, errorFunc_ref, delegateInfo->reference);

    CSAnyToLuaValue(L, p0);
    int n = lapi_lua_pcall(L, 1, 0, errFunc);
    if (!n) {

    }
    else {
        //todo throw exception 
    }
    lapi_lua_settop(L, errFunc - 1);
}
;
static BridgeFuncInfo g_bridgeFuncInfos[] = {
    {"v", (MethodPointer)b_v},
    {"vs", (MethodPointer)b_vs},
    {nullptr, nullptr}
};


static void ifg_S_i4i4_(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {



    struct S_i4i4_ ret;
xlua:GetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &ret);
    GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof(S_i4i4_));

}

static void ifs_S_i4i4_(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {



    //LuaValToCSVal struct
    S_i4i4_* pp = GetCSharpStructPointerFromLua<S_i4i4_>(L, 3, TIp);
    S_i4i4_ p = pp ? *pp : S_i4i4_{};
xlua:SetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &p);
}

static void ifg_i4(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {



    int32_t ret;

xlua:GetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &ret);

    lapi_lua_pushinteger(L, (long long)ret);
}

static void ifs_i4(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {



    //signature i4
    int32_t p = converter::Converter<int32_t>::toCpp(L, 3);
xlua:SetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &p);
}

static void ifg_r4(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {



    float ret;

xlua:GetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &ret);

    lapi_lua_pushnumber(L, (double)ret);
}

static void ifs_r4(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {



    //signature r4
    float p = converter::Converter<float>::toCpp(L, 3);
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
    S_i4i4_* pp = GetCSharpStructPointerFromLua<S_i4i4_>(L, 3, TIp);
    S_i4i4_ p = pp ? *pp : S_i4i4_{};
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
    int32_t p = converter::Converter<int32_t>::toCpp(L, 3);
xlua:SetFieldValue(self, (FieldInfo*)fieldInfo, offset, &p);
}

static void ifg_to(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {

    auto self = GetCppObjMapper()->ToCppObj_Field(L, 1);

    void* ret;

xlua:GetFieldValue(self, (FieldInfo*)fieldInfo, offset, &ret);

    GetCppObjMapper()->TryPushObject(L, ret);
}

static void ifs_to(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {

    auto self = GetCppObjMapper()->ToCppObj_Field(L, 1);

    // object
    void* p = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TIp, L, 3);
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
    float p = converter::Converter<float>::toCpp(L, 3);
xlua:SetFieldValue(self, (FieldInfo*)fieldInfo, offset, &p);
};

static FieldWrapFuncInfo g_fieldWrapFuncInfos[] = {
    {"S_i4i4_", (FieldWrapFuncPtr)ifg_S_i4i4_, (FieldWrapFuncPtr)ifs_S_i4i4_},
    {"i4", (FieldWrapFuncPtr)ifg_i4, (FieldWrapFuncPtr)ifs_i4},
    {"r4", (FieldWrapFuncPtr)ifg_r4, (FieldWrapFuncPtr)ifs_r4},
    {"tS_i4i4_", (FieldWrapFuncPtr)ifg_tS_i4i4_, (FieldWrapFuncPtr)ifs_tS_i4i4_},
    {"ti4", (FieldWrapFuncPtr)ifg_ti4, (FieldWrapFuncPtr)ifs_ti4},
    {"to", (FieldWrapFuncPtr)ifg_to, (FieldWrapFuncPtr)ifs_to},
    {"tr4", (FieldWrapFuncPtr)ifg_tr4, (FieldWrapFuncPtr)ifs_tr4},
    {nullptr, nullptr, nullptr}
};
