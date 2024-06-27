## XP03 类型擦除+工厂模式，告别if-else

> 已有库的类，为其添加公共的基类。

### 1 使用模板封装

```cpp
//1.定义虚基类
struct MsgBase{
	virtual void speak();
	virtual ~Msg()=default; //所有虚基类必须定义虚析构函数
};
//2.定义封装类
template<class Msg>
struct MsgImpl: MsgBase {
    Msg msg;
    template<tyname... Ts>
    MsgImpl(Ts &&...ts): msg(std::forward<Ts>(ts)...){}
    void speak() override{
        msg.speak();
    }
};
//3.定义调用接口
template<class Msg, class... Ts>
auto makeMsg(Ts &&... ts){
    return std::make_shared<MsgImpl<Msg>>(std::forward<Ts>(ts)...);
}
```

> 需要为库的类新增方法

### 2 使用函数重载

```cpp
void update(){
    //以往做法是给每种类型一个枚举值，根据枚举执行相应的运作
    if(type == 1) 
        ...
    else if(type == 2)
        ...
}
//使用重载
void do(A类型){}
void do(B类型){}
void update(){
    do(类型);
}
```

### 3 工厂方法

```cpp
struct MsgFactoryBase{
    virtual MsgBase::Ptr create()=0;
    virtual ~MsgFactoryBase()=default;
    using Ptr = std::shared_ptr<MsgFactoryBase>;
}

template<typename Msg>
struct MsgFactoryImpl: MsgFactoryBase{
    MsgBase::Ptr create() override{
        return std::make_shared<MsgImpl<Msg>>();
    }
}

template<typename Msg>
MsgFactoryBase::Ptr makeFactory(){
    return std::make_shared<MsgFactoryImpl<Msg>>();
}

void main()
{
    int type = 1;
    std::map<int, MsgFactoryBase::Ptr> lut{ //map+工厂模式，可以将「if(type==1) msg = makeMsg<A>(); else if... 」的写法变得更加简洁
        {1, makeFactory<A>}, 
        {2, makeFactory<B>}
    };
    auto msg = lut.at(type)->create(); //使用at的好处在于，当lut没有key时，不会自动创建，而是直接抛异常
    msg->speak();
}
```



