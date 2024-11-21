// #pragma once

// #include <Delegates/Delegate.h>
// #include <Utils.h>

// template <typename TResult, typename... TArgs>
// class Func;

// // template <>
// // class Func<void *, void *> : public Delegate {
// // public:
// //     Func(Delegate::FuncType function) : Delegate(function) {}
// // };

// template <typename TResult>
// class Func<TResult> : public Delegate {
// public:
//     using FuncType = TResult (*)();

//     Func(FuncType function) : function(function) {}

//     TResult invoke() {
//         guard(this->function);
//         return this->function();
//     }

//     operator bool() const { return this->function != nullptr; }

// private:
//     FuncType function;
// };

// template <typename TResult, typename TArg, typename... TArgs>
// class Func<TResult, TArg, TArgs...> : public Delegate {
// public:
//     using FuncType = TResult (*)(TArg, TArgs...);

//     Func(FuncType function) : function(function) {}

//     TResult invoke(TArg arg, TArgs... args) {
//         guard(this->function);
//         return this->function(arg, args...);
//     }

//     Func<TResult, TArgs...> apply(TArg arg) {
//         guard(this->function);
//         return [=](TArgs... args) { return this->function(arg, args...); };
//     }

//     operator bool() const { return this->function != nullptr; }

// private:
//     FuncType function;
// };
