#ifndef UNITS_UTIL_H
#define UNITS_UTIL_H

#include <cstdlib>
#include <cstdint>

namespace units
{
    template<typename T>
    T&& declval();

    template<typename T, typename U>
    constexpr bool b_is_same = false;

    template<typename T>
    constexpr bool b_is_same<T, T> = true;

    template<typename T, typename U>
    inline constexpr bool has_multiply(...)noexcept { return false; }

    template<typename T, typename U, typename Q = decltype(declval<T>() * declval<U>())>
    inline constexpr bool has_multiply(T*)noexcept { return true; }

    template<typename T, typename U>
    constexpr bool b_has_multiply = has_multiply<T, U>(0);

    template<typename T, typename U, typename Q = decltype(declval<T>() + declval<U>())>
    using AddType = Q;

    template<typename T, typename U, typename Q = decltype(declval<T>() - declval<U>())>
    using SubtractType = Q;

    template<typename T, typename U, typename Q = decltype(declval<T>() * declval<U>())>
    using MultiplyType = Q;

    template<typename T, typename U, typename Q = decltype(declval<T>()/declval<U>())>
    using DivideType = Q;

    template<typename T>
    struct IsEmptyHelper : T 
    {
        char data[4];
    };

    template<typename T>
    constexpr bool b_is_empty = sizeof(IsEmptyHelper<T>) == sizeof(IsEmptyHelper<T>::data);

    template<bool val, typename T = void>
    struct TypePredicate{};

    template<typename T>
    struct TypePredicate<true, T>
    {
        using type = T;
    };

    template<size_t n, typename ... Ts>
    struct NthTypeHelper
    {
        //if we end up here, there is a huge error somewhere.
    };

    template<size_t n, typename T, typename ... Ts>
    struct NthTypeHelper<n, T, Ts...>
    {
        using type = typename NthTypeHelper<n - 1, Ts...>::type;
    };

    template<typename T, typename ... Ts>
    struct NthTypeHelper<0, T, Ts...>
    {
        using type = T;
    };

    template<size_t n, typename ... Ts>
    using nth_type = typename NthTypeHelper<n, Ts...>::type;

    template<typename ... Ts>
    using first_type = nth_type<0, Ts ...>;

    template<typename ... Ts>
    using last_type = nth_type<sizeof...(Ts) - 1, Ts ...>;

    template<bool value, typename FalseType, typename TrueType>
    using BoolTypePredicate = nth_type<value, FalseType, TrueType>;
            
}

#endif 