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
using System.Text.RegularExpressions;
namespace XLua.IL2CPP.Editor.Generator
{
    
    public static class WrapUtil
    {
        public static bool IsVoid(this string str){
            return str == TypeSignatures.Void;
        }
        public static bool IsStruct(this string str){
            str = getSignatureWithoutRefAndPrefix(str);
            return (str.StartsWith(TypeSignatures.StructPrefix) || str.StartsWith(TypeSignatures.NullableStructPrefix)) && str.EndsWith("_");
        }
        
        public static bool IsStructRaw(this string str){
            return (str.StartsWith(TypeSignatures.StructPrefix) || str.StartsWith(TypeSignatures.NullableStructPrefix)) && str.EndsWith("_");
        }

        public static bool IsBoolean(this string str){
            return str == TypeSignatures.Bool;
        }
        public static string CSType(this string str){
            if(str[0] == 'V'){
                return SToCPPType(str.Substring(1));
            }
            return SToCPPType(str.Substring(1));
        }
        
        public static string CSTypeNoPointer(this string str){
            if(str[0] == 'V'){
                return str.Substring(1).CSTypeNoPointer();
            }
            if(str.IsOutParameter()){
                return str.Substring(1).CSTypeNoPointer();
            }
            if(str.IsRefParameter()){
                return str.Substring(1).CSTypeNoPointer();
            }
            return SToCPPType(str);
        }
        public static bool IsOutParameter(this string str){

            return str[0] == TypeSignatures.OutParameterPrefix[0];
        }
        public static bool IsRefParameter(this string str){
            return (str[0] == TypeSignatures.OutParameterPrefix[0]
            || str[0] == TypeSignatures.RefOrPointerPrefix[0]) && str[1] != 'v';
        }

        public static bool IsRefStruct(this string str){
            return str.IsRefParameter() && str.Substring(1).IsStruct();
        }

        public static bool IsOptionalParameter(this string str){
            return str[0] == TypeSignatures.OptionalParmaeterPrefix[0];
        }

        public static string GetLastNum(this string str){
            var match = Regex.Match(str, @"\d+$");
            if(match.Success){
                return match.Value;
                // return int.Parse(match.Value);
            }else{
                return "";
            }
        }

