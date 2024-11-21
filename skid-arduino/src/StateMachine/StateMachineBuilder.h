#pragma once

#include <StateMachine/TransitionBuilder.h>

class StateMachineBuilder {
public:
    template <typename TOut, typename TIn>
    TransitionBuilder<TOut, TIn> add(TOut (*state)(TIn), TIn input) const {
        auto transition = new TypedTransition<TIn, TOut>(state, input);
        auto collection = new TransitionCollection(transition);
        return TransitionBuilder<TOut, TIn>(collection, state);
    }

    template <typename TOut, typename TIn>
    TransitionBuilder<TOut, TIn> add(TOut (*state)(TIn)) const {
        auto transition = new TypedTransition<TIn, TOut>(state, TIn());
        auto collection = new TransitionCollection(transition);
        return TransitionBuilder<TOut, TIn>(collection, state);
    }
};
