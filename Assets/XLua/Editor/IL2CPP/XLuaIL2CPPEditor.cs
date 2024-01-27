using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using UnityEditor;
using UnityEngine;
using XLua.IL2CPP;

public class XLuaIL2CPPEditor
{
    
}



public class TypeInfoEditorWindow:EditorWindow{

    [MenuItem("Tools/TypeInfoEditorWindow")]
    public static void ShowTypeInfoEditorWindow(){
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
                var ctors = type.GetConstructors();
                var methods = type.GetMethods();
                var properties = type.GetProperties();
                var fields = type.GetFields();
                if(ctors != null){
                    Debug.Log("======================ctor======================");
                    foreach (var item in ctors)
                    {   
                        Debug.Log(item+"|"+ item.Name + "|"+TypeUtils.GetMethodSignature(item));
                    }
                }

                if(methods != null){
                    Debug.Log("======================methods======================");
                    foreach (var item in methods)
                    {   
                        Debug.Log(item+"|"+ item.Name+ "|"+TypeUtils.GetMethodSignature(item, false, false));
                    }
                }

                if(properties != null){
                    Debug.Log("======================properties======================");
                    foreach (var item in properties)
                    {   
                        Debug.Log(item+"|"+ item.Name);
                        var getter = item.GetGetMethod();
                        Debug.Log(TypeUtils.GetMethodSignature(getter));
                        var setter = item.GetSetMethod();
                        Debug.Log(TypeUtils.GetMethodSignature(setter));
                    }
                }

                if(fields != null){
                    Debug.Log("======================fields======================");
                    foreach (var field in fields)
                    {   
                        string signature = (field.IsStatic ? "" : "t") + TypeUtils.GetTypeSignature(field.FieldType);
                        Debug.Log(signature+"|"+ field.Name);
                    }
                }
            }
        }
    }

}
