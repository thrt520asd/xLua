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
        // IntPtr ptr2 = GenIL2CPPTest(true);
        // GC.Collect();
        // Debug.Log(ptr2);
        
        // IntPtr ptr = GenIL2CPPTest(false);
        // GC.Collect();
        // Debug.Log(ptr);
    }

}

public class IL2CPPTest 
{
    // public IL2CPPTest(string name) : base(name)
    // {
    // }

    // public IL2CPPTest(string name) : base(name)
    // {
    // }
    public int a  = 1;
    public void Print(string str)
    {
        Debug.Log("IL2CPPTest"+str);
    }


    public void Prin_Int(int i)
    {
        Debug.Log("IL2CPPTest"+i);
    }
}


public class TestEntry{

    public void Test(){
        // IL2CPPTest test = new IL2CPPTest("111");
    }

}