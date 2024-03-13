using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Runtime.CompilerServices;
using UnityEngine;
using UnityScript.Steps;
using UnityEngine.Timeline;
namespace XLua.IL2CPP.Editor.Generator
{
    
    public static class WrapUtil
    {
        public static bool IsStruct(this string str){
            return (str.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || str.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix)) && str.EndsWith("_");
        }
        public static string CSType(this string str){
            if(str[0] == 'V'){
                return SToCPPType(str.Substring(1));
            }
            return SToCPPType(str.Substring(1));
        }
        public static bool IsOutParameter(this string str){
            return str[0] == TypeUtils.TypeSignatures.OutParameterPrefix[0];
        }
        public static bool IsRefParameter(this string str){
            return str[0] == TypeUtils.TypeSignatures.OutParameterPrefix[0]
            || str[0] == TypeUtils.TypeSignatures.RefOrPointerPrefix[0];
        }

        public static bool IsOptionalParameter(this string str){
            return str[0] == TypeUtils.TypeSignatures.OptionalParmaeterPrefix[0];
        }

        public static bool NotNullOrEmpty(this string str){
            return str != null &&  str != "";
        }
        public static string TrimStartXing(this string str){
            return str[0] == '*' ? str.Substring(1) : str;
        }

        public static string CSNamePointer(this string str){
            return str[0] == '*' ? str : "&"+str;
        }

        public static bool IsPrimitiveSignatureRaw(this string str){
            return PrimitiveSignatureCppTypeMap.ContainsKey((str));
        }
        public static bool IsPrimitiveSignature(this string str){
            return PrimitiveSignatureCppTypeMap.ContainsKey(getSignatureWithoutRefAndPrefix(str));
        }
        public static bool IsStatic(string thisSignature){
            return !(thisSignature == "t" || thisSignature == "T" || thisSignature == "s");
        }
        public static bool needThis(string thisSignature)
        {
            return thisSignature == "t" || thisSignature == "T" || thisSignature == "s";
        }

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
        public static string SToCPPTypeNoDeclare(string signature){
            if (signature.IsOptionalParameter() || signature.IsOutParameter())
            {
                signature = signature.Substring(1);
            }

            var t = signature.IsPrimitiveSignatureRaw() ? PrimitiveSignatureCppTypeMap[signature] : "void*";
            if (signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                t = $"{signature}";
            }
            if (signature[0] == 'P')
            {
                t = $"{SToCPPType(signature.Substring(1))}*";
            }
            return t;
        }

