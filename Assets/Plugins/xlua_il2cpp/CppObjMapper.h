#include "unordered_map"

class CppObjMapper
{
private:
    std::unordered_map<void*, int32_t> objCache;
    std::unordered_map<void*, int32_t> ilclass2luaMetaId;
    int32_t cacheRef;
public:
    CppObjMapper(/* args */);
    ~CppObjMapper();
    void SetTypeId(void *kclass, int32_t metaId);
    int GetTypeId(void *kclass);
    bool TryPushObject(lua_State
     *L, void *obj);
    void SetCacheRef(int32_t cache_ref);
};

CppObjMapper* GetCppObjMapper();
