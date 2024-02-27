using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Runtime.CompilerServices;
using UnityEngine;
namespace XLua.IL2CPP.Editor.Generator
{
    public class WrapUtil
    {
        static Dictionary<string, string> PrimitiveSignatureCppTypeMap = new System.Collections.Generic.Dictionary<string, string>{
            {"v", "void"},
            {"b", "bool"},
            {"u1", "uint8_t"},
            {"i1", "int8_t"},
            {"i2", "int16_t"},
            {"u2", "uint16_t"},
            {"i4", "int32_t"},
            {"u4", "uint32_t"},
            {"i8", "int64_t"},
            {"u8", "uint64_t"},
            {"c", "Il2CppChar"},
            {"r8", "double"},
            {"r4","float"}

            };
        public static string SToCPPType(string signature)
        {
            if (signature[0] == 'D')
            {
                signature = signature.Substring(1);
            }
            var t = PrimitiveSignatureCppTypeMap.ContainsKey(signature) ? PrimitiveSignatureCppTypeMap[signature] : "void*";
            if (signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                t = $"struct {signature}";
            }
            if (signature[0] == 'P')
            {
                t = $"{SToCPPType(signature.Substring(1))}";
            }
            return t;
        }
        private static string getSignatureWithoutRefAndPrefix(string signature)
        {
            if (signature[0] == 'P' || signature[0] == 'D')
            {
                return signature.Substring(1);
            }
            else
            {
                return signature;
            }
        }

        public static string GenFuncWrapper(SignatureInfo signatureInfo)
        {
            var selfCnt = (signatureInfo.ThisSignature == "t" || signatureInfo.ThisSignature == "T") ? 1 : 1;
            return $@"
//{signatureInfo.CsName}
static bool w_{signatureInfo.Signature}(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int index = 1) {{
    {WrapUtil.DeclareTypeInfo(signatureInfo)}
    if({(signatureInfo.ParameterSignatures.Any(s => s[0] == 'D') ? "true" : "checkLuaArgument")}){{
        auto length = lapi_lua_gettop(L);
        if ({WrapUtil.GenObjArgsLenCheck(signatureInfo.ThisSignature, signatureInfo.ParameterSignatures)}) return false;
        {String.Join("\n\t\t", signatureInfo.ParameterSignatures.Select<string, string>((s, index) => WrapUtil.CheckLuaArgument(s, index+selfCnt+1)))}
    }}
    {WrapUtil.GetThis(signatureInfo.ThisSignature)}
    {String.Join("\n\t", signatureInfo.ParameterSignatures.Select((s, index) => WrapUtil.LuaValToCSVal(s, "p" + index, GetParamIndex(signatureInfo.ThisSignature, index + 1))))}
    
    typedef {WrapUtil.SToCPPType(signatureInfo.ReturnSignature)} (*FuncToCall)({(WrapUtil.needThis(signatureInfo) ? "void*, " : "")}{String.Join("", signatureInfo.ParameterSignatures.Select((s, i) => $"{WrapUtil.SToCPPType(s)} p{i}, "))}const void* method);
    {(signatureInfo.ReturnSignature != "v" ? $"{WrapUtil.SToCPPType(signatureInfo.ReturnSignature)} ret = " : "")}((FuncToCall)methodPointer)({(WrapUtil.needThis(signatureInfo) ? "self, " : "")} {String.Join("", signatureInfo.ParameterSignatures.Select((s, i) => $"p{i}, "))}method);
    {String.Join("", signatureInfo.ParameterSignatures.Select((s, i) => WrapUtil.refSetback(s, i, signatureInfo)))}

    {(signatureInfo.ReturnSignature != "v" ? WrapUtil.ReturnToLua(signatureInfo.ReturnSignature) : "")}
    return true;
}}
";
        }

        private static bool IsPrimitiveSignatureCppType(string signature)
        {
            return PrimitiveSignatureCppTypeMap.ContainsKey(getSignatureWithoutRefAndPrefix(signature));
        }
        public static string DeclareTypeInfo(SignatureInfo signatureInfo)
        {
            var returnHasTypeInfo = signatureInfo.ReturnSignature != null && signatureInfo.ReturnSignature != "" && !IsPrimitiveSignatureCppType(signatureInfo.ReturnSignature);
            List<string> ret = new List<string>();
            int i = 0;
            if (returnHasTypeInfo)
            {
                ret.Add($"auto TIret = wrapData->TypeInfos[{i++}];");
            }
            for (int index = 0; index < signatureInfo.ParameterSignatures.Count; index++)
            {
                if (!IsPrimitiveSignatureCppType(signatureInfo.ParameterSignatures[index]))
                {
                    ret.Add($"auto TIp{index} = wrapData->TypeInfos[{i++}];");
                }
            }
            return String.Join("\n    ", ret);

        }

