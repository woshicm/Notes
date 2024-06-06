> @Date: 2024/6/6
> @LastEditTime: 2024/6/6
> @Description: 模板——链表
> @Ref: "Qt qobjectdefs_impl"
> Copyright (c) 2024 by Chenmeng, All Rights Reserved. 

### 1.基本结构
```cpp
//链表
template<typename...>
struct Node{};

template<typename Head, typename... Tail>
struct Node<Head, Tail...>
{
    using node = Head;
    using next = Node<Tail...>;
};

//遍历
template<typename>
void print(...) {}

template<typename T>
void print(typename std::enable_if<!std::is_same<typename T::node, Node<>>::value>::type* = 0) {
    std::cout << typeid(T::node).name() << '\n';
    print<typename T::next>(nullptr);
}
```

### 2.带下标和数据
```cpp
template<int, typename...>
struct Node {
    static constexpr int ind = -1;
};

template<int N, typename Head, typename... Tail>
struct Node<N, Head, Tail...>
{
    using node = Head;
    using next = Node<N, Tail...>;
    static constexpr int ind = N - (sizeof...(Tail));
};

template<int VAL>
struct Info {
    static constexpr int val = VAL;
};

template<typename>
void print(...) {}

template<typename T>
void print(typename std::enable_if<!std::is_same<typename T::node, Node<0>>::value>::type* = 0) {
    std::cout << typeid(T::node).name() << ": " << T::ind << ", " << T::node::val << '\n';
    print<typename T::next>(nullptr);
}
```

### 3. 链表合并
```cpp
template<typename...>
struct Merge;

template<int N, int M, typename... Node1, typename... Node2>
struct Merge<Node<N, Node1...>, Node<M, Node2...> >
{
    using list = Node<N+M, Node1..., Node2...>;
};
```
