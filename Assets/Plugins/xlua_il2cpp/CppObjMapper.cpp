#pragma once
#include "unordered_map"
#include <il2cpp-api-types.h>
#include "lua_api_adapt.h"
#include "xlua_il2cpp_def.h"
#include "CppObjMapper.h"
#include "LuaClassRegister.h"
#include <vm/Exception.h>
#include "Il2CppTools.h"
#include "vm/Object.h"
#include <vm/Reflection.h>
using namespace il2cpp::vm;
namespace xlua
{
	CSharpGetTypeIdFunc cSharpGetTypeMethodPtr = nullptr;
	void* cSharpGetTypeMethod = nullptr;

	CppObjMapper::CppObjMapper(/* args */)
	{

	}

	CppObjMapper::~CppObjMapper()
	{
	}

	static int xluaTag = 1;
	static int xluaStructTag = 2;
	static Il2CppReflectionMethod* addObjReflectionMethod;
	static Il2CppReflectionMethod* removeObjReflectionMethod;
	static AddObjFunc addObjFunc;
	static RemoveObjFunc removeObjFunc;

	void CppObjMapper::SetCacheRef(int32_t cache_ref)
	{
		cacheRef = cache_ref;
	}

	void CppObjMapper::InitObjPoolMethod(Il2CppReflectionMethod* addObjMethod, Il2CppMethodPointer* addObjMethodPointer, Il2CppReflectionMethod* removeObjMethod, Il2CppMethodPointer* removeObjMethodPointer)
	{
		addObjFunc = (AddObjFunc)addObjMethodPointer;
		addObjReflectionMethod = addObjMethod;
		removeObjFunc = (RemoveObjFunc)removeObjMethodPointer;
		removeObjReflectionMethod = removeObjMethod;
	}

	int32_t CppObjMapper::AddToPool(lua_State* L, Il2CppObject* obj)
	{
		int32_t idx = addObjFunc(L, obj, addObjReflectionMethod);
		return idx;
	}

	Il2CppObject* CppObjMapper::RemoveFromPool(lua_State* L, int index)
	{
		auto obj = removeObjFunc(L, index, removeObjReflectionMethod);
		return obj;
	}


	void* CppObjMapper::ToCppUserData(lua_State* L, int index, int* type)
	{
		auto pointer = lapi_lua_touserdata(L, index);
		if (pointer) {

			int* tagPtr = *static_cast<int**>(pointer);
			if (tagPtr == &xluaTag) {
				*type = XLUA_UDTYPE_OBJ;
				return pointer;
			}
			else if (tagPtr == &xluaStructTag) {
				*type = XLUA_UDTYPE_STRUCT;
				return pointer;
			}
		}
		*type = XLUA_UDTYPE_NONE;
		return nullptr;
	}

	/// 返回引用类型的对象
	void* CppObjMapper::ToCppObj(lua_State* L, int index)
	{
		auto pointer = lapi_lua_touserdata(L, index);
		int* tagPtr = *static_cast<int**>(pointer);
		if (tagPtr == &xluaTag) {
			//obj
			ObjUD* objUD = (ObjUD*)pointer;
			return objUD->pointer;
		}
		else if (tagPtr == &xluaStructTag) {
			//struct
			//结构体类型需要左移一个Il2CppObject指针大小
			StructUD* structUD = (StructUD*)pointer;
			char* data = &structUD->data[0];
			data -= sizeof(RuntimeObject);
			return data;
		}
		return nullptr;
	}


	StructUD* CppObjMapper::CreateStruct(lua_State* L, Il2CppClass* klass, unsigned int size) {
		int32_t metaId = GetTypeIdByIl2cppClass(L, (Il2CppClass*)klass);
		if (metaId != -1) {
			size_t udSize = size + sizeof(unsigned int) + sizeof(void*) + sizeof(int*);
			StructUD* sud = (StructUD*)lapi_lua_newuserdata(L, udSize);
			sud->len = size;
			sud->typeId = klass;
			sud->tag = &xluaStructTag;
			memset(&sud->data[0], 0, size);
			lapi_xlua_rawgeti(L, lapi_lapi_lua_upvalueindex(0), metaId);
			lapi_lua_setmetatable(L, -2);
			return sud;
		}
		else {
			return nullptr;
		}
	}

	StructUD* CppObjMapper::ToStructUD(lua_State* L, int index) {
		void* p = lapi_lua_touserdata(L, index);
		if (p) {
			int* tagPtr = *static_cast<int**>(p);
			if (tagPtr == &xluaStructTag) {
				return  (StructUD*)p;
			}
		}
		return nullptr;
	}

	void* CppObjMapper::ToStruct(lua_State* L, int index, Il2CppClass* klass) {
		int type = 0;
		void* ptr = ToCppUserData(L, index, &type);
		if (type == XLUA_UDTYPE_STRUCT) {
			StructUD* sud = (StructUD*)ptr;
			if (sud->typeId == klass) {
				return &((StructUD*)ptr)->data[0];
			}
			else {
				throw_exception2lua_format(L, "struct type error [%s]", klass->name);
			}
		}
		else if (type == XLUA_UDTYPE_OBJ) {
			ObjUD* objUd = (ObjUD*)ptr;
			Il2CppClass* objKlass = ((Il2CppObject*)objUd->pointer)->klass;
			if (objKlass == klass) {
				return Object::Unbox((Il2CppObject*)objUd->pointer);
			}
			else {
				throw_exception2lua_format(L, "struct type error [%s]", klass->name);
			}
		}

		if (klass && lapi_lua_type(L, index) == LUA_TTABLE) {
			auto reflectObj = FallBackLua2CSObj(L, index, klass);
			if (reflectObj) {
				return Object::Unbox(reflectObj);
			}
		}

		return nullptr;
	}