        public static List<string> GetValueTypeFieldSignatures(Type type)
        {
            List<string> ret = new List<string>();
            if (type.BaseType != null && type.BaseType.IsValueType) ret.Add(TypeUtils.GetTypeSignature(type.BaseType));
            foreach (var field in type.GetFields(BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic))
            {
                // if ((field.FieldType.IsValueType && !field.FieldType.IsPrimitive))
                // {
                //     ret.AddRange(GetValueTypeFieldSignatures(field.FieldType));
                // }
                // else
                // {
                ret.Add(TypeUtils.GetTypeSignature(field.FieldType));
                // }
            }
            return ret;
        }


        private static string GenObjArgsLenCheck(string thisSignature, List<string> parameterSignatures){
            var requireNum = 0;
            var selfCnt = (thisSignature == "t" || thisSignature == "T") ? 1 : 0;
            for (; requireNum < parameterSignatures.Count && parameterSignatures[requireNum][0] != 'V' && parameterSignatures[requireNum][0] != 'D'; ++requireNum) { }
            return requireNum != parameterSignatures.Count ? $"length < {requireNum + selfCnt}" : $"length != {parameterSignatures.Count + selfCnt}";
        }

        public static string GenArgsLenCheck(List<string> parameterSignatures)
        {
            var requireNum = 0;
            for (; requireNum < parameterSignatures.Count && parameterSignatures[requireNum][0] != 'V' && parameterSignatures[requireNum][0] != 'D'; ++requireNum) { }
            return requireNum != parameterSignatures.Count ? $"length < {requireNum}" : $"length != {parameterSignatures.Count}";
        }

