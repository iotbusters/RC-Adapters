#pragma once

#include <StateMachine/Transition.h>

class StateMachine {
private:
    Transition *entry;
    Transition *current;

public:
    StateMachine(Transition *entry) : entry(entry), current(entry) { guard(entry); }

    bool run() {
        if (!this->current)
            return true;

        Transition *next;
        if (this->current->tryMove(next))
            this->current = next;

        return true;
    }

    ~StateMachine() {
        auto head = this->entry;
        while (head && head != this->entry) {
            auto next = head->getNext();
            delete head;
            head = next != this->entry ? next : nullptr;
        }
        this->entry = nullptr;
        this->current = nullptr;
    }
};
