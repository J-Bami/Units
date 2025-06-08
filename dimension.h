#ifndef UNITS_DIMENSION_H
#define UNITS_DIMENSION_H

#include "util.h"

namespace units
{
    /// <summary>
    /// Represents a base physical dimension raised to an integer power
    /// </summary>
    /// <typeparam name="Tag"></typeparam>
    /// <typeparam name="_exponent"></typeparam>
    template<typename Tag, int _exponent = 1>
    struct Dimension
    {
        static_assert(b_is_empty<Tag>);

        using dimension = Tag;
        static constexpr int exponent = _exponent;
    };

    template<typename T, typename U>
    constexpr bool is_same_dimension = false;

    template<typename T, int e1, int e2>
    constexpr bool is_same_dimension<Dimension<T, e1>, Dimension<T, e2>> = true;
}


#endif 