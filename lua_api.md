lua_rawset(L, int index, lua_Integer i)
= t[i] =v , t是栈[index]的table

void lua_pop (lua_State *L, int n);
从lua栈中弹出n个元素