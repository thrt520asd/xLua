using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using UnityEngine;
using XLua.IL2CPP.Editor.Generator;

/// <summary>
/// LuaApi注入生成代码
/// </summary>
public static class LuaAPIAdaptGenerator{

    
    public static string templateFilePath = Application.dataPath +"/XLua/Editor/IL2CPP/LuaAPITemplate.txt";
    public static string XLuaC = Application.dataPath +"/../build/xlua.c";
    public static string Header = Application.dataPath +"/Plugins/xlua_il2cpp/lua_api_adapt.h";
    public static string Body = Application.dataPath +"/Plugins/xlua_il2cpp/lua_api_adapt.c";
    
    const string GenBeginSignature = "//genBegin";
    const string GenEndSignature = "//genEnd";
    public static void Gen(){
        var lines = File.ReadAllLines(templateFilePath);
        List<string> lineList = lines.Where((s)=>s.NotNullOrEmpty() && !s.StartsWith("//")).ToList();
        
        // ClearOldContent();
        ReplaceGenerate(XLuaC, 
$@"static lapi_func_ptr funcs[] = {{
{string.Join("\n", lineList.Select((s,i)=>$"(lapi_func_ptr) &{GetAPIName(s)},//{i}"))}
}};"
);
        ReplaceGenerate(Header, 
$"{string.Join("\n",lineList.Select(s=>$@"//{s.GetAPIName()}{"\n"}{s.GetHeader()}"))}");

        ReplaceGenerate(Body, $@"{string.Join("\n", lineList.Select(s=>GetDeclare(s)))}");

        ReplaceGenerate(Body, $@"{string.Join("\n", lineList.Select((s, i)=>
$@"{s.GetAPINameWithLAPI()}_ptr = ({s.GetAPINameWithLAPI()}Type)func_array[{i}];"))}", 
"//gen1", "//end1");
    }

    private static string GetDeclare(string line){
        string apiName = line.GetAPIName();
        string apiNameType = "lapi_"+apiName;
        return 
$@"//{apiName}
typedef {line.GetHeaderWithType()}
static {apiNameType}Type {apiNameType}_ptr;
{line.Replace(apiName, apiNameType).Replace(";", "").Replace("LUA_API", "").Replace("LUALIB_API", "") }{{
    return {apiNameType}_ptr({line.GetParameters()});
}}
";
    }

    private static void ReplaceGenerate(string filePath, string replace, string beginSig = "", string endSig = ""){
        string content = File.ReadAllText(filePath);
        string result = Regex.Replace(content, $@"({(beginSig.NotNullOrEmpty() ? beginSig : GenBeginSignature)}).*?({ (endSig.NotNullOrEmpty() ? endSig : GenEndSignature)})", $"$1\n{replace}\n$2", RegexOptions.Singleline);
        File.WriteAllText(filePath, result);
    }


    const string pattern = @"\(?(\w+)\)?\s*(\()(?!.*\1)";
    public static string GetAPIName(this string str){
        var match = Regex.Match(str, pattern, RegexOptions.RightToLeft);
        
        return match.Groups[1].Value;
    }
    public static string GetAPINameWithLAPI(this string str){
        var match = Regex.Match(str, pattern, RegexOptions.RightToLeft);
        
        return "lapi_" + match.Groups[1].Value;
    }
    public static string GetParameters(this string str){
        string patternLastParentheses = @"\(([^()]+)\)(?!.*\()";
        Match matchLastParentheses = Regex.Match(str, patternLastParentheses);
        if(matchLastParentheses.Success){
            var match = Regex.Matches(matchLastParentheses.Value,@"(?<=\b\w+\s?\*?\s?)(\w+)(?=[,\)&])");
            string param = "";
            
            for (int i = 0; i < match.Count; i++)
            {
                param = param + match[i].Groups[1].Value + (i == match.Count - 1 ? "" : ",");
            }
            return param;
        }
        return "";
    }

     public static string GetHeaderWithType(this string str){
        
        string apiNameWithCapture = str.GetAPIName();
        string replace = "(*lapi_" + apiNameWithCapture+ "Type)";
        // Debug.Log(apiNameWithCapture);
        // Debug.Log(replace);
        str = str.Replace("LUA_API", "");
        str = str.Replace("LUALIB_API", "");
        str = str.TrimEnd();
        str = str.TrimStart();
        if(!str.EndsWith(";")){
            str+=";";
        }
        
        str = str.Replace(apiNameWithCapture, replace);
        return str;
    }

    public static string GetHeader(this string str){
        var apiName = str.GetAPIName();
        str = str.Replace("LUA_API", "");
        str = str.Replace("LUALIB_API", "");
        str = str.TrimEnd();
        str = str.TrimStart();
        if(!str.EndsWith(";")){
            str+=";";
        }
        str = str.Replace(apiName, "lapi_" + apiName);
        return str;
    }
    public static string ReplaceFirst(this string text, string search, string replace)
    {
        int pos = text.IndexOf(search);
        if (pos < 0)
        {
            return text;
        }
        return text.Substring(0, pos) + replace + text.Substring(pos + search.Length);
    }


}