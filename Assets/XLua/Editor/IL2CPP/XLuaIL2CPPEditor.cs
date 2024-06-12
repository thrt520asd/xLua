using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using UnityEditor;
using UnityEngine;
using XLua.IL2CPP;
using XLua.IL2CPP.Editor.Generator;
namespace XLua.IL2CPP.Editor{
    public class XLuaIL2CPPEditor
    {

        /// <summary>
        /// 生成wrap代码
        /// </summary>
        [MenuItem("Tools/XLuaIL2CPP/GenFunctionBridge.gen.h")]
        public static void GenFunctionBridge(){
            FileExporter.GenCPPWrapAll();
        }

        [MenuItem("Tools/XLuaIL2CPP/GenHash")]
        public static void GenHash(){
            MemberNameHashGen.Generate(new List <Type>(){typeof(ClassLuaCallCS), typeof(GameObject)}, false);
        }
#if AOE_APP
        /// <summary>
        /// 生成wrap代码
        /// </summary>
        [MenuItem("Tools/XLuaIL2CPP/GenAll")]
        public static void GenerateAOEWrap(){
             CSObjectWrapEditor.Generator.GetGenConfig(XLua.Utils.GetAllTypes());
             var types = CSObjectWrapEditor.Generator.LuaCallCSharp.Concat(CSObjectWrapEditor.Generator.CSharpCallLua);
             FileExporter.GenCPPWrap(types, FileExporter.filePath);
        }
#endif
        /// <summary>
        /// 生成LuaAPIAdapt代码
        /// </summary>
        [MenuItem("Tools/XLuaIL2CPP/GenLuaAdapt")]
        public static void GenLuaAPIAdapt(){
            LuaAPIAdaptGenerator.Gen();
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
            
            if(GUILayout.Button("TypeInfo")){
                var type = System.AppDomain.CurrentDomain.GetAssemblies().SelectMany(x => x.GetTypes()).First((Type x)=>{
                    return x.Name == m_TypeName;
                });
                if(type != null){
                    Debug.Log("======================selfSignature======================");
                    var thisSignature = TypeUtils.GetTypeSignature(type);
                    Debug.Log($"{type}:{thisSignature}");
                    BindingFlags flag = BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public;
                    var ctors = type.GetConstructors(flag);
                    var methods = type.GetMethods(flag);
                    var properties = type.GetProperties(flag);
                    var fields = type.GetFields(flag);
                    var extensionMethods = Utils.GetExtensionMethodsOf(type);
                    if(ctors != null){
                        Debug.Log("======================ctor======================");
                        foreach (var item in ctors)
                        {   
                            Debug.Log(item+"|"+ item.Name + "|"+TypeUtils.GetMethodSignature(item)+"|"+item.IsStatic + "|" + item.IsSpecialName);
                        }
                    }

                    if(methods != null){
                        Debug.Log("======================methods======================");
                        foreach (var item in methods)
                        {   
                            Debug.Log(item+"|"+ item.Name+ "|"+TypeUtils.GetMethodSignature(item, false, false)+"|"+item.IsStatic + "|" + item.IsSpecialName);
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

                    if(extensionMethods != null){
                        Debug.Log("======================extensionMethods======================");
                        foreach (var item in extensionMethods)
                        {   
                            Debug.Log(item+"|"+ item.Name+ "|"+TypeUtils.GetMethodSignature(item, false, false)+"|"+item.IsStatic);
                        }
                    }
                }
            }
            if(GUILayout.Button("SToCPPType")){
                Debug.Log(WrapUtil.SToCPPType(m_TypeName));
            }
            if(GUILayout.Button("ReflectionInfo")){
                
                var type = System.AppDomain.CurrentDomain.GetAssemblies().SelectMany(x => x.GetTypes()).First((Type x)=>{
                        return x.Name == m_TypeName;
                    });

                var fields = type.GetFields();
                Debug.Log(type.FullName);
                foreach (var field in fields)
                {   
                    Debug.Log(field + "|" + field.FieldType.IsByRef + "|" + field.FieldType.IsPointer);
                }
                BindingFlags flag = BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public;
                var methods = type.GetMethods(flag);
                foreach (var method in methods)
                {   
                    Debug.Log(method);
                }
            }
            if(GUILayout.Button("TestGen")){
                var method =  typeof(ObjectTranslator).GetMethod("PushByType");
                Debug.Log(method);
                
                // var type = System.AppDomain.CurrentDomain.GetAssemblies().SelectMany(x => x.GetTypes()).First((Type x)=>{
                //         return x.Name == m_TypeName;
                //     });
                
                // Debug.Log(type.FullName);
                // Debug.Log(type.Name);

                // FileExporter.GenCPPWrap(new List<Type>(){type}, Application.dataPath + "/../wrapTest.h");
            }
        }

    }

}
