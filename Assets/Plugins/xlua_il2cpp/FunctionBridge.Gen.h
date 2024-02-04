
    ///模拟生成方法 bridge
    // Nullable`1
    struct N_bS_i4i4i4i4u8__
    {
        bool hasValue;
    };
        
    static struct N_bS_i4i4i4i4u8__ b_N_bS_i4i4i4i4u8__o(void* target, void* p0, void* method) {
        return {};            
    }
    static BridgeFuncInfo g_bridgeFuncInfos[] = {{"N_bS_S_u8_i2__o", (MethodPointer)b_N_bS_i4i4i4i4u8__o}};

    static bool w_vt(void* method, MethodPointer methodPointer, lua_State *L, bool checkArgument, WrapData* wrapData, int index = 1) {
        // PLog(LogLevel::Log, "Running w_vts");
        xlua::GLogFormatted("Running w_vt");
        //#TODO@benp 构造函数再说

        // v8::Isolate* isolate = info.GetIsolate();
        // v8::Local<v8::Context> context = isolate->GetCurrentContext();

        // if (checkJSArgument) {
        //     auto length = info.Length();
        //     if (length != 0) return false;
        // }
        // auto self = puerts::DataTransfer::GetPointerFast<void>(info.Holder());
        
        auto self = lapi_xlua_getcsobj_ptr(L, index);
        typedef void* (*FuncToCall)(void*,const void* method);
        ((FuncToCall)methodPointer)(self, method);
        
        return true;
    }


    ///模拟生成方法 method wrap
    // System.Nullable`1[System.DateTimeOffset] op_Explicit(Newtonsoft.Json.Linq.JToken)
    // Void DeleteSubKey(System.String)
    static bool w_vts(void* method, MethodPointer methodPointer, lua_State *L, bool checkArgument, WrapData* wrapData, int index = 1) {
        // PLog(LogLevel::Log, "Running w_vts");
        xlua::GLogFormatted("Running w_vts");
        auto TIp0 = wrapData->TypeInfos[0];

        if (checkArgument) {

            if(lapi_lua_gettop(L) != 2){
                return false;
            }
            if(lapi_lua_isstring(L, 2) != LUA_TSTRING){
                return false;
            }
        }
        // lua api 编译失败 为啥
        const char* str =  lapi_lua_tolstring(L, 2);
        auto ilstring = il2cpp::vm::String::New(str);

        //#TODO@benp 拿到自己指针
        auto self = lapi_xlua_getcsobj_ptr(L, 1);
        if(self == nullptr){
            return false;
        }

        typedef void (*FuncToCall)(void*,void* p0, const void* method);
        
        ((FuncToCall)methodPointer)(self, ilstring,  method);  

        return true;
    }

    static WrapFuncInfo g_wrapFuncInfos[] = {
        {"vt", (WrapFuncPtr)w_vt},
        {"vts", (WrapFuncPtr)w_vts},
        {nullptr, nullptr},
    };

    ///模拟生成方法 field wrap
    static void ifg_ti4(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {
        auto self = lapi_xlua_getcsobj_ptr(L, 1);
        xlua::GLogFormatted("Running ifg_ti4");
        int32_t ret;
        xlua:GetFieldValue(self, (FieldInfo*)fieldInfo, offset, &ret);
        lapi_lua_pushinteger(L,ret);
    }

    static void ifs_O(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {
        auto self = lapi_xlua_getcsobj_ptr(L, 1);
        xlua::GLogFormatted("Running ifg_ti4");
        auto value = lapi_xlua_tointeger(L, 3);
        xlua:SetFieldValue(self, (FieldInfo*)fieldInfo, offset, &value);
    }
    static void ifg_i4(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {
        xlua::GLogFormatted("Running ifg_ti4");
        int32_t ret;
        xlua:GetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &ret);
        lapi_lua_pushinteger(L, ret);
    }

    static void ifs_i4(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {
        
        auto value = lapi_xlua_tointeger(L, 3);
        xlua:SetFieldValue(nullptr, (FieldInfo*)fieldInfo, offset, &value);
    }
    static FieldWrapFuncInfo g_fieldWrapFuncInfos[] = {
        {"i4", (FieldWrapFuncPtr)ifg_i4, (FieldWrapFuncPtr)ifs_i4},
        {"ti4", (FieldWrapFuncPtr)ifg_ti4, (FieldWrapFuncPtr)ifs_O},
        
        {nullptr, nullptr},
    };