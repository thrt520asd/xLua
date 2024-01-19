# xlua il2cpp 改造
参考https://km.woa.com/articles/show/563564
背景
1 编辑器（mono） 采用aov的methodWarp方案
2 运行时（il2cpp） 采用生成代码方案
InternalCall
这项技术主要用于在原生侧实现一个C#函数。我们试试利用这项技术实现在il2cpp环境下，C++调用上述C# Add函数。

## PuerTS拆解
生成warp代码FunctionBridge.Gen.h
PuerTs.cpp 引用来管理
C#侧的TypeReigster来处理逻辑(RegisterNoThrow)
JsEnv中将RegisterNoThrow注册到C++中

### 编译依赖
puerTs目录划分
native_src_il2cpp 编译到dll的文件{Unity和native的胶水层
	#include "UnityExports4Puerts.h"
	#include "FunctionBridge.Gen.h"
}

Plugins\puerts_il2cpp{ 导出Unity接口
	UnityExports4Puerts.h
	unityenv_for_puerts.h
	Puerts_il2cpp.h 封装了对il2cpp的操作 只有这里可以引用il2cpp和Unity
}
Gen.h依赖il2cpp和Lua
Lua依赖UnityExprot4Xlua.h

### 引用持有
FCppObjectMapper::FindOrAddCppObject
FCppObjectMapper::BindCppObject
FCppObjectMapper::UnBindCppObject

### Register的具体逻辑
Native侧，发起注册  TypePointer
C#侧 
TypePointer->Type
CreateCSharpTypeInfo
AddConstructor
SetUsedTypeInfo
AddMethodToType
AddExtensionMethodToType
AddPropertiesToType
AddFieldToType
RegisterCSharpType
RegisterResultCheck
--methodInfoPointer &  methodPointer
如何持有C++构造的引用呢

同样的 
Lua
Lua测发起注册
ImportType

MethodCallback回调发起


## xlua拆解
TryDelayWrapLoader 
	--delayWrap 导入C# Class Wrap方案
	--ReflectionWrap 反射方案   
		todo 参考dybridge的方案是实现一个新的OverloadMethodWrap
		构建C++ WrapData 执行方法
		持有C++的方法指针
		这块完全放在C++来调用

pushUserData 到Lua


### Xlua反射实现详解

OverloadMethodWrap 每个重载函数的函数实现
 
MethodWrap 管理方法下的所有重载函数

MethodWrapsCache 持有ObjectTranslator  主要函数 _GenMethodWrap产出MethodWrap

ObjectTranslator{
	GetTypeId()->getTypeId() WrapClass并返回TypeId
	{
		makeReflectionWrap 搜集所有函数 _GenMethodWrap
	}
}
lua 侧xlua.private_accessible 访问私有方法
TryDelayWrapLoader 生成WrapClass的实现 todo这里替换为il2cpp的实现
LazyReflectionCall LazyLoad 猜测是出于省内存考量，il2cpp不需要

RegisterFunc 注册函数到clsMetatable，lua_pushstdcallcfunction直接注册函数指针
RegisterLazyFunc 注册函数名和函数信息以及LazyReflectionWrap到ClsMetatable，按需加载
PushFixCSFunction C#侧List持有函数引用，Lua通过提供ListIndex来调用C#函数

### XLua结构
clsMetatable = {

}
clsTable{
	__call = constructor
	//upvalue --- [1]:getters, [2]:feilds, [3]:base, [4]:indexfuncs, [5]:baseindex
	//param   --- [1]: obj, [2]: key
	__index = cls_indexer
	//upvalue --- [1]:setters, [2]:base, [3]:indexfuncs, [4]:baseindex
	//param   --- [1]: obj, [2]: key, [3]: value
	__newIndex = cls_newindexer
	staticMethod .....
	staticProperty .....
}

objMetaTable{
	//upvalue --- [1]: methods, [2]:getters, [3]:csindexer, [4]:base, [5]:indexfuncs, [6]:arrayindexer, [7]:baseindex
	//param   --- [1]: obj, [2]: key
	__index = obj_indexer
	//upvalue --- [1]:setters, [2]:csnewindexer, [3]:base, [4]:newindexfuncs, [5]:arrayindexer, [6]:basenewindex
	//param   --- [1]: obj, [2]: key, [3]: value
	__newIndex = obj_newindexer
}


objMetaTable , cache in gloablMetaTable[clsFullName] cache in LUA_REGISTRYINDEX[uniqId]
clsTable cache in CS[FullName]




## 单元测试特
----------------结构类型----------------

class
struct
array
delegate
Enum

----------------结构成员----------------

构造函数
成员函数
静态函数
属性get/set
字段
静态字段
静态属性

----------------函数特性----------------

函数重载
函数默认参数
泛型函数

----------------HotFix----------------

## RoadMap
1 实现一个IL2CPP的类型，打通lua和IL2CPP交互
2 跑通所有单元测试
3 实现生成代码
4 性能测试
5 更完善的单元测试
6 尝试接入到AOE
7 AOE内性能测试

## 核心代码
结构
lua ClsMetaTable{
	typeId,
	*luaClassInfo,
	__index,
	__newindex,
}

与C#侧实现对应的，C++传递指针到Lua

结构
objMetaTable{
	//upvalue --- [1]: methods, [2]:getters, [3]:csindexer, [4]:base, [5]:indexfuncs, [6]:arrayindexer, [7]:baseindex
	//param   --- [1]: obj, [2]: key
	__index = cppStaticIndexer
	//upvalue --- [1]:setters, [2]:csnewindexer, [3]:base, [4]:newindexfuncs, [5]:arrayindexer, [6]:basenewindex
	//param   --- [1]: obj, [2]: key, [3]: value
	__newIndex = cppStaticNewInder
}
clsTable{
	*luaClassInfo,
}
objUserData = {csObjPointer, typeId}
====================================核心代码END====================================
需要锁吗，不需要 对lua的访问有C#的锁来保证
lua中所有的对象都是table，使用table来桥接C#的行为