        public static string CheckLuaArgument(string signature, int index)
        {
            var ret = "";
            string TypeInfoIndex = index.ToString();
            if (signature[0] == 'D') //optional
            {
                ret += $"if (length > {index} && ";
                signature = signature.Substring(1);
            }
            else if (signature[0] == 'V') //array
            {
                TypeInfoIndex = index + "_V";
                ret += $"auto TIp{index}_V = GetArrayElementTypeId(TIp{index}); if (!info[{index}]->IsNullOrUndefined() && ";
                signature = signature.Substring(1);
            }
            else
            {
                ret += "if (";
            }

            if (PrimitiveSignatureCppTypeMap.ContainsKey(signature))
            {
                if (signature == "b")
                {
                    ret += $"!lapi_lua_isboolean(L, {index})) return false;";
                }
                else if (signature == "u1" || signature == "i1" || signature == "i2" || signature == "u2" || signature == "u4" || signature == "i4"
                 || signature == "u8" || signature == "i8"
                )
                {
                    ret += $"!lapi_lua_isinteger(L, {index})) return false;";
                }
                else if (signature == "c")
                {
                    ret += $"!lapi_lua_isinteger(L, {index})) return false;";
                }
                else if (signature == "r8" || signature == "r4")
                {
                    ret += $"!lapi_lua_isnumber(L, {index})) return false;";
                }else{
                    ret = $"// invalid check lua args signature {signature}";
                }
            }
            else if (signature == "p" || signature == "Pv")
            { // IntPtr, void*
                ret += "true){/*#TODO@benp 类型检查*/};";
            }
            else if (signature[0] == 'P')
            {
                ret += "true){/*#TODO@benp 类型检查*/};";
            }
            else if (signature == "a")
            {
                ret += "true){/*#TODO@benp 类型检查*/};";
            }
            else if (signature == "s")
            {
                ret += $"!lapi_lua_isstring(L, {index})) return false;";
            }
            else if (signature == "o")
            {
                ret += "true){/*#TODO@benp 类型检查*/};";
            }
            else if (signature == "O")
            {
                return "";
            }
            else if ((signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            {
                ret += "true){/*#TODO@benp 类型检查*/};";
                // ret += $"(!info[{index}]->IsObject() || !IsAssignableFrom(TIp{TypeInfoIndex}, GetTypeId(info[{index}].As<v8::Object>())))) return false;";
            }
            else
            {
                ret += "!!true) return false;";
            }
            return ret;
        }

        public static int GetParamIndex(string signature, int index)
        {
            if (signature == "t" || signature == "T")
            {
                return index + 1;
            }
            else
            {
                return index;
            }
        }

        public static string GetThis(string signature)
        {
            if (signature == "t")
            {
                return "auto self = lapi_xlua_getcsobj_ptr(L, 1);";
            }
            else if (signature == "T")
            {
                return "auto self = lapi_xlua_getcsobj_ptr(L, 1);";
            }
            else
            {
                return "";
            }
        }

        public static string LuaValToCSVal(string signature, string CSName, int index)
        {
            if (signature == TypeUtils.TypeSignatures.String)
            {
                return
$@"// string s
    const char* str{index} =  lapi_lua_tolstring(L, {index});
    auto {CSName} = il2cpp::vm::String::New(str{index});
";
            }
    //         else if (PrimitiveSignatureCppTypeMap.ContainsKey(signature)){
    //             //#TODO@benp 
    //             return $@"//Primitive{signature}
    // ";
    //         }
            else if (signature == "Ps")
            { //string ref
                return
$@"// string ref Ps
    void* u{CSName} = nullptr; // string ref
    void** {CSName} = &u{CSName}
    const char* str{index} =  lapi_lua_tolstring(L, {index});
    auto u{CSName} = il2cpp::vm::String::New(str{index});
";
            }
            else if (signature == "O" || signature == "o" || signature == "a")
            { //object
                return $@"// object
    void* {CSName} = LuaValueToCSRef(L, {index}, TI{CSName});";
            }
            else if (signature == "Po" || signature == "PO" || signature == "Pa")
            {
                return $@"// object ref Po/PO
    void* u${CSName} = nullptr; // object ref
    void** ${CSName} = &u${CSName};
    u${CSName} = LuaValueToCSRef(L, {index}, TI${CSName});
        ";
            }
            else if ((signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            {
                //#TODO@benp struct
            }
            else if ((signature.StartsWith('P' + TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith('P' + TypeUtils.TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            { //valuetype ref

            }
            else if (signature[0] == 'P' && signature != "Pv")
            {

            }
            else if (signature[0] == 'V') // array 
            {

            }
            else if (signature[0] == 'D') // optional 
            {

            }
            else 
            {
                // any

                return $@"//signature {signature}
    {SToCPPType(signature)} {CSName} = {GetArgValue(signature, index)}";
            }
            return $"//unknown type {signature}";
        }

        public static string GetArgValue(string signature, int index, bool isRef = false){
            if(PrimitiveSignatureCppTypeMap.ContainsKey(signature)){
                //#TODO@benp ref
                if(signature == "u1" || signature == "i1" ||
                signature == "u2" || signature == "i2" ||
                signature == "u4" || signature == "i4" ||
                signature == "u8" || signature == "i8"){
                    return $"lapi_xlua_tointeger(L, {index});";
                }else if(signature == "c"){
                    return $"(Il2CppChar)lapi_xlua_tointeger(L, {index});";
                }else if(signature == "r8" || signature == "r4"){
                    return $"lapi_lua_tonumber(L, {index});";
                }else if(signature == "b"){
                    return $"(bool)lapi_lua_toboolean(L, {index});";
                }else if(signature == "v"){
                    Debug.LogError("invalid signature v");
                    return "nullptr";
                }
            }else if((signature == "Pv" || signature == "p") && !isRef){
                return "nullptr /*todo pointer*/";
            }else{
                //default value
                return "nullptr /*todo default value*/";
            }

            return "nullptr";
        }

        public static bool needThis(SignatureInfo wrapperInfo)
        {
            return wrapperInfo.ThisSignature == "t" || wrapperInfo.ThisSignature == "T";
        }
        public static string CSValToLuaVal(string signature, string CSName)
        {
            if (PrimitiveSignatureCppTypeMap.ContainsKey(signature))
            {
                if (signature == "b")
                { // bool
                    return $"lapi_lua_pushboolean(L, {CSName})";
                }
                else if (signature == "u1" || signature == "i1"
                || signature == "u2" || signature == "i2"
                || signature == "u4" || signature == "i4"
                || signature == "u8" || signature == "i8"
                || signature == "c"
                )
                {
                    return $"lapi_lua_pushinteger(L, (long long){CSName})";
                }
                else if (signature == "r8" || signature == "r4")
                {
                    return $"lapi_lua_pushnumber(L, (double){CSName})";
                }

            }
            else if (signature == "s" || signature == "O")
            {
                return $@"  const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars((Il2CppString*){CSName});
    std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
    lapi_lua_pushstring(L, str.c_str());";
            }
            else if (signature == "o" || signature == "a")
            {
                return $"GetCppObjMapper()->TryPushObject(L, {CSName})";
            }
            else if (signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                //#TODO@benp struct
                // return `CopyNullableValueType(isolate, context, TI${CSName[0] == '*' ? CSName.substring(1) : CSName}, ${CSName[0] == '*' ? CSName.substring(1) : `&${CSName}`}, (${CSName[0] == '*' ? CSName.substring(1) : `&${CSName}`})->hasValue, sizeof(${CSName}))`
            }
            else if (signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) && signature.EndsWith("_"))
            {
                //#TODO@benp struct
                // return `CopyValueType(isolate, context, TI${CSName[0] == '*' ? CSName.substring(1) : CSName}, ${CSName[0] == '*' ? CSName.substring(1) : `&${CSName}`}, sizeof(${CSName}))`
            }
            return "";
        }
        /// <summary>
        /// ref类型回填
        /// </summary>
        /// <param name=""></param>
        /// <param name=""></param>
        /// <returns></returns> 
        public static string refSetback(string signature, int index, SignatureInfo signatureInfo)
        {
            //#TODO@benp ref类型回填
            if (signature[0] == 'P' && signature != "Pv")
            {
                var elementSignature = signature.Substring(1);
                var val = CSValToLuaVal(elementSignature, $"*p{index}");

                if (val != "")
                {

                }
            }
            return "";
        }

        public static string ReturnToLua(string signature)
        {

            if (PrimitiveSignatureCppTypeMap.ContainsKey(signature))
            {   
                if (signature == "b")
                { // bool
                    return $"lapi_lua_pushboolean(L, ret)";
                }
                else if (signature == "u1" || signature == "i1"
                || signature == "u2" || signature == "i2"
                || signature == "u4" || signature == "i4"
                || signature == "u8" || signature == "i8"
                || signature == "c"
                )
                {
                    return $"lapi_lua_pushinteger(L, (long long)ret);";
                }
                else if (signature == "r8" || signature == "r4")
                {
                    return $"lapi_lua_pushnumber(L, (double)ret);";
                }
                // return "xlua::TryTranslatePrimitiveWithClass(L, ret);";
            }
            else if (signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                // return "info.GetReturnValue().Set(CopyNullableValueType(isolate, context, TIret, &ret, ret.hasValue, sizeof(ret)));";
            }
            else if (signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) && signature.EndsWith("_"))
            {
                // return "info.GetReturnValue().Set(CopyValueType(isolate, context, TIret, &ret, sizeof(ret)));";
            }
            else if (signature == "o")
            { // classes except System.Object
                return "GetCppObjMapper()->TryPushObject(L, ret);";
            }
            else if (signature == "a")
            { // ArrayBuffer
                return "GetCppObjMapper()->TryPushObject(L, ret);";
            }
            else if (signature == "O")
            { // System.Object
                return "GetCppObjMapper()->TryPushObject(L, ret);";
            }
            else if (signature == "s")
            { // string
                return $@"  const Il2CppChar* utf16Ret = il2cpp::utils::StringUtils::GetChars((Il2CppString*)ret);
    std::string retStr = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16Ret);
    lapi_lua_pushstring(L, retStr.c_str());";
            }
            else if (signature == "p" || signature == "Pv")
            { // IntPtr, void*
              //#TODO@benp
              // return "info.GetReturnValue().Set(v8::ArrayBuffer::New(isolate, v8::ArrayBuffer::NewBackingStore(ret, 0, &v8::BackingStore::EmptyDeleter, nullptr)));";
            }
            else
            { //TODO: 能处理的就处理, DateTime是否要处理呢？
                return "// unknow ret signature: ${signature}";
            }
            return "";
        }


        public static string genBridge(SignatureInfo bridgeInfo)
        {
            //#TODO@benp 
            return "";

        }
        public static string genGetField(SignatureInfo fieldWrapperInfo)
        {
            var signature = fieldWrapperInfo.ReturnSignature;
            if ((signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            { //valuetype
              //#TODO@benp struct
                return "//todo struct";
            }
            else
            {
                return $@"{SToCPPType(fieldWrapperInfo.ReturnSignature)} ret;

    xlua:GetFieldValue({(needThis(fieldWrapperInfo) ? "self, " : "nullptr, ")}(FieldInfo*)fieldInfo, offset, &ret);
    
    {ReturnToLua(fieldWrapperInfo.ReturnSignature)}";
            }
        }
        static List<string> list = new List<string>() { "o", "s", "p", "a" };
        public static string genFieldWrapper(SignatureInfo fieldWrapperInfo)
        {
            return $@"
static void ifg_{fieldWrapperInfo.Signature}(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {{
    
    {(needThis(fieldWrapperInfo) ? "auto self = lapi_xlua_getcsobj_ptr(L, 1);" : "")}

    {genGetField(fieldWrapperInfo)}
}}

static void ifs_{fieldWrapperInfo.Signature}(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {{
    
    {(needThis(fieldWrapperInfo) ? GetThis(fieldWrapperInfo.ThisSignature) : "")}

    {LuaValToCSVal(fieldWrapperInfo.ReturnSignature, "p", 3)}
    xlua:SetFieldValue({(needThis(fieldWrapperInfo) ? "self, " : "nullptr, ")}(FieldInfo*)fieldInfo, offset, {(list.IndexOf(fieldWrapperInfo.Signature) != -1 ? "p" : "&p")});
}}";
        }
    }

}