#ifndef UNITS_TYPELIST_H
#define UNITS_TYPELIST_H

#include "util.h"

namespace units
{
    template<typename ...>
    struct TypeList;

    template<>
    struct TypeList<>
    {
        constexpr TypeList() = default;
        ~TypeList() = default;
        static constexpr size_t size = 0;
    };

    template<typename T>
    struct TypeList<T>
    {

        constexpr TypeList() = default;
        ~TypeList() = default;

        static constexpr size_t size = 1;
        using type = T;

        template<typename U>
        constexpr bool hasType()noexcept
        {
            return b_is_same<T, U>;
        }
    };

    template<typename T, typename ... Ts>
    struct TypeList<T, Ts...> : TypeList<Ts...>
    {
        constexpr TypeList() = default;
        ~TypeList() = default;

        using type = T;
        using base = TypeList<Ts...>;
        static constexpr size_t size = 1 + base::size;

        template<typename U>
        static constexpr bool hasType()noexcept
        {
            return b_is_same<T, U> || base::template has_type<U>();
        }
    };

}


#endif