/*
 * @Author: woshicm 35488890+woshicm@users.noreply.github.com
 * @Date: 2024-03-18 21:09:24
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-03-26 14:56
 * @FilePath: \cm_clibs\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <fmt/core.h>

using namespace std::literals;

#if defined(_MSVC_LANG)
    #if _MSVC_LANG >=202002L
        #define CM_MSVC_CXX20
    #else if _MSVC_LANG >=201703L
        #define CM_MSVC_CXX17
    #endif  
#endif
#define CMC_HAS_CPP14_ATTRIBUTE(attribute) \
  (FMT_CPLUSPLUS >= 201402L && FMT_HAS_CPP_ATTRIBUTE(attribute))

#define CMC_HAS_CPP17_ATTRIBUTE(attribute) \
  (FMT_CPLUSPLUS >= 201703L && FMT_HAS_CPP_ATTRIBUTE(attribute))

#define CMC_HAS_CPP20_ATTRIBUTE(attribute) \
  (FMT_CPLUSPLUS >= 202003L && FMT_HAS_CPP_ATTRIBUTE(attribute))

namespace cmc
{
    struct check_ret_p{};
    template<typename T, T val>
    struct check_ret: public check_ret_p{
        static constexpr T value = val;
    };


    struct no_out_t{};
    inline constexpr no_out_t no_out{};


    template <typename T, typename... Ts>
    struct is_convertible_any {
        static constexpr bool value =(false || ... || std::is_convertible_v<Ts, T>);
    };

    template <typename T, typename... Ts>
    inline constexpr bool is_convertible_any_v = is_convertible_any<T, Ts...>::value;

    struct Check_Ret{
        template<typename T, typename... Args>
        static constexpr int get_type_index()
        {
            int ret = -1, cnt = 0;
            ((std::is_convertible_v<Args, T> ? ret = cnt : ++cnt), ...);
            return ret;
        }

        template<typename... Args>
        static constexpr int getIndex(Args... args)
        {
        return index<check_ret_p, Args...>;
        }

        template<typename... Args>
        static constexpr int index = get_type_index<check_ret_p, Args...>();

        template<size_t I, size_t N, typename T, typename... Args>
        static auto _getValue(T t, Args... args)
        {
            if constexpr (I == N || N > sizeof...(Args))
                return t;
            else
                return _getValue<I+1, N>(args...);
        }

        template<typename... Args, typename=std::enable_if_t<index<Args...> != -1>>
        static constexpr auto getValue(Args... args)
        {
            return _getValue<0, index<Args...>>(args...).value;
        }

        template<typename... Args>
        static constexpr void getValue(...)
        {}
    };

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
#define __CHECK_PRINT_LINE(x, ...) \
    cmc::Check_Print::print(fmt::format(R"([CHECK ERROR] Check "{}" failed at line {} in file "{}".)", #x, std::to_string(__LINE__), __FILE__), __VA_ARGS__); 

#if defined(CM_MSVC_CXX20)
    #define CHECK(x, ...) \
    if (!(x)){ \
        __CHECK_PRINT_LINE(x, __VA_ARGS__) \
        if constexpr(fmt::basic_string_view(__FUNCSIG__).starts_with("void ")) return void(0);\
    /* else if constexpr(Check_Ret::getIndex(__VA_ARGS__) != -1) return Check_Ret::getValue(__VA_ARGS__) false;*/\
        else return {};\
    }

    #define CHECK_R(x, r, ...) \
    if (!(x)){ \
        __CHECK_PRINT_LINE(x, __VA_ARGS__) \
        return r;\
    }
#else if defined(CM_MSVC_CXX17)
    #define CHECK(x, ...) \
    if (!(x)){ \
        __CHECK_PRINT_LINE(x, __VA_ARGS__) \
        if constexpr(fmt::basic_string_view(__FUNCSIG__).starts_with("void ")) return void(0);\
    /* else if constexpr(Check_Ret::getIndex(__VA_ARGS__) != -1) return Check_Ret::getValue(__VA_ARGS__) false;*/\
        else return {};\
    }

    #define CHECK_R(x, r, ...) \
    if (!(x)){ \
        __CHECK_PRINT_LINE(x, __VA_ARGS__) \
        return r;\
    }
#endif
}
