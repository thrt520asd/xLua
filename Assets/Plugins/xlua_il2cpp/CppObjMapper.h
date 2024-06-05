#include "unordered_map"

typedef int32_t (*AddObjFunc)(lua_State *L, Il2CppObject *obj, void *method);
typedef Il2CppObject *(*RemoveObjFunc)(lua_State *L, int32_t objIndex, void *method);
typedef bool (*RefObjFunc)(lua_State *L, Il2CppObject *obj, void *method);
typedef bool (*UnRefObjFunc)(lua_State *L, Il2CppObject *obj, void *method);
namespace xlua
{
    class CppObjMapper
    {
    private:
        std::unordered_map<void *, int32_t> objCache;
        std::unordered_map<void *, int32_t> ilclass2luaMetaId;
        int32_t cacheRef;

    public:
        CppObjMapper(/* args */);
        ~CppObjMapper();

        StructUD *ToStructUD(lua_State *L, int index);

        bool TryPushObject(lua_State *L, Il2CppObject *obj);
        void FreeObj(lua_State *L, int idx);
        Il2CppObject *GetCSharpObj(lua_State *L, int idx);
        void SetCacheRef(int32_t cache_ref);
        static void InitObjPoolMethod(Il2CppReflectionMethod *addObjMethod, Il2CppMethodPointer *addObjMethodPointer, Il2CppReflectionMethod *removeObjMethod, Il2CppMethodPointer *removeObjMethodPointer);

        static int AddToPool(lua_State *L, Il2CppObject *obj);
        static Il2CppObject *RemoveFromPool(lua_State *L, int index);

        void *ToCppUserData(lua_State *L, int index, int *type);

        void *ToCppObj(lua_State *L, int index);

        StructUD *CreateStruct(lua_State *L, Il2CppClass * klass, unsigned int size);
        void *ToStruct(lua_State *L, int index, Il2CppClass* klass = nullptr);
        StructUD *PushStruct(lua_State *L, Il2CppClass *klass, unsigned int size, void *pointer);
        bool TryPushStruct(lua_State *L, Il2CppClass *klass, void *pointer, unsigned int size);

        int32_t GetClassMetaId(void *kclass);
        int CSharpGetTypeId(lua_State *L, Il2CppReflectionType *reflectionType);
        void SetTypeId(void *kclass, int32_t metaId);
        int32_t GetTypeIdByIl2cppClass(lua_State *L, const Il2CppClass *klass);
        void SetGetTypeIdFuncPtr(CSharpGetTypeIdFunc methodPtr, void* method);
    };

    CppObjMapper *GetCppObjMapper();

}