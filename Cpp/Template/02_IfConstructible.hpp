/*
 * @Date: 2024-02-22
 * @LastEditTime: 2024-02-22
 * @Description: 检测类的对象是能被构造
 * 
 * Copyright (c) 2024 by Chenmeng, All Rights Reserved. 
 */
/**方法1
 * @description: 原始SFINAE（C++98）判断类型是否一致
 */
template<class T>
struct CanBeConstruct
{
	template<class, class = decltype(T())>
	static char test(void*);
	template<class>
	static long test(...);
	static constexpr bool value = std::is_same_v<decltype(test<T>(0)), char>;
};

/**方法2
 * @description: 使用lambda表达式
 */
template<class T>
struct CanBeConstruct1
{
	template<class U, class = decltype(U())>
	static std::true_type test(void*);
	template<class>
	static std::false_type test(...);
	static constexpr bool value = decltype(test<T>(0))::value;
};

template<class...>
using VoidT = void;

template<class, class = std::void_t<>>
struct CanBeConstruct : std::false_type {};

template<class T>
struct CanBeConstruct<T, std::void_t<decltype(T())>> : std::true_type {};

/**方法3
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
