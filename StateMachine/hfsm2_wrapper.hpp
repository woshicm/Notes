/**
 * Description : Provide easy-used interfaces for hfsm2 in C++11
 * Author: Cm
 * Date: 2024-5-28
 * */

#pragma once
#include <functional>
/** Inner part for the wrapper, don't use this directly. **/
namespace hfsm2_wrapper {
#define MAX_STATE 100
#define EXPAND(...) __VA_ARGS__
#define ITER_1(F, region, from, to)                                                 F(region, from, to)
#define ITER_2(F, region, from, to1, to2)                                           ITER_1(F, region, from, to1)                                         ITER_1(F, region, from, to2)
#define ITER_3(F, region, from, to1, to2, to3)                                      ITER_2(F, region, from, to1, to2)                                    ITER_1(F, region, from, to3)
#define ITER_4(F, region, from, to1, to2, to3, to4)                                 ITER_3(F, region, from, to1, to2, to3)                               ITER_1(F, region, from, to4)
#define ITER_5(F, region, from, to1, to2, to3, to4, to5)                            ITER_4(F, region, from, to1, to2, to3, to4)                          ITER_1(F, region, from, to5)
#define ITER_6(F, region, from, to1, to2, to3, to4, to5, to6)                       ITER_5(F, region, from, to1, to2, to3, to4, to5)                     ITER_1(F, region, from, to6)
#define ITER(F, region, from, N, ...) EXPAND(ITER_##N(F, region, from, __VA_ARGS__))
#define ITER_CONNECT(region, from, to) CONNECT_STATE(from, to, control.context().j##region##_##from##2##to)
#define CONNECT_STATE(from, to, cond) if(from == state && cond) {control.changeTo<s<to>>(); return;}
#define DECLARE_STATE(region, from, to) bool j##region##_##from##2##to {};
}

/** Interface **/
namespace hfsm2_wrapper {
/* Init attributes in the wrapper. */
#define init(para) memset(para, 0, sizeof(para));
/* Make a transition from `from` to `to` on condition `j{region}_{from}2{to}` is `true`, and N is the number of `to`.
   Notice that all transitions with condition should be declared in one statement.  */
#define connect(region, from, N, ...) ITER(ITER_CONNECT, region, from, N, __VA_ARGS__)
/* Make a transition from `from` to `to` with no condition. */
#define fallback(from, to)  CONNECT_STATE(from, to, true)
/* Declare a bool value from `from` to `to` named `j{region}_{from}2{to}`, and N is the number of `to`. */
#define declare(region, from, N, ...) ITER(DECLARE_STATE, region, from, N, __VA_ARGS__)
/* Define a state derived from `base`. */
#define state_base(st, base) template<> struct s<st>: base<st>
/* Define a state derived from `s_common`. */
#define state(st) state_base(st, s_common)
/* Indicate compiler not to generate virtual functions */
#define no_virtual HFSM2_EMPTY_BASES
}

/** User space **/
namespace hfsm2_wrapper{
    /* Record whether a state is active */
    static bool isActive[MAX_STATE];
}
