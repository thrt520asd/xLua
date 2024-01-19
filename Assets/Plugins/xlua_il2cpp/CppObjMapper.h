class CppObjMapper
{
private:
    std::unordered_map<void*, int32_t> objCache;
    std::unordered_map<void*, int32_t> ilclass2luaMetaId;
public:
    CppObjMapper(/* args */);
    ~CppObjMapper();
    void SetTypeId(void *kclass, int32_t metaId);
    int GetTypeId(void *kclass);
    void TryPushObject(lua_State *L, void *obj);
    int cahceRef=0;
};

CppObjMapper* GetCppObjMapper();