        public static string GetTipClass(this string signature){
            if(signature == "ret"){
                return "TIret";
            }else{
                return "TIp"+signature.GetLastNum();
            }
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
            return thisSignature == TypeSignatures.ThisSignature_OBJECT || 
            thisSignature == TypeSignatures.ThisSignature_Value || 
            thisSignature == TypeSignatures.ThisSignature_Value_Offset || 
            thisSignature == TypeSignatures.ThisSignature_Ref;
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
            if (signature.StartsWith(TypeSignatures.StructPrefix) || signature.StartsWith(TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                t = $"{signature}";
            }
            if (signature.IsRefParameter())
            {
                t = $"{SToCPPType(signature.Substring(1))}*";
            }
            return t;
        }

        public static string SToCPPType(string signature)
        {
            if (signature.IsOptionalParameter())
            {
                signature = signature.Substring(1);
            }
            var t = signature.IsPrimitiveSignatureRaw() ? PrimitiveSignatureCppTypeMap[signature] : "void*";
            if (signature.StartsWith(TypeSignatures.StructPrefix) || signature.StartsWith(TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                t = $"struct {signature}";
            }
            if(signature.IsRefParameter()){                
                t = $"{SToCPPType(signature.Substring(1))}*";
            }
            return t;
        }
        private static string getSignatureWithoutRefAndPrefix(string signature)
        {
            if (signature.IsRefParameter() || signature[0] == 'D')
            {
                return signature.Substring(1);
            }
            else
            {
                return signature;
            }
        }

        // 获取真实参数个数
        public static int TriOutParamCnt(this List<string> list){
            int cnt = 0;
            foreach (var item in list)
            {
                if(!item.IsOutParameter()){
                    cnt++;
                }
            }
            return cnt;
        }

        public static int LuaFuncReturnValueCnt(this SignatureInfo signatureInfo){
            int cnt = 0;
            if(!signatureInfo.ReturnSignature.IsVoid()){
                cnt++;
            }
            foreach (var item in signatureInfo.ParameterSignatures)
            {
                if(item.IsRefParameter()){
                    cnt++;
                }
            }
            return cnt;
        }
        
        /// <summary>
        /// lua返回的参数在栈的offset
        /// </summary>
        /// <param name="list"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public static int LuaReturnRefValueRealIndex(SignatureInfo signatureInfo, int index){
            int idx = signatureInfo.ReturnSignature.IsVoid() ? 0 : 1;
            for (int i = 0; i < Math.Min(index, list.Count - 1); i++)
            {
                if(signatureInfo.ParameterSignatures[i].IsRefParameter()){
                    idx ++;
                }
            }
            return idx;
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
    void* klass = wrapData->klass;
    if({(signatureInfo.ParameterSignatures.Any(s => s[0] == 'D') ? "true" : "checkLuaArgument")}){{
        if ({WrapUtil.GenArgsLenCheck(signatureInfo.ParameterSignatures)}) return -1;
        {String.Join("\n\t\t", signatureInfo.ParameterSignatures.Select<string, string>((s, index) => WrapUtil.CheckLuaArgument(s, RealParameterIndex(signatureInfo.ParameterSignatures, index), index)))}
    }}
    {WrapUtil.GetThis(signatureInfo.ThisSignature)}
    {String.Join("\n\t", signatureInfo.ParameterSignatures.Select((s, index) => WrapUtil.LuaValToCSVal(s, "p" + index,  RealParameterIndex(signatureInfo.ParameterSignatures, index).ToString() , true)))}
    
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
            
            for (; requireNum < parameterSignatures.Count && parameterSignatures[requireNum][0] != 'V' && parameterSignatures[requireNum][0] != 'D' && !parameterSignatures[requireNum].IsOutParameter(); ++requireNum) { }
            return requireNum != parameterSignatures.Count ? $"length < paramOffset + {requireNum - 1} " : $"length != paramOffset + {parameterSignatures.Count - 1}";
        }


        public static string CheckLuaArgument(string signature, int index, int typeIndex)
        {
            var ret = "";
            string TypeInfoIndex = index.ToString();
            bool isParamArray = false;
            if (signature[0] == 'D') //optional
            {
                ret += $"if (length > {index} + paramOffset && ";
                signature = signature.Substring(1);
            }
            else if(signature[0] == 'V'){
                isParamArray = true;
                ret += $"if(length >= {index} + paramOffset && (lapi_lua_type(L, {index} + paramOffset) == LUA_TNONE ||";
                signature = signature.Substring(1);
            }
            else if(signature.IsOutParameter()){
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
                    ret += $"!lapi_lua_isboolean(L, {index} + paramOffset)){(isParamArray ? ")" :"")} return -1;";
                }
                else if (signature == "u1" || signature == "i1" || signature == "i2" || signature == "u2" || signature == "u4" || signature == "i4"
                )
                {
                    ret += $"!(lapi_lua_type(L, {index} + paramOffset) == LUA_TNUMBER)){(isParamArray ? ")" :"")} return -1;";
                    // ret += $"!lapi_lua_isnumber(L, {index} + paramOffset)) return -1;";
                }
                else if(signature == "i8"){
                    ret += $"!lapi_lua_isint64(L, {index} + paramOffset)){(isParamArray ? ")" :"")} return -1;";
                }
                else if(signature == "u8"){
                    ret += $"!lapi_lua_isuint64(L, {index} + paramOffset)){(isParamArray ? ")" :"")} return -1;";
                }
                else if (signature == "c")
                {
                    ret += $"!(lapi_lua_type(L, {index} + paramOffset) == LUA_TNUMBER)){(isParamArray ? ")" :"")} return -1;";
                    // ret += $"!lapi_lua_isnumber(L, {index} + paramOffset)) return -1;";
                }
                else if (signature == "r8" || signature == "r4")
                {
                    ret += $"!(lapi_lua_type(L, {index} + paramOffset) == LUA_TNUMBER)){(isParamArray ? ")" :"")} return -1;";
                    // ret += $"!lapi_lua_isnumber(L, {index} + paramOffset)) return -1;";
                }else{
                    ret = $"// invalid check lua args signature {signature}";
                }
            }
            else if (signature == "p" || signature == "Pv")
            { 
                // IntPtr, void*
                ret +=$"!lapi_lua_isuserdata(L, {index} + paramOffset)){(isParamArray ? ")" :"")} return -1;";
            }
            
            else if (signature.IsRefParameter()) //引用传递
            {
                if(signature.Substring(1).IsPrimitiveSignature()){
                    return CheckLuaArgument(signature.Substring(1), index, typeIndex);
                }else if(signature.Substring(1).IsStructRaw()){
                    return CheckLuaArgument(signature.Substring(1), index, typeIndex);
                }
                //#TODO@benp 更细致的检测
                ret +=$"!lapi_lua_isuserdata(L, {index} + paramOffset)) return -1;";
            }
            else if (signature == "s")
            {
                ret += $"!CheckIsString(L, {index} + paramOffset)){(isParamArray ? ")" :"")} return -1;";
            }
            else if (signature == "o")
            {
                ret += $"!CheckIsClass(L, {index} + paramOffset, TIp{typeIndex})){(isParamArray ? ")" :"")} return -1;";
            }
            else if (signature == TypeSignatures.SystemObject)
            {
                return "";//可变参数 也是不检测的
                // if(!isParamArray){
                //     return "";
                // }else{
                    
                // }
                // ret += $"!CheckIsClass(L, {index} + paramOffset, TIp{typeIndex})) return -1;";
            }
            else if (signature.StartsWith(TypeSignatures.StructPrefix) && signature.EndsWith("_")){
                ret +=$"!CheckIsStruct(L, {index} + paramOffset, TIp{typeIndex})){(isParamArray ? ")" :"")} return -1;";
            }
            else if (signature.StartsWith(TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                ret +=$"!CheckIsNullableStruct(L, {index} + paramOffset, TIp{typeIndex})){(isParamArray ? ")" :"")} return -1;";
            }
            else
            {
                ret += $"!!true){(isParamArray ? ")" :"")} return -1;";
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
            if(signature == TypeSignatures.ThisSignature_OBJECT){
                return $"auto self = LuaValueToCSRef(nullptr,L, 1);";
            }
            else if(signature == TypeSignatures.ThisSignature_Ref){
                return $"auto self = GetRawObjPointer(L, 1, klass);";
            }
            else if(signature == TypeSignatures.ThisSignature_Value){
                
                return $"auto self = GetRawValuePointer(L, 1, klass);";
            }
            else if(signature == TypeSignatures.ThisSignature_Value_Offset){
                return $"auto self = GetRawValuePointer_Offset(L, 1, klass);";
            }
            return "";
            
        }
                  
        public static string LuaValToCSVal(string signature, string CSName, string index, bool needOffset = false, bool isField = false)
        {
            
            if (signature == TypeSignatures.String)
            {
                return
$@"// string s
    void* {CSName} = LuaStr2CSharpString(L, {index}{ (needOffset?"+ paramOffset":"")} );";
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
            //string out
            else if (signature == "Us")
            { 
                return
$@"// string out Ps
    void* u{CSName} = nullptr; // string out
    void** {CSName} = &u{CSName};
";
            }
            //object
            else if (signature == "O" || signature == "o")
            { 
                return 
$@"// object
    void* {CSName} = (void*)xlua::LuaValueToCSRef((Il2CppClass*)TI{CSName}, L, {index}{ (needOffset?"+ paramOffset":"")});";
            }
            //object ref
            else if (signature == "Po" || signature == "PO")
            {
                return 
$@"// object ref Po/PO
    void* u{CSName} = nullptr; // object ref
    void** {CSName} = &u{CSName};
    u{CSName} = (void*)LuaValueToCSRef((Il2CppClass*)TI{CSName},L, {index}{ (needOffset?"+ paramOffset":"")});";
            }
            //object ref
            else if (signature == "Uo" || signature == "UO")
            {
                return 
$@"// object ref Uo/UO
    void* u{CSName} = nullptr; // object ref
    void** {CSName} = &u{CSName};";
            }
            //struct
            else if ((signature.StartsWith(TypeSignatures.StructPrefix) || signature.StartsWith(TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            {
                    return 
$@"//LuaValToCSVal struct
    {signature}* p{CSName} = GetCSharpStructPointerFromLua<{signature}>(L, {index}{ (needOffset?"+ paramOffset":"")}, (Il2CppClass*){CSName.GetTipClass()});
    {signature} {CSName} = p{CSName} ? *p{CSName} : {signature} {{}};";
            }
            else if(signature.IsOutParameter()){
                if(signature.IsPrimitiveSignature()){
                   return
$@"{signature.CSTypeNoPointer()}* {CSName} = nullptr; // {signature} out
    {signature.CSTypeNoPointer()} u{CSName} = {(signature.Substring(1) == "b" ? "false" : "0")};
    {CSName} = &u{CSName};";
                }else if(signature.Substring(1).IsStruct()){
                    //此处后续可以深究
                    return
$@"
{signature.CSTypeNoPointer()} u{CSName} = {{}}; // {signature};out
{signature.CSTypeNoPointer()}* {CSName} = &u{CSName};  
";
                }
                else{
                return 
$@"// LuaValToCSVal out not primitive
//void* u{CSName} = nullptr; //out ref
    //void** {CSName} = &u{CSName};
    {WrapUtil.SToCPPType(signature)} {CSName} = {GetArgValue(signature.Substring(1), index, true, needOffset)}; // {signature}";
                }
            }
            //valuetype ref
            else if (
                //(signature.StartsWith('P' + TypeSignatures.StructPrefix) || signature.StartsWith('P' + TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_")
                signature.IsRefStruct()
            )
            { 
                var realS = signature.Substring(1);
                return 
$@"//LuaValToCSVal Pstruct
    {realS}* {CSName} = GetCSharpStructPointerFromLua<{realS}>(L, {index}{ (needOffset?"+ paramOffset":"")}, (Il2CppClass*){CSName.GetTipClass()});
    if(!{CSName}){{
        {realS} u{CSName};
        memset(&u{CSName}, 0, sizeof({realS}));
        {CSName} = &u{CSName};
    }}";
            }
            else if (signature.IsRefParameter())
            {
                string s = signature.Substring(1);
                if(signature.Substring(1).IsPrimitiveSignatureRaw()){
                    return 
$@"//LuaValToCsVal P primitive
    {WrapUtil.SToCPPType(s)} u{CSName} = {GetArgValue(s, index, true, needOffset)};
    {WrapUtil.SToCPPType(s)}* {CSName} = &u{CSName};
";
                }
                else{
                return 
$@"//LuaValToCsVal P not primitive
    {WrapUtil.SToCPPType(signature)} {CSName} = {GetArgValue(signature.Substring(1), index, true, needOffset)}; // {signature}";
                }
            }
            else if (signature[0] == 'V') // array 
            {
                var elementSignature = signature.Substring(1);
                if(elementSignature.IsPrimitiveSignatureRaw()){
                    return $"void* {CSName} = RestArguments<{PrimitiveSignatureCppTypeMap[elementSignature]}>::PackPrimitive(L, {index} + paramOffset, TIp{CSName.GetLastNum()});";
                }else if(elementSignature == "s"){
                    return $"void* {CSName} = RestArguments<void*>::PackString(L, {index} + paramOffset, TIp{CSName.GetLastNum()});";
                }else if(elementSignature == "o" || elementSignature == "O"){
                    return $"void* {CSName} = RestArguments<void*>::PackRef(L, {index} + paramOffset, TIp{CSName.GetLastNum()});";
                }else if(elementSignature.IsStructRaw()){
                    return $"void* {CSName} = RestArguments<{elementSignature}>::PackValueType(L, {index} + paramOffset, TIp{CSName.GetLastNum()});";
                }else{
                    return $"void* {CSName} = nullptr; // unknown array type {signature}";
                }
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
    void* {CSName} = xlua::OptionalParameter<void*>::GetRefType(L,{index}{ (needOffset?"+ paramOffset":"")}, method, wrapData, {index}, TIp{CSName.GetLastNum()});";
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

        public static string GetArgValue(string signature, string index, bool isRef = false, bool needOffset = false){
            if(PrimitiveSignatureCppTypeMap.ContainsKey(signature)){
                return $@"converter::Converter<{PrimitiveSignatureCppTypeMap[signature]}>::toCpp(L, {index}{ (needOffset?"+ paramOffset":"")});";
            }else if((signature == "Pv" || signature == "p") && !isRef){
                return $"lapi_lua_touserdata(L, {index}{ (needOffset?"+ paramOffset":"")});";
            }else{
                //default value
                if(signature.IsPrimitiveSignatureRaw()){
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
            var rawSignature = signature;
            if(signature.IsOutParameter()){
                return "";
            }
            if(signature.IsRefParameter()){
                signature = signature.Substring(1);
            }
            if (PrimitiveSignatureCppTypeMap.ContainsKey(signature))
            {
                return $"converter::Converter<{PrimitiveSignatureCppTypeMap[signature]}>::toScript(L, {(rawSignature.IsRefParameter()?"*":"")}{CSName});";
            }
            else if (signature == "s" || signature == "O")
            {
                return $@"CSAnyToLuaValue(L, {(rawSignature.IsRefParameter()?"*":"")}{CSName});";
            }
            else if (signature == "o" || signature == "a")
            {
                return $@"CSAnyToLuaValue(L, {(rawSignature.IsRefParameter()?"*":"")}{CSName});";
            }
            else if (signature.StartsWith(TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                
                return $"CopyNullableValueType(L, TI{CSName.TrimStartXing()}, {(rawSignature.IsRefParameter()?"*":"")}{CSName}, {CSName}->hasValue, sizeof({signature}));";
            }
            else if (signature.StartsWith(TypeSignatures.StructPrefix) && signature.EndsWith("_"))
            {
                return $"CSValueToLuaValue(L, (void*)TI{CSName.TrimStartXing()}, {(rawSignature.IsRefParameter()?"*":"")}{CSName.CSNamePointer()}, sizeof({signature}));";
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
            if (signature.IsRefParameter())
            {
                var elementSignature = signature.Substring(1);
                string CSName = "p"+index;
                string returnStr = "";
                if(elementSignature.IsPrimitiveSignatureRaw() && elementSignature != TypeSignatures.Void){
                    returnStr = $"converter::Converter<{PrimitiveSignatureCppTypeMap[elementSignature]}>::toScript(L, *{CSName})";
                }
                else if (elementSignature == "s" || elementSignature == "O")
                {
                    returnStr = $@"CSAnyToLuaValue(L, *{CSName}, (void*)TIp{CSName.GetLastNum()})";
                }
                else if (elementSignature == "o" || elementSignature == "a")
                {
                    returnStr = $@"CSAnyToLuaValue(L, *{CSName}, (void*)TIp{CSName.GetLastNum()})";
                }
                else if (elementSignature.StartsWith(TypeSignatures.NullableStructPrefix) && elementSignature.EndsWith("_"))
                {
                    
                    returnStr = $"CopyNullableValueType(L, (void*)TI{CSName.TrimStartXing()}, {CSName}, {CSName}->hasValue, sizeof({elementSignature}))";
                }
                else if (elementSignature.StartsWith(TypeSignatures.StructPrefix) && elementSignature.EndsWith("_"))
                {
                    returnStr = $"CSValueToLuaValue(L, (void*)TI{CSName.TrimStartXing()}, {CSName}, sizeof({elementSignature}))";
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

        public static string ReturnToLua(string signature, string klass = "")
        {
            if(signature.IsVoid()) return "";
            if (PrimitiveSignatureCppTypeMap.ContainsKey(signature))
            {   
                
                return $"converter::Converter<{PrimitiveSignatureCppTypeMap[signature]}>::toScript(L, ret);";
            }
            else if (signature.StartsWith(TypeSignatures.NullableStructPrefix) && signature.EndsWith("_"))
            {
                return "CopyNullableValueType(L, (void*)TIret, &ret, ret.hasValue, sizeof(ret));";
            }
            else if (signature.StartsWith(TypeSignatures.StructPrefix) && signature.EndsWith("_"))
            {
                return $"CSValueToLuaValue(L, (void*)TIret, &ret, sizeof({signature}));";
            }
            else if (signature == "o")
            {
                return $"CSAnyToLuaValue(L, ret{(klass.NotNullOrEmpty()?", TIret":"")});";
            }
            else if (signature == "O")
            { // System.Object
                return $"CSAnyToLuaValue(L, ret{(klass.NotNullOrEmpty()?", TIret":"")});";
            }
            else if (signature == "s")
            { // string
                return $"CSAnyToLuaValue(L, ret{(klass.NotNullOrEmpty()?", TIret":"")});";
            }
            else if (signature == "p" || signature == "Pv")
            { // IntPtr, void*
              return $@"lapi_lua_pushlightuserdata(L, ret);";
            }
            else
            {
                return "// unknow ret signature: ${signature}";
            }
        }

        public static string ReturnToCS(string signature, string index){
            if(signature == TypeSignatures.Void){
                return "";
            }
            return 
$@"{LuaValToCSVal(signature, "ret", index)}
    return ret;";
        }
        /// <summary>
        /// 返回异常情况下默认值
        /// </summary>
        /// <param name="signature"></param>
        /// <returns></returns>
        public static string GenDefaultReturnValue(string signature){
            string str = "return ";
            if(signature.IsStruct()){
                str += "{}";
            }else if(signature.IsBoolean()){
                str += "false";
            }else if(signature == "v"){

            }else if(PrimitiveSignatureCppTypeMap.ContainsKey(signature)){
                str += "0";
            }else{
                str += "nullptr";
            }
            str += ";";
            return str;
        }
        public static string GetCSMethodInvokeReturnValue(string signature){
            if(signature.IsVoid()){
                return "";
            }
            return $"{SToCPPType(signature)} ret = ";
        }
        

        public static string BridgeRefParamterSetBack(string signature, string pi, string index){
            if(signature.IsRefParameter()){
                var elementSignature = signature.Substring(1);
                if(elementSignature.IsPrimitiveSignatureRaw()){
                    return $"*{pi} = converter::Converter<{PrimitiveSignatureCppTypeMap[elementSignature]}>::toCpp(L, {index});";
                }else if (elementSignature == "s" || elementSignature == "O"||elementSignature == "o" || elementSignature == "a"
                )
                {
                    return $"*{pi} = LuaValueToCSRef((Il2CppClass*)TI{pi}, L, {index});";
                } else if (elementSignature.StartsWith(TypeSignatures.NullableStructPrefix) && elementSignature.EndsWith("_"))
                {
                    
                    return $"{pi} = GetCSharpStructPointerFromLua<{signature}>(L, {index}, (void*)TI{pi});";
                }
                else if (elementSignature.StartsWith(TypeSignatures.StructPrefix) && elementSignature.EndsWith("_"))
                {
                    //todo 此处需要深究
                    return $"{pi} = GetCSharpStructPointerFromLua<{elementSignature}>(L, {index}, (void*)TI{pi});";
                }
                
                return $"*{pi} = nullptr//todo BrdigeRefParamterSetBack{signature}";
            }
            return "";
        }

        /// <summary>
        /// 通过luaFunc构造的delegate 不支持默认参数  支持引用参数
        /// </summary>
        /// <param name="bridgeInfo"></param>
        /// <returns></returns>
        public static string genBridge(SignatureInfo bridgeInfo)
        {
            var hasVarArgs = bridgeInfo.ParameterSignatures.Count > 0 && bridgeInfo.ParameterSignatures[bridgeInfo.ParameterSignatures.Count - 1][0] == 'V';
            int errFuncOffset = bridgeInfo.ReturnSignature == "v" ? 1 :2;
            return 
$@"//{bridgeInfo.CsName}
static int bw_{bridgeInfo.Signature}(lua_State* L, Il2CppDelegate* ildelegate,const MethodInfo* method) {{
    {(bridgeInfo.ReturnSignature.NotNullOrEmpty() && !bridgeInfo.ReturnSignature.IsPrimitiveSignature() ? "auto TIret = GetReturnType(method);":"")}
    {string.Join("\n", bridgeInfo.ParameterSignatures.Select((s,i)=>$"{(!s.IsPrimitiveSignature()? $"auto TIp{i} = GetParameterType(method, {i});":"")}"))}
    
    {String.Join("\n\t", bridgeInfo.ParameterSignatures.Select((s, index) => WrapUtil.LuaValToCSVal(s, "p" + index,  (RealParameterIndex(bridgeInfo.ParameterSignatures, index) + 2).ToString() , false)))}
    const MethodInfo* invoke = il2cpp::vm::Runtime::GetDelegateInvoke(ildelegate->object.klass);
    typedef {WrapUtil.SToCPPType(bridgeInfo.ReturnSignature)}(*Func)(void* target, {String.Join("", bridgeInfo.ParameterSignatures.Select((s, i) => $"{WrapUtil.SToCPPType(s)} p{i}, "))}const void* method);
    {GetCSMethodInvokeReturnValue(bridgeInfo.ReturnSignature)} ((Func)invoke->methodPointer)(ildelegate, {String.Join("", bridgeInfo.ParameterSignatures.Select((s, i) => $"p{i}, "))}invoke);
    

    {WrapUtil.ReturnToLua(bridgeInfo.ReturnSignature)}

    {String.Join("", bridgeInfo.ParameterSignatures.Select((s, i) => WrapUtil.refSetback(s, i, bridgeInfo)))}
    return {GetFuncRetCnt(bridgeInfo)};
}}
//{bridgeInfo.CsName}
static {SToCPPType(bridgeInfo.ReturnSignature)} b_{bridgeInfo.Signature}(void* target, {
    string.Join("",bridgeInfo.ParameterSignatures.Select((s,i)=>$"{SToCPPType(s)} p{i},"))
}MethodInfo* method) {{
    {(bridgeInfo.ReturnSignature.NotNullOrEmpty() && !bridgeInfo.ReturnSignature.IsPrimitiveSignature() ? "auto TIret = GetReturnType(method);":"")}
    {string.Join("\n", bridgeInfo.ParameterSignatures.Select((s,i)=>$"{(!s.IsPrimitiveSignature()? $"auto TIp{i} = GetParameterType(method, {i});":"")}"))}

    DelegateMiddlerware* delegateInfo = GetDelegateInfo(target);
    auto L = delegateInfo->L;
    if (!L)
    {{
        Exception::Raise(Exception::GetInvalidOperationException(""lua bridge has disposed""));
        
        {GenDefaultReturnValue(bridgeInfo.ReturnSignature)}
    }}
    int oldTop = lapi_lua_gettop(L);
    int errFunc = lapi_pcall_prepare(L, errorFunc_ref, delegateInfo->reference);
    
    {string.Join("\n",bridgeInfo.ParameterSignatures.Select((s,i)=>CSValToLuaVal(s, "p"+i)))}
    int n = lapi_lua_pcall(L, {bridgeInfo.ParameterSignatures.TriOutParamCnt()}, {bridgeInfo.LuaFuncReturnValueCnt()}, errFunc);
    if(!n){{
        {string.Join("\n",bridgeInfo.ParameterSignatures.Select((s,i)=>BridgeRefParamterSetBack(s, "p"+i, "errFunc +" + (LuaReturnRefValueRealIndex(bridgeInfo, i) + 1))))}
        {ReturnToCS(bridgeInfo.ReturnSignature, "errFunc + 1")}
    }}else{{
       throwExceptionFromLuaError(L, oldTop);
    }}
    lapi_lua_settop(L, errFunc-1);
    
}}
";

        }
        public static string genGetField(SignatureInfo fieldWrapperInfo)
        {
            var signature = fieldWrapperInfo.ReturnSignature;
            if ((signature.StartsWith(TypeSignatures.StructPrefix) || signature.StartsWith(TypeSignatures.NullableStructPrefix)) && signature.EndsWith("_"))
            { 
                //valuetype todo 生成代码有问题
                return $@"{SToCPPType(fieldWrapperInfo.ReturnSignature)} ret;
    xlua:GetFieldValue({(needThis(fieldWrapperInfo.ThisSignature) ? "self, " : "nullptr, ")}(FieldInfo*)fieldInfo, offset, &ret);
    {ReturnToLua(fieldWrapperInfo.ReturnSignature, "TIret")}            
                ";          
            }
            else
            {
                return $@"{SToCPPType(fieldWrapperInfo.ReturnSignature)} ret;

    xlua:GetFieldValue({(needThis(fieldWrapperInfo.ThisSignature) ? "self, " : "nullptr, ")}(FieldInfo*)fieldInfo, offset, &ret);
    
    {ReturnToLua(fieldWrapperInfo.ReturnSignature, "TIret")}";
            }
        }
        static List<string> list = new List<string>() { "o", "s", "p", "a" };
        public static string genFieldWrapper(SignatureInfo fieldWrapperInfo)
        {
            return $@"
static void ifg_{fieldWrapperInfo.Signature}(lua_State* L, void* fieldInfo, size_t offset, void* TIret, void* klass) {{
    
    {(needThis(fieldWrapperInfo.ThisSignature) ? GetThis(fieldWrapperInfo.ThisSignature) : "")}

    {genGetField(fieldWrapperInfo)}
}}

static void ifs_{fieldWrapperInfo.Signature}(lua_State* L, void* fieldInfo, size_t offset, void* TIp, void* klass) {{
    
    {(needThis(fieldWrapperInfo.ThisSignature) ? GetThis(fieldWrapperInfo.ThisSignature) : "")}

    {LuaValToCSVal(fieldWrapperInfo.ReturnSignature, "p", "3", false, true)}
    xlua:SetFieldValue({(needThis(fieldWrapperInfo.ThisSignature) ? "self, " : "nullptr, ")}(FieldInfo*)fieldInfo, offset, {(list.IndexOf(fieldWrapperInfo.Signature) != -1 ? "p" : "&p")});
}}";
        }
    }

}