	bool CppObjMapper::TryPushStruct(lua_State* L, Il2CppClass* klass, void* pointer, unsigned int size)
	{
		if (!klass->has_references)
		{
			StructUD* sud = CreateStruct(L, klass, size);
			memcpy(&sud->data[0], pointer, size);
			return sud;
		}
		else
		{
			Il2CppObject* obj = il2cpp::vm::Object::Box(klass, pointer);
			CppObjMapper::TryPushObject(L, obj);
		}

		return false;
	}


	bool CppObjMapper::TryPushObject(lua_State* L, Il2CppObject* obj)
	{
		if (!obj)
		{
			lapi_lua_pushnil(L);
			return true;
		}
		auto iter = objCache.find(obj);

		int32_t poolIdx = 0;
		if (iter != objCache.end())
		{
			// 已经有了一份了
			poolIdx = iter->second;
			if (lapi_xlua_tryget_cachedud(L, poolIdx, cacheRef) == 1)
			{
				return true;
			}
			else
			{
				// lua中的引用清理掉  C#没有清理掉
			}
		}
		else
		{
			poolIdx = AddToPool(L, obj);
		}

		int32_t metaId = GetTypeIdByIl2cppClass(L, obj->klass);
		if (metaId != -1)
		{
			objCache[obj] = poolIdx;
			ObjUD* objUd = (ObjUD*)lapi_lua_newuserdata(L, sizeof(ObjUD));
			objUd->pointer = obj;
			objUd->tag = &xluaTag;
			lapi_xlua_rawgeti(L, lapi_xlua_get_registry_index(), cacheRef);
			lapi_lua_pushvalue(L, -2);
			lapi_xlua_rawseti(L, -2, poolIdx);
			lapi_lua_pop(L, 1);
			// // xlua::GWarnFormatted("cacheud2 %p", L);
			lapi_xlua_rawgeti(L, lapi_xlua_get_registry_index(), metaId);
			// // xlua::GWarnFormatted("cacheud3 %p", L);
			lapi_lua_setmetatable(L, -2);
		}
		else
		{
			
            Exception::Raise(Exception::GetInvalidOperationException("meta id invalid"));
            return false;
		}

		return true;
	}

	// clean C# ref
	void CppObjMapper::FreeObj(lua_State* L, int idx)
	{
		Il2CppObject* obj = (Il2CppObject*)ToCppObj(L, idx);
		if (obj)
		{
			auto iter = objCache.find(obj);
			if (iter != objCache.end())
			{
				int32_t poolIdx = iter->second;
				RemoveFromPool(L, poolIdx);
				objCache.erase(iter);
			}
			else
			{
				GWarnFormatted("obj not found in obj cache");
				objCache.erase(iter);
			}
		}
	}

	Il2CppObject* CppObjMapper::GetCSharpObj(lua_State* L, int idx)
	{
		ObjUD* obj = (ObjUD*)lapi_lua_touserdata(L, idx);
		if (obj && obj->tag == &xluaTag)
		{
			return (Il2CppObject*)obj->pointer;
		}
		return nullptr;
	}

    
    int CppObjMapper::CSharpGetTypeId(lua_State *L, Il2CppReflectionType* reflectionType) {
        if (cSharpGetTypeMethodPtr) {
             int typeId = cSharpGetTypeMethodPtr(L, reflectionType, cSharpGetTypeMethod);
             return typeId;
        }
        else {
            xlua::GLogFormatted("[error] cSharpGetTypeMethodPtr hasn't register");
        }
        return -1;
    }

    void CppObjMapper::SetTypeId(void* kclass, int32_t metaId){
        auto result = ilclass2luaMetaId.insert({kclass, metaId});
        if(result.second){
            // xlua::GLogFormatted("set type id insert success %p $d", kclass, metaId);
        }else{
            // xlua::GLogFormatted("set type id insert success %p $d", kclass, metaId);

        }
    }

    int32_t CppObjMapper::GetClassMetaId(void* kclass){
        auto iter = ilclass2luaMetaId.find(kclass);
        if(iter != ilclass2luaMetaId.end()){
            return iter->second;
        }
        return -1;
    }

    int32_t CppObjMapper::GetTypeIdByIl2cppClass(lua_State *L, const Il2CppClass *klass){   
        auto metaId = GetClassMetaId((void*)klass);
        if(metaId == -1){
            auto reflectType = il2cpp::vm::Reflection::GetTypeObject(&klass->byval_arg);
            if(reflectType){
                return CSharpGetTypeId(L, reflectType);
            }
        }
        return metaId;
    }

	void CppObjMapper::SetGetTypeIdFuncPtr(CSharpGetTypeIdFunc methodPtr, void* method) {
		cSharpGetTypeMethodPtr = methodPtr;
		cSharpGetTypeMethod = method;
	}

	CppObjMapper* GetCppObjMapper()
	{
		static CppObjMapper s_cppObjMapper;
		return &s_cppObjMapper;
	}
}