#if !XLUA_GENERAL
using UnityEngine;
using System.Collections;
using XLua;

public class Main_il2cpp : MonoBehaviour {
	LuaEnv luaenv;
	// Use this for initialization
	void Start () {
		luaenv = LuaEnvSingleton.Instance;
		luaenv.DoString ("require 'Il2Cpp.main_il2cpp'");
	}
	
	// Update is called once per frame
	void Update () {
		luaenv.GC ();
	}
}
#endif