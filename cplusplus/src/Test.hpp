#pragma once
#include <type_traits>

template<typename T, typename U>
concept can_add = requires(T t, U u) { t + u; };

template<typename T, class ...Args>
concept can_construct = requires(T t, Args&& ...args) { T(args...); };

template<typename T, class ...Args>
concept can_construct2 = requires {std::is_constructible<T, Args...>::value; };


class Test {
    template<typename T, typename U> requires can_add<T, U>
    auto add(T t, U u) {
        return t + u;
    }
    template<typename T, class ...Args> requires can_construct2<T,Args...>
    T create(Args&& ...args) {}
};