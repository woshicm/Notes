/*
 * @Author: Chen Meng
 * @Date: 2024-03-27 09:27
 * @LastEditTime: 2024-03-27 15:28
 * @FilePath: \cm_clibs\cm_utils.hpp
 * @Description: Util libraries for C++17.
 */

#pragma once
#include <iostream>
#include <functional>
#include <string>
#include "cm_traits.hpp"
using namespace std::literals;

#if defined(_MSVC_LANG)
    #if _MSVC_LANG >=202002L
        #define CM_MSVC_CXX20
    #else if _MSVC_LANG >=201703L
        #define CM_MSVC_CXX17
    #endif  
#endif

namespace cmc
{
    /*** Check the condition's state for reurn or going on ***/
    struct no_out_t{};
    inline constexpr no_out_t no_out{};

    struct Check_Print {
        template<typename... Args>
        static constexpr bool ifDefinedOutPut = is_convertible_any_v<const char*, Args...>;
        template<typename... Args>
        static constexpr bool ifNoOutput = is_convertible_any_v<no_out_t, Args...>;
        template<typename... Args>
        static constexpr bool ifDefaultOutput = !ifDefinedOutPut<Args...> && !ifNoOutput<Args...>;

        template<typename... Args>
        std::enable_if_t<ifDefinedOutPut<Args...>>
        static _print(std::string const&, std::tuple<Args...> args){std::cout << std::get<const char*>(args) << '\n';}

        template<typename... Args>
        std::enable_if_t<ifNoOutput<Args...>>
        static _print(...){}

        template<typename... Args>
        std::enable_if_t<ifDefaultOutput<Args...>>
        static _print(std::string const& msg, ...){std::cout << msg << '\n';}

        template<typename... Args>
        static void print(std::string const& msg, Args... args)
        {
            _print<Args...>(msg, std::make_tuple(args...));
        }
    };

    template<typename... Ts>
    constexpr void* get(const std::tuple<Ts...>& tp, size_t index)
    {
        auto getE = [&](std::in_place_index_t<I>, size_t index)
        {
            return I == index ? std::get<I>(tp) : nullptr;
        }

        const void* res = nullptr;
        [&]<size_t... Is>(std::index_sequence<Is...>){
            (((res = getE(std::in_place_index_t<Is>{}, index)) != nullptr) || ...);
        }(std::make_index_sequence<sizeof...(Ts)>{});
        return res;
    }

    struct Check_Lambda {
        using LambdaT = std::function<void()>;
        template<typename T>
        static constexpr bool ifLambda = std::is_convertible_v<T, LambdaT>;

        template<typename T> std::enable_if_t<ifLambda<T>>
        static _invoke(T t)
        {
            std::invoke(t);
        }

        template<typename T> std::enable_if_t<!ifLambda<T>>
        static _invoke(T)
        {}

        template<typename... Args>
        static void invoke(Args... args)
        {
            (_invoke(args),...);
        }
    };

    #define __CHECK_COMMON(x, ...) \
        cmc::Check_Print::print("[CHECK ERROR] Check \""s + #x + " failed at line " + std::to_string(__LINE__) + " in file \"" + __FILE__ + "\".", __VA_ARGS__); \
        cmc::Check_Lambda::invoke(__VA_ARGS__);

    #define CHECK(x, ...) \
    if (!(x)){ \
        __CHECK_COMMON(x, __VA_ARGS__) \
        if constexpr(std::string_view(__FUNCSIG__)._Starts_with("void ")) return void(0);\
        else return {};\
    }

    #define CHECK_R(x, r, ...) \
    if (!(x)){ \
        __CHECK_COMMON(x, __VA_ARGS__) \
        return r;\
    }
}
