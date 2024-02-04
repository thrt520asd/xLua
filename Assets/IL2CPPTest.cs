using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class IL2CPPTestBase{
    IL2CPPTest test = null;
    public IL2CPPTestBase(string name){

    }

    public IL2CPPTestBase(){

    }
    public class AnyClass {string name;}

    

    public void TestIL2CPPGC(){
        Debug.Log("test");
    }

}

public class IL2CPPTest :IL2CPPTestBase
{

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

    private int ctorInt = 1;
    public IL2CPPTest(string name) : base(name)
    {
    }

    public IL2CPPTest(int n) : base("name")
    {
        ctorInt = n;
    }

    

    public void Print(string str)
    {
        Debug.Log("Print"+str);
    }

    public void Print(int n){
        Debug.Log("Print"+n);
    }

    public static void StaticPrint(string str)
    {
        Debug.Log("StaticPrint"+str);
    }

    public static void StaticPrint(int n){
        Debug.Log("StaticPrint"+n);
    }
    
}


public class TestEntry{

    public void Test(){
        IL2CPPTest test = new IL2CPPTest(1);
        
        // IL2CPPTest test = new IL2CPPTest("111");
    }

}