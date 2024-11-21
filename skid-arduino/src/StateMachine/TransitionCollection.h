#pragma once

#include <StateMachine/StateMachine.h>

#define TRANSITIONS_MAX 32

class TransitionCollection {
private:
    Transition *entry;

    Transition *find(void *state) {
        auto head = entry;
        while (head && head->getState() != state)
            head = head->getNext();
        return head;
    }

    template <typename TOut, typename TIn>
    TypedTransition<TIn, TOut> *addOrGet(TOut (*state)(TIn), bool (*condition)(TIn) = nullptr) {
        auto found = this->find(reinterpret_cast<void *>(state));
        if (!found)
            return new TypedTransition<TIn, TOut>(state, condition);
        auto transition = reinterpret_cast<TypedTransition<TIn, TOut> *>(found);
        if (condition)
            transition->addCondition(condition);
        return transition;
    }

public:
    TransitionCollection(Transition *entry) : entry(entry) { guard(entry); }

    template <typename T, typename T2, typename T3>
    void addTransition(T2 (*stateFrom)(T), T3 (*stateTo)(T2), bool (*condition)(T2)) {
        auto transitionFrom = this->addOrGet(stateFrom);
        auto transitionTo = this->addOrGet(stateTo, condition);
        transitionFrom->continueWith(transitionTo);
    }

    Transition *getEntry() { return this->entry; }

    ~TransitionCollection() { this->entry = nullptr; }
};
