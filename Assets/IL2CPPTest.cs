using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using XLua;

public class IL2CPPTestBase{
    private string m_name = "";
    public IL2CPPTestBase(string name){
        Debug.Log("IL2CPPTestBase constructor "+ name);
        m_name = name;
    }

    public IL2CPPTestBase(){

    }


    public void InheritTest(){
        Debug.Log("InheritTest");
    }
    
    public int baseInt = 1;

}

[LuaCallCSharp]
public class IL2CPPTest :IL2CPPTestBase
{
    public Il2CppTestStruct il2CppTestStruct;

    protected virtual int InheirtInt {
        get;set;
    }
    public int fieldInt  = 1;
    private int privateInt = 1;
    public static int staticInt = 1;

    private int propertyInt = 1; 

    public int PropertyInt {
        get { Debug.Log("propertyInt get "); return propertyInt;}
        set { Debug.Log("propertyInt set "); propertyInt = value;}
    }
    private static int staticPropertyInt = 1; 

    public static int StaticPropertyInt {
        get { Debug.Log("staticPropertyInt get "); return staticPropertyInt;}
        set { Debug.Log("staticPropertyInt set "); staticPropertyInt = value;}
    }

    private int[] arr = new int[1024];
    public int this[int index]{
        get{
            return arr[index];
        }
        set{
            arr[index] = value;
        }
    }


    // public int Item{
    //     get{
    //         return 1;
    //     }
    // }

    private int ctorInt = 1;
    public IL2CPPTest(string name) : base(name)
    {
        
    }

    public IL2CPPTest(int n) : base("name")
    {
        ctorInt = n;
    }

    public IL2CPPTest() : base("NoArgument")
    {

    }

    ~IL2CPPTest(){
        Debug.Log("IL2CPPTest Dispose");
    }
    

    public void Print(string str)
    {
        Debug.Log("Print"+str);
    }

    public void Print(int n){
        Debug.Log("Print"+n);
    }
    
    public void MethodInvoke(){

    }

    public static void StaticPrint(string str)
    {
        Debug.Log("StaticPrint"+str);
    }

    public static void StaticPrint(int n){
        Debug.Log("StaticPrint"+n);
    }
    
}

public struct Il2CppTestStruct{
    public int field1;
    public int field2;

    public static int staticField;


    public Il2CppTestStruct(int n, int m){
        Debug.Log("Il2CppTestStruct constructor 1");
        field1 = n;
        field2 = m;
    }

    public Il2CppTestStruct(int n)
    {
        Debug.Log("Il2CppTestStruct constructor 2");
        field1 = n;
        field2 = -1;
    }

    public int AddField1(int n ){
        field1 += n;
        return field1;
    }

    public static Il2CppTestStruct StaticGetStruct(){
        return new Il2CppTestStruct(1);
    }

    public static int staticField1 = 0;
    public static int StaticPropertyInt{get;set;}
    
    public static Il2CppTestStruct StaticStruct;
}

