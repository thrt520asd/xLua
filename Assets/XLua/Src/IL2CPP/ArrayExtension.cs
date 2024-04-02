using System;

namespace XLua.IL2CPP
{
    [LuaCallCSharpAttribute]
    public static class ArrayExtension
    {
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static int get_Item(this int[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static float get_Item(this float[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static double get_Item(this double[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static bool get_Item(this bool[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static long get_Item(this long[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static ulong get_Item(this ulong[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static sbyte get_Item(this sbyte[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static byte get_Item(this byte[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static ushort get_Item(this ushort[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static short get_Item(this short[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static char get_Item(this char[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static uint get_Item(this uint[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static string get_Item(this string[] arr, int idx) { return arr[idx]; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static object get_Item(this System.Array arr, int idx) { return arr.GetValue(idx); }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this int[] arr, int idx, int val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this float[] arr, int idx, float val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this double[] arr, int idx, double val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this bool[] arr, int idx, bool val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this long[] arr, int idx, long val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this ulong[] arr, int idx, ulong val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this sbyte[] arr, int idx, sbyte val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this byte[] arr, int idx, byte val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this ushort[] arr, int idx, ushort val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this short[] arr, int idx, short val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this char[] arr, int idx, char val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this uint[] arr, int idx, uint val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this string[] arr, int idx, string val) { arr[idx] = val; }
        [UnityEngine.Scripting.Preserve] [LuaCallCSharpAttribute] public static void set_Item(this System.Array arr, int idx, object val) 
        { 
            
            arr.SetValue(val, idx); 
        }
    }
}
