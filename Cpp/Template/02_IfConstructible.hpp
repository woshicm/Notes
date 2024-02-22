/*
 * @Date: 2024-02-22
 * @LastEditTime: 2024-02-22
 * @Description: 检测类的对象是能被构造
 * 
 * Copyright (c) 2024 by Chenmeng, All Rights Reserved. 
 */
/**方法1**/

/**方法2
 * @description: 使用lambda表达式
 */
template<class F, class...Args, class = decltype(std::declval<F>()(std::declval<Args&&>()...))>
std::true_type isValidImpl(void*);

template<class F, class...Args>
std::false_type isValidImpl(...);

inline constexpr auto isValid = [](auto f) {
	return [](auto&&... args) {
		return decltype(isValidImpl<decltype(f), decltype(args)&&...>(nullptr)){};
		};
	};

template<class T>
struct TypeT {
	using Type = T;
};

template<typename T>
constexpr auto type = TypeT<T>{};

template<typename T>
T valueT(TypeT<T>);

constexpr auto isDefaultConstructible = isValid([](auto x) ->decltype((void) decltype(valueT(x))())
	{});
