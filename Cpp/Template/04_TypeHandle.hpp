/*
 * @Date: 2024/5/31
 * @LastEditTime: 2024/5/31
 * @Description: 类型的处理
 * @Ref: "hfsm2"
 * 
 * Copyright (c) 2024 by Chenmeng, All Rights Reserved. 
 */

//可根据参数类型定义数据类型
template <uint64_t N>
using UIndex = Conditional<N <= (1ull <<  8),  uint8_t,
			   Conditional<N <= (1ull << 16), uint16_t,
			   Conditional<N <= (1ull << 32), uint32_t,
												  void>>>;
