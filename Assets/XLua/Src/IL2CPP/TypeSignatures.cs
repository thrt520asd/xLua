namespace XLua.IL2CPP{
    public class TypeSignatures
    {
        public static string Void = "v";
        public static string Bool = "b";
        public static string Byte = "u1";
        public static string Sbyte = "i1";
        public static string Short = "i2";
        public static string Ushort = "u2";
        public static string Int = "i4";
        public static string Uint = "u4";
        public static string Long = "i8";
        public static string Ulong = "u8";
        public static string Char = "c";
        public static string Double = "r8";
        public static string Float = "r4";
        public static string IntPtr = "p";
        public static string String = "s";
        public static string ArrayBuffer = "a";
        public static string SystemObject = "O";
        public static string RefOrPointerPrefix = "P";
        public static string Object = "o";
        public static string StructPrefix = "S_";
        public static string NullableStructPrefix = "N_";
        public static string ArrayPrefix = "V";
        public static string OptionalParmaeterPrefix = "D";
        public static string OutParameterPrefix = "U";

        /// <summary>
        /// 成员函数标记 T表示Object 引用类型直接拿指针 值类型需要装箱
        /// </summary>
        public static string ThisSignature_OBJECT = "T";
        /// <summary>
        /// 成员函数标记 s表示值类型 裸指针
        /// </summary>
        public static string ThisSignature_Value = "k";
        /// <summary>
        /// 成员函数标记 s表示值类型 需要指针左移一个IL2CPPObject 
        /// 调用值类型的成员函数，属性需要调用
        /// </summary>
        public static string ThisSignature_Value_Offset = "K";
        /// <summary>
        /// 成员函数标记 普通引用类型
        /// </summary>
        public static string ThisSignature_Ref = "t";

    }
}