        public static string SToCPPType(string signature)
        {
            if (signature.IsOptionalParameter() || signature.IsOutParameter())
            {
                signature = signature.Substring(1);
            }
            var t = signature.IsPrimitiveSignatureRaw() ? PrimitiveSignatureCppTypeMap[signature] : "void*";
            if (signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                t = $"struct {signature}";
            }
            if (signature[0] == 'P')
            {
                t = $"{SToCPPType(signature.Substring(1))}*";
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
        /// <summary>
        /// 过滤out的参数
        /// </summary>
        /// <param name="list"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public static int RealParameterIndex(List<string> list, int index){
            int outParameterCnt = 0;
            for(int i = 0; i < Math.Min(list.Count - 1, index); i++){
                if(list[i].IsOutParameter()){
                    outParameterCnt++;
                }
            }
            return Math.Max(index - outParameterCnt, 0);
        }
        /// <summary>
        /// 函数的返回值数量（in,out,ref通过返回值返回到lua）
        /// </summary>
        /// <param name="signatureInfo"></param>
        /// <returns></returns>
        public static int GetFuncRetCnt(SignatureInfo signatureInfo){
            int refCnt = 0;
            int returnCnt = 0;
            if(signatureInfo.ReturnSignature.NotNullOrEmpty() && signatureInfo.ReturnSignature != "v"){
                returnCnt = 1;
            }
            foreach (var item in signatureInfo.ParameterSignatures)
            {
                if(item.IsRefParameter()) refCnt++;
            }
            return returnCnt + refCnt;
            
        }
        /// <summary>
        /// 函数的lua栈 1 closure 2 param1 3 param2
        /// </summary>
        /// <param name="signatureInfo"></param>
        /// <returns></returns>
        public static string GenFuncWrapper(SignatureInfo signatureInfo)
        {
            
            return 
$@"//{signatureInfo.CsName}
static int w_{signatureInfo.Signature}(void* method, MethodPointer methodPointer, lua_State *L, bool checkLuaArgument, WrapData* wrapData, int paramOffset) {{
    {WrapUtil.DeclareTypeInfo(signatureInfo)}
    auto length = lapi_lua_gettop(L);
    if({(signatureInfo.ParameterSignatures.Any(s => s[0] == 'D') ? "true" : "checkLuaArgument")}){{
        if ({WrapUtil.GenArgsLenCheck(signatureInfo.ParameterSignatures)}) return -1;
        {String.Join("\n\t\t", signatureInfo.ParameterSignatures.Select<string, string>((s, index) => WrapUtil.CheckLuaArgument(s, RealParameterIndex(signatureInfo.ParameterSignatures, index))))}
    }}
    {WrapUtil.GetThis(signatureInfo.ThisSignature)}
    {String.Join("\n\t", signatureInfo.ParameterSignatures.Select((s, index) => WrapUtil.LuaValToCSVal(s, "p" + index,  RealParameterIndex(signatureInfo.ParameterSignatures, index) , true)))}
    
    typedef {WrapUtil.SToCPPType(signatureInfo.ReturnSignature)} (*FuncToCall)({(WrapUtil.needThis(signatureInfo.ThisSignature) ? "void*, " : "")}{String.Join("", signatureInfo.ParameterSignatures.Select((s, i) => $"{WrapUtil.SToCPPType(s)} p{i}, "))}const void* method);
    {(signatureInfo.ReturnSignature != "v" ? $"{WrapUtil.SToCPPType(signatureInfo.ReturnSignature)} ret = " : "")}((FuncToCall)methodPointer)({(WrapUtil.needThis(signatureInfo.ThisSignature) ? "self, " : "")} {String.Join("", signatureInfo.ParameterSignatures.Select((s, i) => $"p{i}, "))}method);

    {(signatureInfo.ReturnSignature != "v" ? WrapUtil.ReturnToLua(signatureInfo.ReturnSignature) : "")}

    {String.Join("", signatureInfo.ParameterSignatures.Select((s, i) => WrapUtil.refSetback(s, i, signatureInfo)))}
    return {GetFuncRetCnt(signatureInfo)};
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


        private static string GenArgsLenCheck(List<string> parameterSignatures){
            var requireNum = 0;
            
            for (; requireNum < parameterSignatures.Count && parameterSignatures[requireNum][0] != 'V' && parameterSignatures[requireNum][0] != 'D'; ++requireNum) { }
            return requireNum != parameterSignatures.Count ? $"length < paramOffset + {requireNum - 1} " : $"length != paramOffset + {parameterSignatures.Count - 1}";
        }


        public static string CheckLuaArgument(string signature, int index)
        {
            var ret = "";
            string TypeInfoIndex = index.ToString();
            if (signature[0] == 'D') //optional
            {
                ret += $"if (length > {index} + paramOffset && ";
                signature = signature.Substring(1);
            }
            else if (signature[0] == 'V') //array
            {
                TypeInfoIndex = index + "_V";
                
                ret += $@"";
                signature = signature.Substring(1);
                return "//#TODO@benp check null";
            }else if(signature.IsOutParameter()){
                return "";
            }
            else
            {
                ret += "if (";
            }

            if (PrimitiveSignatureCppTypeMap.ContainsKey(signature))
            {
                if (signature == "b")
                {
                    ret += $"!lapi_lua_isboolean(L, {index} + paramOffset)) return -1;";
                }
                else if (signature == "u1" || signature == "i1" || signature == "i2" || signature == "u2" || signature == "u4" || signature == "i4"
                 || signature == "u8" || signature == "i8"
                )
                {
                    ret += $"!lapi_lua_isnumber(L, {index} + paramOffset)) return -1;";
                }
                else if (signature == "c")
                {
                    ret += $"!lapi_lua_isnumber(L, {index} + paramOffset)) return -1;";
                }
                else if (signature == "r8" || signature == "r4")
                {
                    ret += $"!lapi_lua_isnumber(L, {index} + paramOffset)) return -1;";
                }else{
                    ret = $"// invalid check lua args signature {signature}";
                }
            }
            else if (signature == "p" || signature == "Pv")
            { 
                // IntPtr, void*
                ret +=$"!lapi_lua_isuserdata(L, {index} + paramOffset)) return -1;";
            }
            else if (signature[0] == 'P')
            {
                ret +=$"!lapi_lua_isuserdata(L, {index} + paramOffset)) return -1;";
            }
            else if (signature == "a")
            {
                ret += "true){/*#TODO@benp 类型检查*/};";
            }
            else if (signature == "s")
            {
                ret += $"!lapi_lua_isstring(L, {index} + paramOffset)) return -1;";
            }
            else if (signature == "o")
            {
                ret += $"!CheckIsClass(L, {index} + paramOffset, TIp{index})) return -1;";
            }
            else if (signature == "O")
            {
                return "";
            }
            else if ((signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            {
                ret +=$"!CheckIsStruct(L, {index} + paramOffset, TIp{index})) return -1;";
            }
            else
            {
                ret += "!!true) return -1;";
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
        /// struct 什么时候需要offset 需要调用struct的成员方法时 该struct需要offset 其他时刻均不需要
        public static string GetThis(string signature, bool field = false)
        {
            if (signature == "t")
            {
                return $"auto self = GetCppObjMapper()->ToCppObj{(field?"_Field":"")}(L, 1);";
            }
            else if (signature == "T")
            {
                return $"auto self = GetCppObjMapper()->ToCppObj{(field?"_Field":"")}(L, 1);";
            }
            else if (signature == "s" ){
                return $"auto self = GetCppObjMapper()->ToCppObj{(field?"_Field":"")}(L, 1);";
            }
            
            return "";
        }

                  
        public static string LuaValToCSVal(string signature, string CSName, int index, bool needOffset = false, bool isField = false)
        {
            if(signature.IsOutParameter()){
                Debug.Log("signature "+signature + index);
                return 
$@"void* u{CSName} = nullptr; //out ref
    void** {CSName} = &u{CSName};
    ";
            }
            if (signature == TypeUtils.TypeSignatures.String)
            {
                return
$@"// string s
    void* {CSName} = LuaStr2CSharpString(L, {index});";
            }
            //string ref
            else if (signature == "Ps")
            { 
                return
$@"// string ref Ps
    void* u{CSName} = nullptr; // string ref
    void** {CSName} = &u{CSName};
    u{CSName} = LuaStr2CSharpString(L, {index}{ (needOffset?"+ paramOffset":"")});
";
            }
            //object
            else if (signature == "O" || signature == "o" || signature == "a")
            { 
                return 
$@"// object
    void* {CSName} = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TI{CSName}, L, {index}{ (needOffset?"+ paramOffset":"")});";
            }
            //object ref
            else if (signature == "Po" || signature == "PO" || signature == "Pa")
            {
                return 
$@"// object ref Po/PO
    void* u{CSName} = nullptr; // object ref
    void** {CSName} = &u{CSName};
    u{CSName} = (void*)LuaValueToCSRef((Il2CppClass*)TI{CSName},L, {index}{ (needOffset?"+ paramOffset":"")});";
            }
            //struct
            else if ((signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            {
                    return 
$@"//LuaValToCSVal struct
    {signature}* p{CSName} = GetCSharpStructPointerFromLua<{signature}>(L, {index}{ (needOffset?"+ paramOffset":"")}, TIp{(isField?"":index.ToString())});
    {signature} {CSName} = p{CSName} ? *p{CSName} : {signature} {{}};";
            }
            //valuetype ref
            else if ((signature.StartsWith('P' + TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith('P' + TypeUtils.TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            { 
                var realS = signature.Substring(1);
                return 
$@"//LuaValToCSVal Pstruct
    {realS}* {CSName} = nullptr;
    {CSName} = ({realS}*)GetCSharpStructPointerFromLua<{realS}>(L, {index}{ (needOffset?"+ paramOffset":"")}, TIp{(isField? "": index.ToString())});
    if(!{CSName}){{
        {realS} u{CSName};
        memset(&u{CSName}, 0, sizeof({realS}));
        {CSName} = &u{CSName};
    }}";
            }
            else if (signature[0] == 'P' && signature != "Pv")
            {
                
                return 
$@"{signature.CSType()}* {CSName} = nullptr; // {signature}
    {signature.CSType()} u{CSName} = {GetArgValue(signature.Substring(1), index, true, true)}
    {CSName} = &u{CSName};";
            }
            else if (signature[0] == 'V') // array 
            {
                return $"{signature.CSType()} {CSName} = nullptr;//todo array";
            }
            else if (signature[0] == 'D') // optional 
            {
                var str = signature.Substring(1);
                if(PrimitiveSignatureCppTypeMap.ContainsKey(str)){
                    return 
$@"//LuaValToCSVal primitive default
    {PrimitiveSignatureCppTypeMap[str]} {CSName} = xlua::OptionalParameter<{PrimitiveSignatureCppTypeMap[str]}>::GetPrimitive(L,{index}{ (needOffset?"+ paramOffset":"")}, method, wrapData, {index});";
                }
                else if(str == "s"){
                    return 
$@"//LuaValToCSVal string default
    void* {CSName} = xlua::OptionalParameter<void*>::GetString(L,{index}{ (needOffset?"+ paramOffset":"")}, method, wrapData, {index});";
                }
                else if(str == "o" || str == "O" || str == "a"){
                    return 
$@"//LuaValToCSVal ref with default
    void* {CSName} = xlua::OptionalParameter<void*>::GetRefType(L,{index}{ (needOffset?"+ paramOffset":"")}, method, wrapData, {index});";
                }
                else if(str.IsStruct()){
                    return 
$@"//LuaValToCSVal valuetype  with default
    {str} {CSName} = xlua::OptionalParameter<{str}>::GetValueType(L,{index}{ (needOffset?"+ paramOffset":"")}, method, wrapData, {index});";
                }
                else{
                    return 
$@"//LuaValToCSVal unknow type with default
    void* {CSName} = nullptr;";
                }
            }
            else 
            {
                // any

                return $@"//signature {signature}
    {SToCPPType(signature)} {CSName} = {GetArgValue(signature, index, false, needOffset)}";
            }
        }

        public static string GetArgValue(string signature, int index, bool isRef = false, bool needOffset = false){
            if(PrimitiveSignatureCppTypeMap.ContainsKey(signature)){
                return $@"converter::Converter<{PrimitiveSignatureCppTypeMap[signature]}>::toCpp(L, {index}{ (needOffset?"+ paramOffset":"")});";
            }else if((signature == "Pv" || signature == "p") && !isRef){
                return $"lapi_lua_touserdata(L, {index}{ (needOffset?"+ paramOffset":"")});";
            }else{
                //default value
                if(signature.IsPrimitiveSignature()){
                    if(signature == "v"){
                        throw new Exception("void has no default");
                    }
                    return signature == "b" ? "false" :"0";
                }

                if(signature.IsStruct()){
                    return "{}";
                }
            }

            return "nullptr";
        }
        
        public static string CSValToLuaVal(string signature, string CSName)
        {
            if (PrimitiveSignatureCppTypeMap.ContainsKey(signature))
            {
                return $"converter::Converter<{PrimitiveSignatureCppTypeMap[signature]}>::toScript(L, {CSName})";
            }
            else if (signature == "s" || signature == "O")
            {
                return $@"CSAnyToLuaValue(L, {CSName})";
            }
            else if (signature == "o" || signature == "a")
            {
                return $@"CSAnyToLuaValue(L, {CSName})";
            }
            else if (signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                
                return $"CopyNullableValueType(L, TI{CSName.TrimStartXing()}, {CSName}, {CSName}->hasValue, sizeof({signature}))";
            }
            else if (signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) && signature.EndsWith("_"))
            {
                return $"CSValueToLuaValue(L, TI{CSName.TrimStartXing()}, {CSName.CSNamePointer()}, sizeof({signature}))";
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
            if(signature.IsOutParameter()){
                signature = signature.Substring(1);
            }
            if (signature[0] == 'P' && signature != "Pv")
            {
                var elementSignature = signature.Substring(1);
                string CSName = "p"+index;
                string returnStr = "";
                if(elementSignature.IsPrimitiveSignature()){
                    returnStr = $"converter::Converter<{PrimitiveSignatureCppTypeMap[elementSignature]}>::toScript(L, *{CSName})";
                }
                else if (elementSignature == "s" || elementSignature == "O")
                {
                    returnStr = $@"CSAnyToLuaValue(L, *{CSName}, TIp{index})";
                }
                else if (elementSignature == "o" || elementSignature == "a")
                {
                    returnStr = $@"CSAnyToLuaValue(L, *{CSName}, TIp{index})";
                }
                else if (elementSignature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix) && elementSignature.EndsWith("_"))
                {
                    
                    returnStr = $"CopyNullableValueType(L, TI{CSName.TrimStartXing()}, {CSName}, {CSName}->hasValue, sizeof({elementSignature}))";
                }
                else if (elementSignature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) && elementSignature.EndsWith("_"))
                {
                    returnStr = $"CSValueToLuaValue(L, TI{CSName.TrimStartXing()}, {CSName}, sizeof({elementSignature}))";
                }
                // var val = CSValToLuaVal(elementSignature, $"*p{index}");
                if (returnStr != "")
                {
                    returnStr = returnStr + $";//refsetback {elementSignature}\n\t";
                    return returnStr;
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
                    return $"lapi_lua_pushboolean(L, ret);";
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
                return $"GetCppObjMapper()->TryPushStruct(L, TIret, &ret, sizeof({signature}));";
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
              return $@"lapi_lua_pushlightuserdata(L, ret);";
            }
            else
            { //TODO: 能处理的就处理, DateTime是否要处理呢？
                return "// unknow ret signature: ${signature}";
            }
            return "";
        }

        public static string ReturnToCS(string signature){
            if(signature == TypeUtils.TypeSignatures.Void){
                return "";
            }
            return 
$@"{LuaValToCSVal(signature, "ret", -1)}
    return ret;";
        }
        public static string genBridge(SignatureInfo bridgeInfo)
        {
            
            var hasVarArgs = bridgeInfo.ParameterSignatures.Count > 0 && bridgeInfo.ParameterSignatures[bridgeInfo.ParameterSignatures.Count - 1][0] == 'V';
            return 
$@"static {SToCPPType(bridgeInfo.ReturnSignature)} b_{bridgeInfo.Signature}(void* target, {
    string.Join("",bridgeInfo.ParameterSignatures.Select((s,i)=>$"{SToCPPType(s)} p{i},"))
}MethodInfo* method) {{
    {(bridgeInfo.ReturnSignature.NotNullOrEmpty() && !bridgeInfo.ReturnSignature.IsPrimitiveSignature() ? "auto TIret = GetReturnType(method);":"")}
    {string.Join("\n", bridgeInfo.ParameterSignatures.Select((s,i)=>$"{(!s.IsPrimitiveSignature()? $"auto TIp{i} = GetParameterType(method, {i});":"")}"))}

    PersistentObjectInfo* delegateInfo = GetDelegateInfo(target);
    auto L = delegateInfo->L;
    if (!L)
    {{
        //todo thwrow exception
        return;
    }}
    int oldTop = lapi_lua_gettop(L);
    int errFunc = lapi_pcall_prepare(L, errorFunc_ref, delegateInfo->reference);
    
    {string.Join("\n",bridgeInfo.ParameterSignatures.Select((s,i)=>$"CSAnyToLuaValue(L,p{i});"))}
    int n = lapi_lua_pcall(L, {bridgeInfo.ParameterSignatures.Count}, 0, errFunc);
    if(!n){{
        {ReturnToCS(bridgeInfo.ReturnSignature)}
    }}else{{
       //todo throw exception 
    }}
    lapi_lua_settop(L, errFunc-1);
}}
";

        }
        public static string genGetField(SignatureInfo fieldWrapperInfo)
        {
            var signature = fieldWrapperInfo.ReturnSignature;
            if ((signature.StartsWith(TypeUtils.TypeSignatures.StructPrefix) || signature.StartsWith(TypeUtils.TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            { 
                //valuetype todo 生成代码有问题
                return $@"{SToCPPType(fieldWrapperInfo.ReturnSignature)} ret;
    xlua:GetFieldValue({(needThis(fieldWrapperInfo.ThisSignature) ? "self, " : "nullptr, ")}(FieldInfo*)fieldInfo, offset, &ret);
    {ReturnToLua(fieldWrapperInfo.ReturnSignature)}            
                ";          
            }
            else
            {
                return $@"{SToCPPType(fieldWrapperInfo.ReturnSignature)} ret;

    xlua:GetFieldValue({(needThis(fieldWrapperInfo.ThisSignature) ? "self, " : "nullptr, ")}(FieldInfo*)fieldInfo, offset, &ret);
    
    {ReturnToLua(fieldWrapperInfo.ReturnSignature)}";
            }
        }
        static List<string> list = new List<string>() { "o", "s", "p", "a" };
        public static string genFieldWrapper(SignatureInfo fieldWrapperInfo)
        {
            return $@"
static void ifg_{fieldWrapperInfo.Signature}(lua_State* L, void* fieldInfo, size_t offset, void* TIret) {{
    
    {(needThis(fieldWrapperInfo.ThisSignature) ? GetThis(fieldWrapperInfo.ThisSignature, true) : "")}

    {genGetField(fieldWrapperInfo)}
}}

static void ifs_{fieldWrapperInfo.Signature}(lua_State* L, void* fieldInfo, size_t offset, void* TIp) {{
    
    {(needThis(fieldWrapperInfo.ThisSignature) ? GetThis(fieldWrapperInfo.ThisSignature, true) : "")}

    {LuaValToCSVal(fieldWrapperInfo.ReturnSignature, "p", 3, false, true)}
    xlua:SetFieldValue({(needThis(fieldWrapperInfo.ThisSignature) ? "self, " : "nullptr, ")}(FieldInfo*)fieldInfo, offset, {(list.IndexOf(fieldWrapperInfo.Signature) != -1 ? "p" : "&p")});
}}";
        }
    }

}