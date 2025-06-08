#ifndef UNITS_QUANTITY_H
#define UNITS_QUANTITY_H
#include "dimension.h"
#include "typelist.h"

namespace units
{       
    /// <summary>
    /// struct which represents a set of dimensions.
    /// </summary>
    /// <typeparam name="...Dimensions"></typeparam>
    template<typename ... Dimensions>
    struct Quantity;

    template<typename ... Dimensions>
    constexpr Quantity<Dimensions ...> quantityFromTypeList(TypeList<Dimensions...>)noexcept;

    template<typename ... Dimensions>
    constexpr TypeList<Dimensions ...> typelistFromQuantity(Quantity<Dimensions...>)noexcept;

    template<typename TypelistType>
    using QuantitiesFromTypeList = decltype(quantityFromTypeList(declval<TypelistType>()));

    template<typename QuantityType>
    using TypelistFromQuantity = decltype(typelistFromQuantity(declval<QuantityType>()));

    namespace quantities
    {
        struct Reduce
        {
            template<typename T, int e, typename ... Ds, typename ... Cs, typename U, int e2, typename ... Others>
            static constexpr decltype(auto) reduce(Dimension<T, e> comparator, TypeList<Ds...> quantityTypes, TypeList<Cs...> storedTypes, Dimension<U, e2> comp, Others&& ... others)
            {
                //we take the first dimension and compare with all the others
                //since the dimension's tag is different, we append to the stored types
                return reduce(comparator, quantityTypes, declval<TypeList<Cs..., Dimension<U, e2>>>(), std::forward<Others>(others)...);
            }

            template<typename T, int e, typename ... Ds, typename ... Cs, int e2, typename ... Others>
            static constexpr decltype(auto) reduce(Dimension<T, e> comparator, TypeList<Ds...> quantityTypes, TypeList<Cs...> storedTypes, Dimension<T, e2> comp, Others&& ... others)
            {
                //since the dimension's tag is the same, we can add the exponents
                return reduce(declval<Dimension<T, e + e2>>(), quantityTypes, storedTypes, std::forward<Others>(others)...);
            }

            template<typename T, int e, typename ... Ds, typename U, int e2, typename ... Cs>
            static constexpr decltype(auto) reduce(Dimension<T, e> comparator, TypeList<Ds...> quantity, TypeList<Dimension<U, e2>, Cs...> storedTypes)
            {
                //when there are no more left to compare, we append to the quantity,
                //and expand the types stored in the TypeList to repeat...
                return reduce(declval<Dimension<U, e2>>(), declval<TypeList<Ds..., Dimension<T, e>>>(), declval<TypeList<>>(), declval<Cs>()...);
            }

            template<typename T, typename ... Ds, typename U, int e2, typename ... Cs>
            static constexpr decltype(auto) reduce(Dimension<T, 0> comparator, TypeList<Ds...> quantity, TypeList<Dimension<U, e2>, Cs...> storedTypes)
            {
                //when there are no more left to compare, and the compator's exponent is zero, we remove it.
                //and expand the types stored in the TypeList to repeat...
                return reduce(declval<Dimension<U, e2>>(), declval<TypeList<Ds...>>(), declval<TypeList<>>(), declval<Cs>()...);
            }

            //when the typelist is empty, then we get to the result
            template<typename T, int e, typename ... Ds>
            static constexpr TypeList<Ds..., Dimension<T, e>> reduce(Dimension<T, e> comparator, TypeList<Ds...> quantity, TypeList<> storedTypes)noexcept
            {
                return TypeList<Ds..., Dimension<T, e>>();
            }

            template<typename T, typename ... Ds>
            static constexpr TypeList<Ds...> reduce(Dimension<T, 0> comparator, TypeList<Ds...> quantity, TypeList<> storedTypes)noexcept
            {
                return TypeList<Ds...>();
            }
        };

        //helper struct to deduce if the typeList contains the dimnsion
        template<typename DimensionType, typename TypelistType>
        struct Contains
        {
            //assume false by default
            static constexpr bool value = false;
        };

        template<typename T, int e, typename U, int e2, typename ... Others>
        struct Contains<Dimension<T, e>, TypeList<Dimension<U, e2>, Others...>>
        {
            //this current tag doesn't have it so we check the next
            static constexpr bool value = Contains<Dimension<T, e>, TypeList<Others...>>::value;
        };

        template<typename T, int e, int e2, typename ... Others>
        struct Contains<Dimension<T, e>, TypeList<Dimension<T, e2>, Others...>>
        {
            //this tag is _definitely_ here.
            static constexpr bool value = true;
        };

        //helper struct to deduce if two typelists are the same. Requires both typelists to be simplified
        template<typename TypeList1, typename TypeList2, typename TypeList2Reducing>
        struct HasDimensions;

        template<typename T, int e, typename ... Others1, typename ... Others2, typename U, int e2, typename ... Others2Red>
        struct HasDimensions<TypeList<Dimension<T, e>, Others1...>, TypeList<Others2...>, TypeList<Dimension<U, e2>, Others2Red...>>
        {
            //entry point?
            
            //the tags don't match so we go to the next in the typelist
            constexpr static bool value = 
                Contains<Dimension<T, e>, TypeList<Others2...>>::value &&
                HasDimensions<TypeList<Dimension<T, e>, Others1...>, TypeList<Others2...>, TypeList<Others2Red...>>::value;
        };

