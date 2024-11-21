#pragma once

#include <StateMachine/TransitionCollection.h>

template <typename TOut, typename TIn>
class TransitionBuilder {
private:
    TransitionCollection *collection;
    TOut (*stateFrom)(TIn);

public:
    TransitionBuilder(TransitionCollection *collection, TOut (*stateFrom)(TIn)) : collection(collection), stateFrom(stateFrom) {}

    template <typename TNextOut>
    TransitionBuilder<TNextOut, TOut> add(TNextOut (*stateTo)(TOut), bool (*condition)(TOut)) {
        guard(stateTo);          // not null
        guard(this->collection); // not null

        this->collection->addTransition(this->stateFrom, stateTo, condition);
        return TransitionBuilder<TNextOut, TOut>(this->collection, stateTo);
    }

    StateMachine *build() {
        auto machine = new StateMachine(this->collection->getEntry());
        delete this->collection;
        this->collection = nullptr;
        return machine;
    }
};
