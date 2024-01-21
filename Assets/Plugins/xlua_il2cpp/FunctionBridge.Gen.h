
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

    static bool w_vt(void* method, MethodPointer methodPointer, lua_State *L, bool checkArgument, WrapData* wrapData) {
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
        

        typedef void* (*FuncToCall)(void*,const void* method);
        void * ret = ((FuncToCall)methodPointer)(nullptr, method);
        GetCppObjMapper()->TryPushObject(L, ret);
        
        
        return true;
    }


    ///模拟生成方法 method wrap
    // System.Nullable`1[System.DateTimeOffset] op_Explicit(Newtonsoft.Json.Linq.JToken)
    // Void DeleteSubKey(System.String)
    static bool w_vts(void* method, MethodPointer methodPointer, lua_State *L, bool checkArgument, WrapData* wrapData) {
        // PLog(LogLevel::Log, "Running w_vts");
        xlua::GLogCallback("Running w_vts");
        auto TIp0 = wrapData->TypeInfos[0];

        if (checkArgument) {

            // if(lua_gettop(L) != 2){
            //     return false;
            // }
            // if(lua_type(L, 2) != LUA_TSTRING){
            //     return false;
            // }
        }
        // lua api 编译失败 为啥
        // const char* str =  lua_tostring(L, 1);
        //#TODO@benp 拿到自己指针
        // auto self = xlua_get_csobj_pointer(L); //Cshare obj pointer
        // auto self = puerts::DataTransfer::GetPointerFast<void>(info.Holder());
        
        // JSValToCSVal s
        // v8::String::Utf8Value tp0(isolate, info[0]);
        // void* p0 = CStringToCSharpString(str);

        typedef void (*FuncToCall)(void*,void* p0, const void* method);
        //#TODO@benp 执行方法
        // ((FuncToCall)methodPointer)(self, p0,  method);  

        return true;
    }

    static WrapFuncInfo g_wrapFuncInfos[] = {
        {"vt", (WrapFuncPtr)w_vt},
        {"vts", (WrapFuncPtr)w_vts},
    };

    ///模拟生成方法 field wrap
    static void ifg_O(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {
        
    }

    static void ifs_O(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {
        
    }
    static FieldWrapFuncInfo g_fieldWrapFuncInfos[] = {
        {"O", (FieldWrapFuncPtr)ifg_O, (FieldWrapFuncPtr)ifs_O},
    };