        template<typename T, int e, typename ... Others1, typename ... Others2, int e2, typename ... Others2Red>
        struct HasDimensions<TypeList<Dimension<T, e>, Others1...>, TypeList<Others2...>, TypeList<Dimension<T, e2>, Others2Red...>>
        {
            //tags match so we compare exponents. we _COULD_ go to the next dimension from here.
            constexpr static bool value = (e == e2) &&
                HasDimensions<TypeList<Others1...>, TypeList<Others2...>, TypeList<Others2...>>::value;
        };

        template<typename T, int e, typename ... Others1, typename ... Others2>
        struct HasDimensions<TypeList<Dimension<T, e>, Others1...>, TypeList<Others2...>, TypeList<>>
        {
            //reached the end of typelist2, so repeat with the other dimensions in typelist1
            constexpr static bool value = HasDimensions<TypeList<Others1...>, TypeList<Others2...>, TypeList<Others2 ...>>::value;
        };

        template<typename T, int e, typename ... Others2>
        struct HasDimensions<TypeList<Dimension<T, e>>, TypeList<Others2...>, TypeList<>>
        {
            constexpr static bool value = Contains<Dimension<T, e>, TypeList<Others2...>>::value;
        };

        template<typename ... Others2, typename ... Others2Red>
        struct HasDimensions<TypeList<>, TypeList<Others2...>, TypeList<Others2Red...>>
        {
            //we're at the end of the first type list which has succesfully found all matching dimensions
            //in Typelist2
            constexpr static bool value = true;
        };
        
        constexpr TypeList<> reduceDimensions()noexcept;//{return declval<Quantity<>>();}

        template<typename Tag, int exponent, typename ... Dimensions>
        constexpr decltype(auto) reduceDimensions(Dimension<Tag, exponent> first, Dimensions&&... others)noexcept
        {
            return Reduce::reduce(first, declval<TypeList<>>(), declval<TypeList<>>(), std::forward<Dimensions>(others)...);
        }

        template<typename ... Dimensions>
        using ReducedDimensionsList = decltype(quantities::reduceDimensions(declval<Dimensions>()...));

        template<typename ... Dimensions>
        using QuantityType = QuantitiesFromTypeList<ReducedDimensionsList<Dimensions...>>;

        //for division, we need to be able to negate a typeList.
        //we can do this by extracting each dimension from the List, putting a negated instance into the typelist.
        struct Negate
        {
            template<typename T, int e, typename ... Ds, typename ... Others>
            static constexpr decltype(auto) negate(TypeList<Dimension<T, e>, Ds...>t, Others&&...o)
            {
                return negate(TypeList<Ds...>(), o..., Dimension<T, -e>());
            }

            template<typename ... Others>
            static constexpr TypeList<Others...> negate(TypeList<> t, Others&&...o);
        };


        template<typename ... Dimensions>
        using NegatedDimensionsList = decltype(quantities::Negate::negate(declval<TypeList<Dimensions...>>()));
    }

    template<typename DimensionType, typename QuantityType>
    constexpr bool b_contains_dimension = false;

    template<typename T, int e, typename ... Dimensions>
    constexpr bool b_contains_dimension<Dimension<T, e>, Quantity<Dimensions...>> =
        quantities::Contains<Dimension<T, e>, TypeList<Dimensions...>>::value;

    template<typename ... Dims1, typename ... Dims2>
    constexpr bool b_is_same<Quantity<Dims1...>, Quantity<Dims2...>> =
        quantities::HasDimensions<quantities::ReducedDimensionsList<Dims1...>, quantities::ReducedDimensionsList<Dims2...>, quantities::ReducedDimensionsList<Dims2...>>::value &&
        quantities::HasDimensions<quantities::ReducedDimensionsList<Dims2...>, quantities::ReducedDimensionsList<Dims1...>, quantities::ReducedDimensionsList<Dims1...>>::value;

    template<typename ... Dimensions>
    struct Quantity
    {
       using Simplified = quantities::QuantityType<Dimensions ...>;
    };

    template<typename ... Dims>
    constexpr Quantity<Dims...> operator+(Quantity<Dims...>, Quantity<Dims...>)noexcept;

    template<typename ... Dims1, typename ... Dims2, typename = typename TypePredicate<b_is_same<Quantity<Dims1...>, Quantity<Dims2...>>>::type>
    constexpr quantities::QuantityType<Dims1...> operator+(Quantity<Dims1...>, Quantity<Dims2...>)noexcept;

    template<typename ... Dims>
    constexpr Quantity<Dims...> operator-(Quantity<Dims...>, Quantity<Dims...>)noexcept;

    template<typename ... Dims1, typename ... Dims2, typename = typename TypePredicate<b_is_same<Quantity<Dims1...>, Quantity<Dims2...>>>::type>
    constexpr quantities::QuantityType<Dims1...> operator-(Quantity<Dims1...>, Quantity<Dims2...>)noexcept;

    template<typename ... Dims1, typename ... Dims2>
    constexpr quantities::QuantityType<Dims1..., Dims2 ...> operator*(Quantity<Dims1...>, Quantity<Dims2...>)noexcept;

    template<typename ... Dims1, typename ... Dims2, typename N = quantities::NegatedDimensionsList<Dims2...>>
    constexpr MultiplyType<Quantity<Dims1...>, QuantitiesFromTypeList<N>> operator/(Quantity<Dims1...>, Quantity<Dims2...>)noexcept;

    template<typename T>
    constexpr bool b_is_quantity = false;

    template<typename ... Dims>
    constexpr bool b_is_quantity<Quantity<Dims...>> = true;
}

#endif 