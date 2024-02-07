namespace XLua.IL2CPP.Editor.Generator{


    public static class CppWrapper{
        static string content = 
@"// Auto Gen

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
${valueTypeInfos.map(CODE_SNIPPETS.defineValueType).join('\n')}

${wrapperInfos.map(genFuncWrapper).join('\n')}

static WrapFuncInfo g_wrapFuncInfos[] = {
    %s
    {nullptr, nullptr}
};

${bridgeInfos.map(genBridge).join('\n')}

static BridgeFuncInfo g_bridgeFuncInfos[] = {
    %s
    {nullptr, nullptr}
};

${fieldWrapperInfos.map(genFieldWrapper).join('\n')}

static FieldWrapFuncInfo g_fieldWrapFuncInfos[] = {
    %s
    {nullptr, nullptr, nullptr}    
};";

        public static string GenFunctionBridge(FileExporter.CppWrappersInfo cppWrappersInfo){
                           
            return content;      
        }

    }

}