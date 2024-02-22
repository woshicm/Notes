1.为何 [Markdown语法](https://github.com/woshicm/Notes/blob/main/Cpp/Template/01_IfHasMethods.hpp) [1]处的的valueT(x).get不会报错？  
比较
```cpp
	constexpr auto hasFirst1 = isValid([&](auto x) ->decltype((void)valueT(x).draw(1)) {});
	std::cout << std::boolalpha << hasFirst1(type<Pic>);

	Pic pic;
	constexpr auto hasFirst2 = isValid([&](auto x) ->decltype((void)pic.draw) {});
	std::cout << std::boolalpha << hasFirst2(type<Pic>);
```
