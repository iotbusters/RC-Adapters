// #pragma once

// #include <Delegates/Delegate.h>
// #include <Utils.h>

// template <typename... TArgs>
// class Proc;

// template <>
// class Proc<> : public Delegate {
// public:
//     using ProcType = void (*)();

//     Proc(ProcType procedure) : procedure(procedure) {}

//     void invoke() {
//         guard(this->procedure);
//         this->procedure();
//     }

//     operator bool() const { return this->procedure != nullptr; }

// private:
//     ProcType procedure;
// };

// template <typename TArg, typename... TArgs>
// class Proc<TArg, TArgs...> : public Delegate {
// public:
//     using ProcType = void (*)(TArg, TArgs...);

//     Proc(ProcType procedure) : procedure(procedure) {}

//     void invoke(TArg arg, TArgs... args) {
//         guard(this->procedure);
//         this->procedure(arg, args...);
//     }

//     Proc<TArgs...> apply(TArg arg) {
//         guard(this->procedure);
//         return [=](TArgs... args) { return this->procedure(arg, args...); };
//     }

//     operator bool() const { return this->procedure != nullptr; }

// private:
//     ProcType procedure;
// };