using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using UnityEditor;
using UnityEngine;
using XLua.IL2CPP;
namespace XLua.IL2CPP.Editor{
    public class XLuaIL2CPPEditor
    {

        /// <summary>
        /// 生成wrap代码
        /// </summary>
        [MenuItem("Tools/XLuaIL2CPP/GenFunctionBridge.gen.h")]
        public static void GenFunctionBridge(){
            Generator.FileExporter.GenCPPWrap(Application.dataPath+"/FunctionBridge.gen.h");
        }
    }



    public class TypeInfoEditorWindow:EditorWindow{

        [MenuItem("Tools/TypeInfoEditorWindow")]
        public static void ShowTypeInfoEditorWindow(){
            var v3Type = typeof(Vector3);
            Debug.Log(v3Type.Name);
            EditorWindow.GetWindow<TypeInfoEditorWindow>().Show();
        }

        private string m_TypeName = "";
        void OnGUI(){
            EditorGUILayout.LabelField("TypeName");
            m_TypeName = EditorGUILayout.TextField(m_TypeName);
            
            if(GUILayout.Button("Generate")){
                var type = System.AppDomain.CurrentDomain.GetAssemblies().SelectMany(x => x.GetTypes()).First((Type x)=>{
                    return x.Name == m_TypeName;
                });
                if(type != null){
                    Debug.Log("======================selfSignature======================");
                    var thisSignature = TypeUtils.GetTypeSignature(type);
                    Debug.Log($"{type}:{thisSignature}");
                    var ctors = type.GetConstructors();
                    var methods = type.GetMethods();
                    var properties = type.GetProperties();
                    var fields = type.GetFields();
                    if(ctors != null){
                        Debug.Log("======================ctor======================");
                        foreach (var item in ctors)
                        {   
                            Debug.Log(item+"|"+ item.Name + "|"+TypeUtils.GetMethodSignature(item)+"|"+item.IsStatic);
                        }
                    }

                    if(methods != null){
                        Debug.Log("======================methods======================");
                        foreach (var item in methods)
                        {   
                            Debug.Log(item+"|"+ item.Name+ "|"+TypeUtils.GetMethodSignature(item, false, false)+"|"+item.IsStatic);
                        }
                    }

                    if(properties != null){
                        Debug.Log("======================properties======================");
                        foreach (var item in properties)
                        {   
                            Debug.Log(item+"|"+ item.Name);
                            var getter = item.GetGetMethod();
                            Debug.Log(TypeUtils.GetMethodSignature(getter)+"|"+getter.IsStatic);
                            var setter = item.GetSetMethod();
                            Debug.Log(TypeUtils.GetMethodSignature(setter)+"|"+setter.IsStatic);
                        }
                    }

                    if(fields != null){
                        Debug.Log("======================fields======================");
                        foreach (var field in fields)
                        {   
                            string signature = (field.IsStatic ? "" : "t") + TypeUtils.GetTypeSignature(field.FieldType);
                            Debug.Log(signature+"|"+ field.Name + "|" + field.IsStatic);
                        }
                    }
                }
            }
        }

    }

}
