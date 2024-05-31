/**
* Description : State machine definition and transition realization by hfsm2 in c++11.
 * Author: Chenmeng
 * Date: 2024-5-28
 * */

#pragma once
#include <iostream>
#define HFSM2_ENABLE_PLANS
#include "hfsm2.hpp"
#include "hfsm2_wrapper.hpp"

/** Definition **/
namespace state_machine {
#define _(state) s<state>
#define C(init, ...) M::Composite<_(init), __VA_ARGS__>
#define O(init, ...) M::Orthogonal<_(init), __VA_ARGS__>
    template<int> struct s;
    /** State definition **/
    enum E_STATE {
        Apex, I, A, F,
        N_STATE /* Used for count, user state must be defined before. */
    };

    /** Context definition **/
    using Context = struct {
        declare(, I, 2, A, F);
    };
    using Config = hfsm2::Config::ContextT<Context&>;
    using M = hfsm2::MachineT<Config>;

    /** Region structure definition **/
    using FSM = M::Root<_(Apex), _(I), _(A), _(F)>;
}

/** Transition definition **/
namespace state_machine {
#define case_connect(region, from, N, ...) case from: {connect(region, from, N, __VA_ARGS__)};
    /**
    * Common methods will be shared by all states except specicalized.
    **/
    template<int st>
    struct s_common: FSM::State {
        /** !!! You should invoke `base::enter()` and `base::exit()` actively in your overwritten methods,
         * !!! to make sure `isActive` Ws properly. **/
        void enter(...) {
            hfsm2_wrapper::isActive[st] = true;
        }
        void exit(...) {
            hfsm2_wrapper::isActive[st] = false;
        }
        /** Shared transition **/
        void update(FullControl& control) {
            switch(st) {
                case_connect(, I, 2, A, F);
            }
        }
        using base = s_common;
        static constexpr int state = st;
    };

    template<int st>
    struct no_virtual s: s_common<st>{};

    /**
     * All specilized states should be defined here.
     **/
    state(Apex) {
        /** You can perform some initialization all at once in this method. **/
        void enter(...){
            init(hfsm2_wrapper::isActive);
            base::enter();
        }
    };
}
#undef case_connect
#undef PER_CYCLE_TIME
#undef _
#undef C
#undef O
#ifdef init
#undef init
#undef init_connect
#undef connect
#undef fallback
#undef declare
#undef state_base
#undef state
#endif
