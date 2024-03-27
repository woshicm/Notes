/*
 * @Author: Chen Meng
 * @Date: 2024-03-27 09:27
 * @LastEditTime: 2024-03-27 09:40
 * @FilePath: \cm_clibs\cm_traits.hpp
 * @Description: Extensive type traits libraries for C++17.
 */

#pragma once
#include <type_traits>

namespace cmc
{
    template <typename T, typename... Ts>
    struct is_convertible_any {
        static constexpr bool value =(false || ... || std::is_convertible_v<Ts, T>);
    };

    template <typename T, typename... Ts>
    inline constexpr bool is_convertible_any_v = is_convertible_any<T, Ts...>::value;
}
