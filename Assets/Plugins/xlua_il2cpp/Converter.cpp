#pragma once
#include <type_traits>
#include "lua_api_adapt.h"
#include "il2cpp-object-internals.h"
#include <utils/StringUtils.h>
#include "string.h"

namespace converter
{
    template <typename T, typename Enable = void>
    struct Converter;
    // int64 todo
    template <typename T>
    struct Converter<T, typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 8 && std::is_signed<T>::value>::type>
    {
        static void toScript(lua_State *L, T value)
        {
            lapi_lua_pushint64(L, value);
        }

        static T toCpp(lua_State *L, int index)
        {

            return lapi_lua_toint64(L, index);
        }

        static bool accept(lua_State *L, int index)
        {

            return lapi_lua_isint64(L, index);
        }
    };

    // uint64
    template <typename T>
    struct Converter<T, typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 8 && std::is_unsigned<T>::value>::type>
    {
        static void toScript(lua_State *L, T value)
        {
            lapi_lua_pushuint64(L, value);
        }

        static T toCpp(lua_State *L, int index)
        {

            return lapi_lua_touint64(L, index);
        }

        static bool accept(lua_State *L, int index)
        {

            return lapi_lua_isuint64(L, index);
        }
    };

    // int
    template <typename T>
    struct Converter < T, typename std::enable_if<std::is_integral<T>::value && sizeof(T) <8 && std::is_signed<T>::value > ::type>
    {
        static void toScript(lua_State* L, T value)
        {
            lapi_xlua_pushinteger(L, value);
        }

        static T toCpp(lua_State* L, int index)
        {
            return (T)lapi_lua_tonumber(L, index);
        }

        static bool accept(lua_State* L, int index)
        {
            return lapi_lua_isnumber(L, index);
        }
    };

    // uint
    template <typename T>
    struct Converter < T, typename std::enable_if<std::is_integral<T>::value && sizeof(T) < 8 && std::is_unsigned<T>::value > ::type>
    {
        static void toScript(lua_State *L, T value)
        {
            lapi_xlua_pushuint(L, value);
        }

        static T toCpp(lua_State *L, int index)
        {
            return (T)lapi_lua_tonumber(L, index);
        }

        static bool accept(lua_State *L, int index)
        {
            return lapi_lua_isnumber(L, index);
        }
    };

    // float
    template <typename T>
    struct Converter<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
    {
        static void toScript(lua_State *L, T value)
        {
            lapi_lua_pushnumber(L, value);
        }

        static T toCpp(lua_State *L, int index)
        {
            return lapi_lua_tonumber(L, index);
        }

        static bool accept(lua_State *L, int index)
        {
            return lapi_lua_isnumber(L, index);
        }
    };

    // bool
    template <>
    struct Converter<bool>
    {
        static void toScript(lua_State *L, bool value)
        {
            lapi_lua_pushboolean(L, value ? 1 : 0);
        }

        static bool toCpp(lua_State *L, int index)
        {
            return lapi_lua_toboolean(L, index) == 1;
        }

        static bool accept(lua_State *L, int index)
        {
            return lapi_lua_isboolean(L, index);
        }
    };

    // void*
    template <>
    struct Converter<void *>
    {
        static void toScript(lua_State *L, void *value)
        {
            lapi_lua_pushlightuserdata(L, value);
        }

        static void *toCpp(lua_State *L, int index)
        {
            return lapi_lua_touserdata(L, index);
        }

        static bool accept(lua_State *L, int index)
        {
            return lapi_lua_isuserdata(L, index);
        }
    };

}