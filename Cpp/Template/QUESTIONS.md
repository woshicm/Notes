1.为何 [Markdown语法](https://github.com/woshicm/Notes/blob/main/Cpp/Template/01_IfHasMethods.hpp) [1]处的的valueT(x).get不会报错？  
比较
```cpp
	constexpr auto hasFirst1 = isValid([&](auto x) ->decltype((void)valueT(x).draw(1)) {});
	std::cout << std::boolalpha << hasFirst1(type<Pic>); //true

	Pic pic;
	constexpr auto hasFirst2 = isValid([&](auto x) ->decltype((void)pic.draw) {});
	std::cout << std::boolalpha << hasFirst2(type<Pic>);  //true
```
[2024/2/22] 仅在vs2022中上述编译通过并能正确实现效果，但在其他编译器(gcc、clang)中编译能通过，但是输出为false.
