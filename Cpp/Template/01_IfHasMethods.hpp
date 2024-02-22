/*
 * @Date: 2024-02-22
 * @LastEditTime: 2024-02-22
 * @Description: 检测类中是否存在某些函数
 * @Ref: "C++ Template, 2nd"
 * 
 * Copyright (c) 2024 by Chenmeng, All Rights Reserved. 
 */

/**方法1
 * @description: C++20
 */
template<class T>
concept StrOnly = requires(T x)
{
	{x.draw()} -> std::convertible_to<void>;
	{x.get()} -> std::convertible_to<int>;
};

template<class T>
requires StrOnly<T>
void print(T&& t)
{
	std::cout << t.get();
}

class Pic
{
public:
	int get()
	{
		return 100;
	}
	void draw() {}
};

/**方法2
 * @description: lambda实现，C++17
 */
//引用03_CallWhen.hpp中的isValid
constexpr auto hasGet = isValid([](auto x) ->decltype((void)valueT(x).get) {});
