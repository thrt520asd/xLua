using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IL2CPPTestBase{
    public IL2CPPTestBase(string name){

    }

    public IL2CPPTestBase(){

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

    // public void Print(string str)
    // {
    //     Debug.Log("IL2CPPTest"+str);
    // }

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