#pragma once
#include <type_traits>
#include "lua_api_adapt.h"
#include "il2cpp-object-internals.h"
#include <utils/StringUtils.h>
#include "String.h"

namespace converter{
    template <typename T, typename Enable = void>
struct Converter;
    //int64 todo
    template <typename T>
    struct Converter<T, typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 8 && std::is_signed<T>::value>::type>
    {
        static void toScript(lua_State* L, T value)
        {
        }

        static T toCpp(lua_State * L, int index)
        {
            // if (value->IsBigInt())
            // {
            //     return value.As<v8::BigInt>()->Int64Value();
            // }
            return 0;
        }

        static bool accept(lua_State *L, int index)
        {
            return false;
            // return value->IsBigInt();
        }
    };

    //int
    template <typename T>
    struct Converter<T, typename std::enable_if<std::is_integral<T>::value && sizeof(T) < 8 >::type>
    {
        static void toScript(lua_State* L, T value)
        {
            lapi_lua_pushinteger(L, value);
        }

        static T toCpp(lua_State* L, int index)
        {
            
            return lapi_xlua_tointeger(L, index);
        }

        static bool accept(lua_State* L, int index)
        {
            return lapi_lua_isinteger(L, index);
        }
    };

    //uint64
    template <typename T>
    struct Converter<T, typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 8 && std::is_unsigned<T>::value>::type>
    {
        static void toScript(lua_State* L, T value)
        {
        }

        static T toCpp(lua_State * L, int index)
        {

            return 0;
        }

        static bool accept(lua_State *L, int index)
        {
            return false;
        }
    };

    //float
    template <typename T>
    struct Converter<T, typename std::enable_if<std::is_floating_point<T>::value>>
    {
        static void toScript(lua_State* L, T value)
        {
            lapi_lua_pushnumber(L, value);
        }

        static T toCpp(lua_State * L, int index)
        {
            return lapi_lua_tonumber(L, index);
        }

        static bool accept(lua_State *L, int index)
        {
            return lapi_lua_isnumber(L, index);
        }
    };

    //bool
    template <>
    struct Converter<bool>
    {
        static void toScript(lua_State* L, bool value)
        {
            lapi_lua_pushboolean(L, value?1 : 0);
        }

        static bool toCpp(lua_State * L, int index)
        {
            return lapi_lua_toboolean(L, index) == 1;
        }

        static bool accept(lua_State *L, int index)
        {
            return lapi_lua_isboolean(L, index);
        }
    };

    // //il2cppString
    // template <>
    // struct Converter<Il2CppString>
    // {
    //     static void toScript(lua_State* L, Il2CppString value)
    //     {
    //          const Il2CppChar* utf16 = il2cpp::utils::StringUtils::GetChars(&value);
    //         std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
    //         lapi_lua_pushstring(L, str.c_str());
    //     }

    //     static Il2CppString toCpp(lua_State * L, int index)
    //     {
    //         const char* chars =  lapi_lua_tolstring(L, index);
    //         return il2cpp::vm::String::New(chars);
    //     }

    //     static bool accept(lua_State *L, int index)
    //     {
    //         return lapi_lua_isstring(L, index);
    //     }
    // };

    //void*
    template <>
    struct Converter<void*>
    {
        static void toScript(lua_State* L, void* value)
        {
             lapi_lua_pushlightuserdata(L, value);
        }

        static void* toCpp(lua_State * L, int index)
        {
            return lapi_lua_touserdata(L, index);
        }

        static bool accept(lua_State *L, int index)
        {
            return lapi_lua_isuserdata(L, index);
        }
    };

    
}