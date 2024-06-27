## XP04 自己封装RAII类

### 1 拷贝赋值的指针双重释放问题

```cpp
struct Resource {
    Resource() {
        p = new int(0);
        std::cout << "Resource()" << std::endl;
    }
    //问题：不同的对象指向同一个p，导致指针双重释放
    Resource(const Resource& that): p(that.p) {
        std::cout << "Resource(const Resource&)" << std::endl;
    }
    ~Resource() {
        delete p;
        std::cout << "~Resource()" << std::end l;
    }
    int* p;
};
void func(Resource r){}
void main() {
    Resource r;
//情形1：使用=拷贝
    Resource r2 = r; //左边有类型，等价于Resource r2(r)，只有 r = r2；才会调用拷贝赋值
//情形2：传参拷贝
    func(r);
}
```

> [!TIP]
>
> 1.【不是有=就调用拷贝赋值函数】
>
> Resource r2 = r; 左边有类型，等价于Resource r2(r)，只有 r = r2；，即已有r对象，再去赋值才会调用拷贝赋值
>
> 2.【Resource r2(r) 和 Resource r2 = r 区别】
>
> 一般情况下二者等价。但当Resource 的构造函数声明了explicit时，会禁用Resource r2 = r，即不允许在构造时产生隐式转换，即当声明了 explict Resource (int i)，不允许 Resource  r = 1的写法。



### 2 处理

```cpp
/**	C++98	**/
//1.只声明，不实现，等链接时报错
Resource(const Resource& that);
//2.模拟移动构造的原理
Resource(Resource& that){ //去掉const为了改动that，调用的时候如果是非const Res ource对象会调用
    p = that.p;
    that.p = nullptr;
}

/**	C++11	**/
//1.删除构造函数
Resource(const Resource& that)=delete;
//2.移动构造函数
Resource(Resource&& that){
    p = that.p;
    that.p = nullptr;
}
~Resource() {
    if(p) //当p=nullptr,对于delete和free,不判空也可以，但是对于fclose()等就需要判空
    	delete p;
}
void main() {
    Resource r;
//情形1：使用=拷贝
    Resource r2 = std::move(r); //默认的Resource r2=r调用拷贝构造，强行使用移动构造
//或者
    Resource r2 = (Resource&&)r;
//情形2：传参拷贝
    func(std::move(r);
}
```

> [!IMPORTANT]
>
> std::move()不会转移资源，源码层面只是使用static_cast<Resource&&>做转换。
>
> 1.普通字面量，如func(11)中的「11」，func(Resource{})中的「Resource{}」，默认是右值，使用
>
> std::move(11)或std::move(Resource{})是多余的操作。
>
> 2.只有当不加std::move会调用拷贝构造时（上面main中的两种情形），std::move才真正有效。

### 3 推荐的写法

#### 3.1 禁用拷贝和移动

```cpp
struct Resource {
    Resource(): p(new int(0)) {}
    // 删除移动构造，编译器会将其他构造函数全删
    // 禁止拷贝和移动
    Resource(Resource&& that)=delete;
    ~Resource() {
        delete p;
    }
    int* p;
};

//1.使用std::unique_ptr
void func(std::unique_ptr<Resource>){}
void main() {
    auto r = make_unique<Resource>();
//由于std::unique_ptr不支持拷贝，只能通过移动的方式或引用的方式传递资源
//情形1：使用=拷贝
    Resource r2 = std::move(r);
//情形2：传参拷贝
    func(std::move(r));
}

//2.使用std::shared_ptr
void func(std::shared_ptr<Resource>){}
void main() {
    auto r = make_shared<Resource>();
//由于std::unique_ptr不支持拷贝，只能通过移动的方式或引用的方式传递资源
//情形1：使用=拷贝
    Resource r2 = r; //引用、移动和拷贝都可以
//情形2：传参拷贝
    func(r); //引用、移动和拷贝都可以
}
```

> [!TIP]
>
> 1.删除移动构造函数，编译器会将其他构造函数全删
>
> 2.删除移动赋值函数，会保留默认构造和析构函数，其他构造函数全删

#### 3.2 内部类管理资源

