/*
 * @Date: 2024-03-18 21:09:24
 * @LastEditTime: 2024-03-26 14:56
 * @Description: 提前返回语句的精简，并能指定输出字符串和返回值
 */
#include <iostream>
#include <fmt/core.h>
#include <functional>
using namespace std::literals;

#if defined(_MSVC_LANG)
    #if _MSVC_LANG >=202002L
        #define CM_MSVC_CXX20
    #else if _MSVC_LANG >=201703L
        #define CM_MSVC_CXX17
    #endif  
#endif

#if defined(CM_MSVC_CXX20)
#include<format>
#endif

namespace cmc
{
    struct no_out_t{};
    inline constexpr no_out_t no_out{};

    template <typename T, typename... Ts>
    struct is_convertible_any {
        static constexpr bool value =(false || ... || std::is_convertible_v<Ts, T>);
    };

    template <typename T, typename... Ts>
    inline constexpr bool is_convertible_any_v = is_convertible_any<T, Ts...>::value;

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

#if defined(CM_MSVC_CXX20)
#define __CHECK_COMMON(x, ...) \
    cmc::Check_Print::print(std::format(R"([CHECK ERROR] Check "{}" failed at line {} in file "{}".)", #x, std::to_string(__LINE__), __FILE__), __VA_ARGS__); \
    cmc::Check_Lambda::invoke(__VA_ARGS__);
#else if defined(CM_MSVC_CXX17)
#define __CHECK_COMMON(x, ...) \
    cmc::Check_Print::print(fmt::format(R"([CHECK ERROR] Check "{}" failed at line {} in file "{}".)", #x, std::to_string(__LINE__), __FILE__), __VA_ARGS__); \
    cmc::Check_Lambda::invoke(__VA_ARGS__);
#endif
#if defined(CM_MSVC_CXX20)
    #define CHECK(x, ...) \
    if (!(x)){ \
        __CHECK_COMMON(x, __VA_ARGS__) \
        if constexpr(std::string(__FUNCSIG__).starts_with("void ")) return void(0);\
        else return {};\
    }

    #define CHECK_R(x, r, ...) \
    if (!(x)){ \
        __CHECK_COMMON(x, __VA_ARGS__) \
        return r;\
    }
#else if defined(CM_MSVC_CXX17)
    #define CHECK(x, ...) \
    if (!(x)){ \
        __CHECK_COMMON(x, __VA_ARGS__) \
        if constexpr(fmt::basic_string_view(__FUNCSIG__).starts_with("void ")) return void(0);\
        else return {};\
    }

    #define CHECK_R(x, r, ...) \
    if (!(x)){ \
        __CHECK_COMMON(x, __VA_ARGS__) \
        return r;\
    }
#endif
}
