using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Runtime.CompilerServices;
using UnityEngine;
using System.Data.SqlTypes;

// using Mono.Cecil;
// using Mono.Cecil.Cil;



namespace XLua.IL2CPP.Editor
{
    namespace Generator
    {

        public class ValueTypeInfo
        {
            public string Signature;
            public string CsName;
            public List<string> FieldSignatures;
            public int NullableHasValuePosition;

            public override string ToString()
            {
                if (string.IsNullOrEmpty(Signature))
                {
                    return "";
                }
                return 
$@"struct {Signature}{{
    {String.Join("\n\t", this.FieldSignatures.Select((s,i)=>
        Signature.StartsWith("N_") && i == NullableHasValuePosition ? 
            WrapUtil.SToCPPType(s)+" hasValue;" : $"{WrapUtil.SToCPPType(s)} p{i};"
    ))}
}};";
                
            }
        }

        public class SignatureInfo
        {
            public string Signature;
            public string CsName;
            public string ReturnSignature;
            public string ThisSignature;
            public List<string> ParameterSignatures;


        }

        public class CppWrappersInfo
        {
            public List<ValueTypeInfo> ValueTypeInfos;

            public List<SignatureInfo> WrapperInfos;

            public List<SignatureInfo> BridgeInfos;

            public List<SignatureInfo> FieldWrapperInfos;
        }

        
        public class FileExporter
        {


            public static Type GetUnrefParameterType(ParameterInfo parameterInfo)
            {
                return (parameterInfo.ParameterType.IsByRef || parameterInfo.ParameterType.IsPointer) ? parameterInfo.ParameterType.GetElementType() : parameterInfo.ParameterType;
            }

            public static void GenericArgumentInInstructions(MethodBase node, HashSet<Type> result, HashSet<MethodBase> proceed, Func<MethodBase, IEnumerable<MethodBase>> callingMethodsGetter)
            {
                var declaringType = node.DeclaringType;
                if (proceed.Contains(node)) return;
                if (node.IsGenericMethod && !node.IsGenericMethodDefinition)
                {
                    foreach (var t in node.GetGenericArguments())
                    {
                        if (!t.IsDefined(typeof(CompilerGeneratedAttribute)))
                        {
                            result.Add(t);
                        }
                    }
                }
                if (declaringType != null && Utils.shouldNotGetArgumentsInInstructions(node)) return;

                proceed.Add(node);

                var callingMethods = callingMethodsGetter(node);
                foreach (var callingMethod in callingMethods)
                {
                    GenericArgumentInInstructions(callingMethod, result, proceed, callingMethodsGetter);
                }
            }

            private static bool IterateAllValueType(Type type, List<ValueTypeInfo> list)
            {
                if (Utils.isDisallowedType(type)) return false;
                if (type.IsPrimitive)
                {
                    return true;
                }
                
                Type baseType = type.BaseType;
                while (baseType != null && baseType != typeof(System.Object))
                {
                    if (baseType.IsValueType)
                    {
                        if (!IterateAllValueType(baseType, list)) return false;
                    }
                    baseType = baseType.BaseType;
                }

                foreach (var field in type.GetFields(BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic))
                {
                    if (field.FieldType.IsValueType && !field.FieldType.IsPrimitive)
                        if (!IterateAllValueType(field.FieldType, list)) return false;
                }

                int value = -1;
                if (Nullable.GetUnderlyingType(type) != null)
                {
                    var fields = type.GetFields(BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
                    for (var i = 0; i < fields.Length; i++)
                    {
                        if (fields[i].Name == "hasValue" || fields[i].Name == "has_value")
                        {
                            value = i;
                            break;
                        }
                    }
                }

                list.Add(new ValueTypeInfo
                {
                    Signature = TypeUtils.GetTypeSignature(type),
                    CsName = type.Name,
                    FieldSignatures = WrapUtil.GetValueTypeFieldSignatures(type),
                    NullableHasValuePosition = value
                });
                return true;
            }

            public static void GenCPPWrapAll(){
                var types = from assembly in AppDomain.CurrentDomain.GetAssemblies()
                            // where assembly.FullName.Contains("Assembly-CSharp") || assembly.FullName.Contains("Unity")
                            where !assembly.FullName.Contains("Unity.Entities")
                            where !(assembly.ManifestModule is System.Reflection.Emit.ModuleBuilder)
                            from type in assembly.GetTypes()
                            where type.IsPublic 
                            where !(type.FullName.StartsWith("UnityEditor")
                                ||type.FullName.StartsWith("Unity.IO")
                                ||type.FullName.StartsWith("UnityEngine.iOS")
                                ||type.FullName.StartsWith("UnityEngine.Experimental")
                                ||type.FullName.StartsWith("UnityEngine.UIElements")
                                ||type.FullName.StartsWith("UnityEngine.Networking")
                                ||type.FullName.StartsWith("UnityEngine.XR")
                                ||type.FullName.StartsWith("UnityEngine.TestTools.")
                                ||type.FullName.StartsWith("XLua.CSObjectWrap")
                                ||type.FullName.StartsWith("Unity.Entities")
                                ||type.FullName.StartsWith("Unity.Collections")
                                ||type.FullName.StartsWith("Unity.Jobs")
                                ||type.FullName.StartsWith("UnityEngine.AI")
                                ||type.FullName.StartsWith("Unity.Build")
                                ||type.FullName.StartsWith("Unity.Burst")
                                ||type.FullName.StartsWith("Unity.Particle")
                                ||type.FullName.StartsWith("Unity.Serialization")
                                ||type.FullName.StartsWith("UnityEngine.Rendering")
                                ||type.FullName.StartsWith("UnityEngine.Assertions")
                                ||type.FullName.StartsWith("Mono")
                                ||type.FullName.StartsWith("Microsoft")
                                ||type.FullName.StartsWith("ICSharpCode")
                                ||type.FullName.StartsWith("TMPro")
                                ||type.FullName.StartsWith("JetBrains")
                                ||type.FullName.StartsWith("ExCSS")
                                ||type.FullName.StartsWith("NUnit")
                                ||type.FullName.StartsWith("Bee")
                                ||type.FullName.StartsWith("Newtonsoft")
                                ||type.FullName.StartsWith("Diagnostics")
                                ||type.FullName.StartsWith("Mono.Cecil"))
                                ||type.FullName.StartsWith("System.Data.")
                                ||type.FullName.StartsWith("System.Linq.")
                                ||type.FullName.StartsWith("System.Dynamic.")
                                ||type.FullName.StartsWith("System.Management.")
                                ||type.FullName.StartsWith("System.Text.")
                                ||type.FullName.StartsWith("System.Configuration.")
                                ||type.FullName.StartsWith("System.ComponentModel.")
                                ||type.FullName.StartsWith("System.EnterpriseServices.")
                                ||type.FullName.StartsWith("System.Xml.")
                                ||type.FullName.StartsWith("System.CodeDom.")
                                ||type.FullName.StartsWith("System.Diagnostics.")
                                ||type.FullName.StartsWith("System.Linq.")
                                ||type.FullName.StartsWith("System.Management.")
                                ||type.FullName.StartsWith("System.Diagnostics.")
                                ||type.FullName.StartsWith("System.Dynamic.")
                                ||type.FullName.StartsWith("System.Threading.")
                                ||type.FullName.StartsWith("System.Globalization.")
                                ||type.FullName.StartsWith("System.Security.")
                                ||type.FullName.StartsWith("System.ComponentModel.")
                                ||type.FullName.StartsWith("System.Diagnostics.")
                                ||type.FullName.StartsWith("System.Runtime.Remoting")
                                ||type.FullName.StartsWith("System.Runtime.CompilerServices")
                                ||type.FullName.StartsWith("System.Runtime.InteropServices")
                                ||type.FullName.StartsWith("System.Runtime.Serialization")
                                ||type.FullName.StartsWith("System.Net.")
                                ||type.FullName.StartsWith("System.CodeDom.")
                                ||type.FullName.StartsWith("System.Configuration.")
                                ||type.FullName.StartsWith("System.Transactions.")
                            select type;
                string filePath = "Assets/Plugins/xlua_il2cpp/FunctionBridge.Gen.h";
                GenCPPWrap(types, filePath);
            }

            public static void GenCPPWrap(IEnumerable<Type> types, string saveTo)
            {
                

                const BindingFlags flag = BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public;
                
                
                var typeExcludeDelegate = types.Where(t => !typeof(MulticastDelegate).IsAssignableFrom(t));
                
                var ctorToWrapper = typeExcludeDelegate
                    .SelectMany(t => t.GetConstructors(flag));
                
                var methodToWrap = typeExcludeDelegate
                    .SelectMany(t => t.GetMethods(flag));
                // File.WriteAllText(Application.dataPath+"/../types.txt", string.Join("\n", types.Select(t=>t.FullName)));
                // File.WriteAllText(Application.dataPath+"/../methods.txt", string.Join("\n", methodToWrap.Select(m=>$"{m.DeclaringType.FullName}.{m.Name}")));
                var fieldToWrapper = typeExcludeDelegate
                    .SelectMany(t => t.GetFields(flag));

                var wrapperUsedTypes = types
                    .Concat(ctorToWrapper.SelectMany(c => c.GetParameters()).Select(pi => GetUnrefParameterType(pi)))
                    .Concat(methodToWrap.SelectMany(m => m.GetParameters()).Select(pi => GetUnrefParameterType(pi)))
                    .Concat(methodToWrap.Select(m => m.ReturnType))
                    .Concat(fieldToWrapper.Select(f => f.FieldType))
                    .Distinct();


                //#TODO@benp 泛型支持
                // HashSet<Type> typeInGenericArgument = new HashSet<Type>();
                // HashSet<MethodBase> processed = new HashSet<MethodBase>();
                // foreach (var method in methodToWrap)
                // {
                //     GenericArgumentInInstructions(method, typeInGenericArgument, processed, mb =>
                //     {
                //         try
                //         {
                //             if (mb.GetMethodBody() == null || mb.IsGenericMethodDefinition || mb.IsAbstract) {

                //             return new MethodBase[] { };
                //             }

                //             var body =  mb.GetMethodBody();
                            
                //             return new MethodBase[] { };
                //             // mb.GetMethodBody().
                //             // return mb.GetInstructions()
                //             // .Select(i => i.Operand)
                //             // .Where(o => o is MethodBase)
                //             // .Cast<MethodBase>();
                //         }
                //         catch (Exception e)
                //         {
                //             UnityEngine.Debug.LogWarning(string.Format("get instructions of {0} ({2}:{3}) throw {1}", mb, e.Message, mb.DeclaringType == null ? "" : mb.DeclaringType.Assembly.GetName().Name, mb.DeclaringType));
                //             return new MethodBase[] { };
                //         }
                //     });
                // }

                var delegateToBridge = wrapperUsedTypes
                    // .Concat(typeInGenericArgument)
                    .Where(t => typeof(MulticastDelegate).IsAssignableFrom(t));

                var delegateInvokes = delegateToBridge
                    .Select(t => t.GetMethod("Invoke"))
                    .Where(m => m != null);

                var delegateUsedTypes = delegateInvokes.SelectMany(m => m.GetParameters()).Select(pi => GetUnrefParameterType(pi))
                    .Concat(delegateInvokes.Select(m => m.ReturnType));

                var valueTypeInfos = new List<ValueTypeInfo>();
                foreach (var type in wrapperUsedTypes.Concat(delegateUsedTypes))
                {
                    IterateAllValueType(type, valueTypeInfos);
                }
                valueTypeInfos = valueTypeInfos
                    .GroupBy(s => s.Signature)
                    .Select(s => s.FirstOrDefault())
                    .ToList();

                var bridgeInfos = delegateInvokes
                    .Select(m => new SignatureInfo
                    {
                        Signature = TypeUtils.GetMethodSignature(m, true),
                        CsName = m.ToString(),
                        ReturnSignature = TypeUtils.GetTypeSignature(m.ReturnType),
                        ThisSignature = null,
                        ParameterSignatures = m.GetParameters().Select(p => TypeUtils.GetParameterSignature(p)).ToList()
                    })
                    .GroupBy(s => s.Signature)
                    .Select(s => s.FirstOrDefault())
                    .ToList();
                bridgeInfos.Sort((x, y) => string.CompareOrdinal(x.Signature, y.Signature));

                var genWrapperCtor = ctorToWrapper;
                var genWrapperMethod = methodToWrap;
                var genWrapperField = fieldToWrapper;
                var wrapperInfos = genWrapperMethod
                    .Where(m => !m.IsGenericMethodDefinition && !m.IsAbstract)
                    .Select(m =>
                    {
                        
                        var isExtensionMethod = m.IsDefined(typeof(ExtensionAttribute));
                        return new SignatureInfo
                        {
                            Signature = TypeUtils.GetMethodSignature(m, false, isExtensionMethod),
                            CsName = m.ToString(),
                            ReturnSignature = TypeUtils.GetTypeSignature(m.ReturnType),
                            ThisSignature = TypeUtils.GetThisSignature(m, isExtensionMethod),
                            ParameterSignatures = m.GetParameters().Skip(isExtensionMethod ? 1 : 0).Select(p => TypeUtils.GetParameterSignature(p)).ToList()
                        };
                    })
                    .Concat(
                        genWrapperCtor
                            .Select(m =>
                            {
                                
                                File.AppendAllText(Application.dataPath+"/../methods.txt", m.DeclaringType.ToString() + "|" +m.ToString() + "\n");
                                return new SignatureInfo
                                {
                                    Signature = TypeUtils.GetMethodSignature(m),
                                    CsName = m.ToString(),
                                    ReturnSignature = "v",
                                    ThisSignature = TypeUtils.GetThisSignature(m),
                                    ParameterSignatures = m.GetParameters().Skip(0).Select(p => TypeUtils.GetParameterSignature(p)).ToList()
                                };
                            })
                    )
                    .GroupBy(s => s.Signature)
                    .Select(s => s.FirstOrDefault())
                    .ToList();
                wrapperInfos.Sort((x, y) => string.CompareOrdinal(x.Signature, y.Signature));

                var fieldWrapperInfos = genWrapperField
                    .Select(f => new SignatureInfo
                    {
                        Signature = (f.IsStatic ? "" : TypeUtils.GetThisSignature2(f.DeclaringType)) + TypeUtils.GetTypeSignature(f.FieldType),
                        CsName = f.ToString(),
                        ReturnSignature = TypeUtils.GetTypeSignature(f.FieldType),
                        ThisSignature = f.IsStatic ? "" : TypeUtils.GetThisSignature2(f.DeclaringType),
                        ParameterSignatures = null
                    })
                    .GroupBy(s => s.Signature)
                    .Select(s => s.FirstOrDefault())
                    .ToList();
                fieldWrapperInfos.Sort((x, y) => string.CompareOrdinal(x.Signature, y.Signature));
                
                string content = CppWrapper.Gen(new CppWrappersInfo
                {
                    ValueTypeInfos = valueTypeInfos,
                    WrapperInfos = wrapperInfos,
                    BridgeInfos = bridgeInfos,
                    FieldWrapperInfos = fieldWrapperInfos
                });
                using (StreamWriter textWriter = new StreamWriter(saveTo)){
                    textWriter.Write(content);
                    textWriter.Flush();
                }

                Debug.LogWarning($"build success 处理类数量 {types.Count()} 处理接口数量 {methodToWrap.Count()} 压缩为{wrapperInfos.Count} 处理field数量{genWrapperField.Count()} 压缩为{fieldWrapperInfos.Count} 处理delegate数量{delegateInvokes.Count()} 压缩为{bridgeInfos.Count()}");
            }

            public static void GenExtensionMethodInfos(string outDir)
            {
                //                 var configure = Puerts.Configure.GetConfigureByTags(new List<string>() {
                //                     "Puerts.BindingAttribute",
                //                 });

                //                 var genTypes = new HashSet<Type>(configure["Puerts.BindingAttribute"].Select(kv => kv.Key)
                //                     .Where(o => o is Type)
                //                     .Cast<Type>()
                //                     .Where(t => !t.IsGenericTypeDefinition && !t.Name.StartsWith("<"))
                //                     .Distinct()
                //                     .ToList());

                //                 genTypes.Add(typeof(PuertsIl2cpp.ArrayExtension));
                //                 // genTypes.Add(typeof(PuertsIl2cpp.ArrayExtension2));
                //                 var extendedType2extensionType = (from type in genTypes
                // #if UNITY_EDITOR
                //                     where !System.IO.Path.GetFileName(type.Assembly.Location).Contains("Editor")
                // #endif
                //                                                   from method in type.GetMethods(BindingFlags.Static | BindingFlags.Public).Select(method => TypeUtils.HandleMaybeGenericMethod(method)).Where(method => method != null)
                //                                                   where Utils.isDefined(method, typeof(ExtensionAttribute))
                //                                                   group type by Utils.getExtendedType(method)).ToDictionary(g => g.Key, g => (g as IEnumerable<Type>).Distinct().ToList()).ToList();

                // using (var jsEnv = new Puerts.JsEnv())
                // {
                //     var wrapRender = jsEnv.ExecuteModule<Func<List<KeyValuePair<Type, List<Type>>>, string>>(
                //         "puerts/templates/extension_methods_gen.tpl.mjs", "default");
                //     string fileContent = wrapRender(extendedType2extensionType);
                //     var filePath = outDir + "ExtensionMethodInfos_Gen.cs";
                //     using (StreamWriter textWriter = new StreamWriter(filePath, false, Encoding.UTF8))
                //     {
                //         textWriter.Write(fileContent);
                //         textWriter.Flush();
                //     }
                // }
            }

            public static void GenLinkXml(string outDir)
            {
                // var configure = Puerts.Configure.GetConfigureByTags(new List<string>() {
                //         "Puerts.BindingAttribute",
                //     });
                // var genTypes = configure["Puerts.BindingAttribute"].Select(kv => kv.Key)
                //     .Where(o => o is Type)
                //     .Cast<Type>()
                //     .Where(t => !t.IsGenericTypeDefinition && !t.Name.StartsWith("<"))
                //     .Distinct()
                //     .ToList();

                //#TODO@benp gen link
                // using (var jsEnv = new Puerts.JsEnv())
                // {
                //     var linkXMLRender = jsEnv.ExecuteModule<Func<List<Type>, string>>("puerts/templates/linkxmlgen.tpl.mjs", "LinkXMLTemplate");
                //     string linkXMLContent = linkXMLRender(genTypes);
                //     var linkXMLPath = outDir + "link.xml";
                //     using (StreamWriter textWriter = new StreamWriter(linkXMLPath, false, Encoding.UTF8))
                //     {
                //         textWriter.Write(linkXMLContent);
                //         textWriter.Flush();
                //     }
                // }
            }

            public static void GenMarcoHeader(string outDir)
            {
                //                 var filePath = outDir + "unityenv_for_puerts.h";

                //                 using (var jsEnv = new Puerts.JsEnv())
                //                 {
                //                     var macroHeaderRender = jsEnv.ExecuteModule<Func<bool, bool, string>>("puerts/xil2cpp/unityenv_for_puerts.h.tpl.mjs", "default");
                //                     string macroHeaderContent = macroHeaderRender(              
                // #if !UNITY_2021_1_OR_NEWER
                //                         false,
                // #else
                //                         true,
                // #endif
                // #if UNITY_ANDROID || UNITY_IPHONE
                //                         false
                // #else
                //                         true
                // #endif
                //                     );

                //                     using (StreamWriter textWriter = new StreamWriter(filePath, false, Encoding.UTF8))
                //                     {
                //                         textWriter.Write(macroHeaderContent);
                //                         textWriter.Flush();
                //                     }
                //                 }
            }
        }
    }
}