```cpp
struct Resource {
    struct Impl {
        Impl(): p(new int(0)) {}
        Impl(Impl&&) = delete;
        ~Impl(){
            delete p;
        }
        int* p;
    };
public:
    Resource(): _impl(std::make_shared<Impl>()){}
    Impl* operator->(){
        return _impl.get();
    }
    //默认是对shared_ptr的浅拷贝，自定义深拷贝的方法
    Resource clone() const{ 
        return std::make_shared<Impl>();
    }
private:
    Resource(std::shared_ptr<Impl> impl){
        _impl = impl;
    }
    std::shared_ptr<Impl> _impl;
};

//1.使用std::unique_ptr
void func(std::unique_ptr<Resource>){}
void main() {
    auto r = make_unique<Resource>();
//由于std::unique_ptr不支持拷贝，只能通过移动的方式或引用的方式传递资源
//情形1：使用=拷贝
    Resource r2 = std::move(r);
//情形2：传参拷贝
    func(std::move(r));
}

//2.使用std::shared_ptr
void func(std::shared_ptr<Resource>){}
void main() {
    auto r = make_shared<Resource>();
//由于std::unique_ptr不支持拷贝，只能通过移动的方式或引用的方式传递资源
//情形1：使用=拷贝
    Resource r2 = r; //引用、移动和拷贝都可以
//情形2：传参拷贝
    func(r); //引用、移动和拷贝都可以
}
```

#### 3.3 定义了析构函数最好把五大构造函数删除

这同样是因为双重指针释放问题带来的影响。尽管类的析构函数不涉及资源管理，但是析构函数释放2可能带来副作用。

```cpp
struct scope_guard{
    function<void()> func;
    scope_guard(function<void()>): func(func){}
    ~scope_guard(){ func(); }
};
```

如以上的守护函数，在功能上会带来问题。

### 4 题外话

#### 4.1 std::shared_ptr配合文件指针使用可起到scope_guard效果

​	当提前返回造成的文件资源不有效关闭时，通常使用scope_guard确保。然而使用share_ptr貌似能起到相同的效果。

```cpp
auto filename = "data1.ini"s;
auto p = std::shared_ptr<FILE>(fopen(filename.c_str(), "r"),
    [filename](FILE* handle) { //在删除器里指定善后操作
        if(handle)
            fclose(handle); 
        std::cout << "File " + filename + "not exist.\n";
    });
//或者
auto p = std::shared_ptr<FILE>(fopen(filename.c_str(), "r"),
    							std::default_delete<FILE>());
```

#### 4.2 std::unique_ptr的删除器

std::unique_ptr的删除器类型是写在模板参数里的，使用时需要显式指明。

std::unique_ptr在接收了nullptr时，对象不会创建，因此无须删除；std::shared_ptr的删除器总是被调用，可以在删除器里处理空指针的情况。

```cpp
auto p = std::unique_ptr<FILE, function<void(int*)>>(fopen(filename.c_str(), "r"),
    [filename](FILE* handle) { fclose(handle); }); //能被调用则handle一定不为空，因此无须判空
```

#### 4.3 Pimpl (Pointer to Implementation)

为了解决仅需定义时需要在头文件中引入大量别的头文件的问题。

```cpp
//1.如果仅声明指针，使用前向声明即可
struct File;
class Resource {
    File* file;
}

//2.如果需声明成员变量，使用Pimpl
//在.h文件中
class Resource{
    struct Impl;
    Impl* impl;
}

//在.cpp中
#include <File>
struct Resource::Impl{
    File file;
};
```

#### 4.4 const和非const函数

当`const`和`非const`函数都实现时，常量调用const函数，变量调用非const函数；

当只有`const`版本时，常量和变量都将调用const函数。

当类不会改变时（即有可能会被声明const），其中的所有函数都应声明为const；

当类会可变时，纯get类型的函数应声明为const，get+返回引用（即既可以get，也可以set）的函数应同时有const和非const版本。

```cpp
struct MyStr {
	const char& operator[] (size_t p) const{
		return _text[p];
	} 
	char& operator[] (size_t p) { // 非const版本通过const版本实现
		return const_cast<char&>(static_cast<const MyStr &>(*this)[p]);
	}
private:
    std::array<char, 10> _text;
};
```

#### 4.5 CTAD（Class Template Argument Deduction）

C++17引入，模板参数自动推导。

```cpp
```

