#include "unordered_map"

typedef int32_t(*AddObjFunc)(lua_State* L, Il2CppObject* obj, void* method);
typedef Il2CppObject* (*RemoveObjFunc)(lua_State* L, int32_t objIndex, void* method);

class CppObjMapper
{
private:
    std::unordered_map<void*, int32_t> objCache;
    std::unordered_map<void*, int32_t> ilclass2luaMetaId;
    int32_t cacheRef;
    AddObjFunc addObjFunc;
    RemoveObjFunc removeObjFunc;
    Il2CppReflectionMethod* addObjReflectionMethod;
    Il2CppReflectionMethod* removeObjReflectionMethod;
public:
    CppObjMapper(/* args */);
    ~CppObjMapper();
    void SetTypeId(void *kclass, int32_t metaId);
    int GetClassMetaId(void *kclass);
    bool TryPushObject(lua_State*L, void *obj);
    void FreeObj(Il2CppObject* obj);
    void SetCacheRef(int32_t cache_ref);
    void InitObjPoolMethod(Il2CppReflectionMethod* addObjMethod, Il2CppMethodPointer* addObjMethodPointer, Il2CppReflectionMethod* removeObjMethod, Il2CppMethodPointer* removeObjMethodPointer);

    int AddToPool(lua_State* L,Il2CppObject* obj);

    Il2CppObject* RemoveFromPool(lua_State* L, int index);

    void *ToCppObj(lua_State *L, int index);

    void* ToCppObj_Field(lua_State* L, int index);

    bool TryPushStruct(lua_State *L, void *typeId, void *pointer, unsigned int size);
};

CppObjMapper* GetCppObjMapper();
