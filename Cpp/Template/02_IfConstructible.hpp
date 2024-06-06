/*
 * @Date: 2024-02-22
 * @LastEditTime: 2024/6/6
 * @Description: 检测类的对象是能被构造
 * @Ref: "C++ Template, 2nd"
 * 
 * Copyright (c) 2024 by Chenmeng, All Rights Reserved. 
 */
/**方法1
 * @description: 利用模板函数重载实现
 */
//(1)自定义分支判断的结构体
template<class T>
struct isDefaultConstructibleT
{
	template<class, class = decltype(T())>
	static char test(void*);
	template<class>
	static long test(...);
//2024/6/6：这里test<T>(0)的0不能省略，为的是利用...比void*的优先级更高。如果不利用这个特性，
//	则需要通过std::enable_t控制只有一个函数被匹配。这样算是一种简化技巧。
	static constexpr bool value = std::is_same_v<decltype(test<T>(0)), char>;
};
//(2)使用std的结构体
template<class T>
struct isDefaultConstructibleT
{
	template<class U, class = decltype(U())>
	static std::true_type test(void*);
	template<class>
	static std::false_type test(...);
	static constexpr bool value = decltype(test<T>(0))::value;
};

/**方法2
 * @description: 利用类的重载实现
 */
template<class...>
using VoidT = void;

template<class, class = VoidT<>>
struct isDefaultConstructibleT : std::false_type {};

template<class T>
struct isDefaultConstructibleT<T, VoidT<decltype(T())>> : std::true_type {};

template<class T>
using isDefaultConstructible = isDefaultConstructibleT<T>;

/**方法3
 * @description: 使用lambda表达式实现
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
//调用
isDefaultConstructible(type<int>); //true
isDefaultConstructible(type<type_info>); //false
