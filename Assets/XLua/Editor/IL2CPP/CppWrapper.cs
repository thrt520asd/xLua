using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XLua.IL2CPP.Editor.Generator{


    public static class CppWrapper{


        public static string Gen(CppWrappersInfo cppWrappersInfo){
            
            
            return $@"// Auto Gen
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
{String.Join("\n", cppWrappersInfo.ValueTypeInfos.Select(s=>s.ToString()))}

{String.Join("\n", cppWrappersInfo.WrapperInfos.Select(s=>WrapUtil.GenFuncWrapper(s)))}

static WrapFuncInfo g_wrapFuncInfos[] = {{
    {String.Join("\n\t", cppWrappersInfo.WrapperInfos.Select(s => $@"{{""{s.Signature}"", (WrapFuncPtr)w_{s.Signature}}},"))}
    {{nullptr, nullptr}}
}};

{String.Join("\n", cppWrappersInfo.BridgeInfos.Select(s=>WrapUtil.genBridge(s)))};
static BridgeFuncInfo g_bridgeFuncInfos[] = {{
    {String.Join("\n\t", cppWrappersInfo.BridgeInfos.Select(s=>"{"+ $@"""{s.Signature}"", (MethodPointer)b_{s.Signature}" + "},"))}
    {{nullptr, nullptr}}
}};

{String.Join("\n", cppWrappersInfo.FieldWrapperInfos.Select(s=>WrapUtil.genFieldWrapper(s)))};

static FieldWrapFuncInfo g_fieldWrapFuncInfos[] = {{
    {String.Join("\n\t", cppWrappersInfo.FieldWrapperInfos.Select(s=>$@"{{""{s.Signature}"", (FieldWrapFuncPtr)ifg_{s.Signature}, (FieldWrapFuncPtr)ifs_{s.Signature}}},"))}
    {{nullptr, nullptr, nullptr}}
}};
            ";
        }
    }

}