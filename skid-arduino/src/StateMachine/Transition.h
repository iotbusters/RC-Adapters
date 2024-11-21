#pragma once

#include <Arduino.h>
#include <Utils.h>
// #include <Delegates/Func.h>

class Transition {
public:
    virtual bool tryMove(Transition *(&next)) { return false; };
    virtual Transition *getNext() { return nullptr; };
    virtual void *getState() { return nullptr; };
    virtual ~Transition() {}
};

template <typename TIn>
class InputTransition : public Transition {
public:
    virtual void start(TIn input) = 0;
};

template <typename TIn, typename TOut>
class TypedTransition : public InputTransition<TIn> {
private:
    bool started = false;
    TOut (*state)(TIn);
    bool (*condition)(TIn);
    InputTransition<TOut> *next = nullptr;
    TIn input = TIn();

public:
    TypedTransition(TOut (*state)(TIn), bool (*condition)(TIn)) : state(state), condition(condition) {}

    TypedTransition(TOut (*state)(TIn), TIn input) : state(state), condition(nullptr) { start(input); }

    void start(TIn input) override {
        guard(!this->started);

        this->started = true;
        this->input = input;
    }

    void addCondition(bool (*condition)(TIn)) {
        guard(condition);        // not null
        guard(!this->condition); // null

        this->condition = condition;
    }

    bool tryMove(Transition *(&next)) override {
        if (this->condition && !this->condition(this->input))
            return false;

        auto output = this->state(this->input);

        if (this->next) {
            this->next->start(output);
            next = reinterpret_cast<Transition *>(this->next);
        }

        this->started = false;
        return true;
    }

    Transition *getNext() override { return this->next; }
    void *getState() override { return reinterpret_cast<void *>(this->state); }

    void continueWith(InputTransition<TOut> *next) {
        guard(!this->next); // null
        this->next = next;
    }

    ~TypedTransition() {
        if (this->next)
            delete this->next;
        this->next = nullptr;
    }
};
