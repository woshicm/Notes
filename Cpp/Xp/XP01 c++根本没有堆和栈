### XP01 c++根本没有堆和栈

#### 1 企图用指针偏移访问局部变量的作为是ub

```cpp
//用局部变量a的地址访问b
void modify(int *pa) {
    int *pb = pa + 1;
    *pb = 3;
}

//Debug模式下结果可能OK，但是Release下结果错误
int func() {
    int a = 1;
    int b = 2;
    modify(&a);
    return b;
}

```

​	原因是c++架构对局部变量的分布规则没有明确的规定，编译器为了优化会在局部变量间插入「真空」。例如上面的例子中b的地址没有被用到，编译器可能把a放在内存中，b放在寄存器中，a、b之间就不是连续的。

#### 2 企图用short*修改int的低8位的行为是非法的（严格别名规则Strict aliasing）

```cpp
//用short*修改i的低8位
//[[gnu::noinline]]
__declspec(noinline)
void modify(int *p) {
    *(short*)p = 3;
}

//Debug模式下结果可能OK，但是Release+noinline结果错误
__declspec(noinline)
int func() {
    int i = 0x10002;
    modify(&i);
    return i;
}
```

​	上面的例子中，使用`(unsigned) char`是可以的。但是`enum enum_char: char{};`是不可以。虽说std::byte的实现也是`enum class byte : unsigned char {};`，但是调用

```cpp
void modify(int *p) {
    *(std::byte*)p = (std::byte)3;
}
```

是被允许的。因为标准库为此作了例外的实现。

> [!CAUTION]
>
> [20240603] 该实现暂未深入源码。

​	此外，`long long`和`long` 也是不兼容类型。 

​	在gcc中，`-fno-strict-aliasing`可以阻止编译器严格别名规则优化，msvc不会利用此优化。

#### 3 对齐

​	sizeof和alignof在基本类型中是一致的。但是结构体中，sizeof % alignof == 0.

​	`malloc`分配的内存是`__STDCPP_DEFAULT_NEW_ALIGNMENT__`（16）的整数倍，为标准库中最大的基本类型（long double=16，取决编译器）所决定。

​	`alignas`可以指定结构体的大小，但是使用`malloc`时会产生ub，使用`new`安全。

```cpp
struct alignas(64) A {
    long double ld;
}
(A*)malloc(sizeof(A)); //ub
(A*)new char[sizeof(A)]; //ub
new A; //安全
(A*)new (std::align_val_t(alignof(A))) char[sizeof(A)]; //安全
```

​	如果结构体A含有int成员，可以用A*访问int变量。

```cpp
struct A{ int val};
int i;
(A*)&i; //合法
```

​	为了避免ub，尽量不用`reinterpret_cast`或C风格的转换。

#### 4 const变量不能转为非const指针访问

​	但是有特例，允许去掉const

- 本来不是const，被转为const指针后，可以去掉const

- const变量是结构体，在mutable成员，该成员可以去掉const

  为了避免ub，尽量不用`const_cast`或C风格的转换。
