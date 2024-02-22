/*
 * @Date: 2024-02-22 13:25
 * @LastEditTime: 2024-02-22 13:29
 * @Description: 检测类中是否存在某些函数
 * 
 * Copyright (c) 2024 by Microport, All Rights Reserved. 
 */

#pragma once
#include <iostream>

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
