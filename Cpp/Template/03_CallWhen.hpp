/*
 * @Date: 2024-02-22
 * @LastEditTime: 2024-02-22
 * @Description: 根据对象类型做决策
 * @Ref: "C++ Template, 2nd"
 * 
 * Copyright (c) 2024 by Chenmeng, All Rights Reserved. 
 */

	template<class T1, class T2>
	struct is_sameT : std::false_type {};

	template<class T>
	struct is_sameT<T, T> : std::true_type {};

	template<class T1, class T2>
	using is_same = is_sameT<T1, T2>;

	void testImpl(std::true_type)
	{
		std::cout << "True Type called";
	}

	void testImpl(std::false_type)
	{
		std::cout << "False Type called";
	}

	template<class T>
	void test(T)
	{
		testImpl(is_same<bool, T>{});
	}
