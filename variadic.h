#pragma once

#include <type_traits>

/*
 *
 */

namespace ns
{
    namespace mp
    {
        /* index_of:
         * returns the index of the first equal type found to the first template parameter in the list.
         * examples:
         * 1) index_of<A, A, B, C, D>() //0
         * 2) index_of<A, B, C, D, A>() //4
         */

        template<class T, class... Us>
        struct index_of;

        template<class T, class U, class... Vs>
        struct index_of<T, U, Vs...> : std::integral_constant<std::size_t, index_of<T, Vs...>() + 1>
        {
        };

        template<class T, class... Us>
        struct index_of<T, T, Us...> : std::integral_constant<std::size_t, 0>
        {
        };

        template<class T>
        struct index_of<T> : std::integral_constant<std::size_t, 9000>
        {
            //static_assert(false, "");
        };

        /* type_at:
         * aliases the type at a given index in a list.
         * examples:
         * 1) using a_type = type_at<0, a, b, c, d> //a
         * 2) using d_type = type_at<3, a, b, c, d> //d
         */

        namespace detail
        {
            template<std::size_t N, class T, class... Us>
            struct type_at;

            template<std::size_t N, class T, class... Us>
            struct type_at
            {
                using type = typename type_at<N - 1, Us...>::type;
            };

            template<class T, class... Us>
            struct type_at<0, T, Us...>
            {
                using type = T;
            };
        }

        template<std::size_t N, class T, class... Us>
        using type_at = typename detail::type_at<N, T, Us...>::type;

        /* contains:
         * returns true or false if a type is in a list
         * examples:
         * 1) contains<c, b, c, d, e>() //true
         * 2) contains<c, a, d, e, a>() //false
         * */

        template<class T, class... Us>
        struct contains;

        template<class T, class U, class... Vs>
        struct contains<T, U, Vs...> : contains<T, Vs...>
        {
        };

        template<class T, class... Us>
        struct contains<T, T, Us...> : std::true_type
        {
        };

        template<class T>
        struct contains<T > : std::false_type
        {
        };

        /* convert:
         * aliases a variadic template container from another container
         * examples:
         * 1) using tuple1 = convert<type_list<a, b, c>, std::tuple>; //tuple1 = std::tuple<a, b, c>
         * 2) using list1  = type_list<a, b, c>;
         *    using tuple2 = convert<list1, std::tuple>               //tuple2 = std::tuple<a, b, c>
         */

        namespace detail
        {
            template<class T, template <class...> class U>
            struct convert;

            template<template <class...> class T, class... Ts, template <class...> class U>
            struct convert<T<Ts...>, U>
            {
                using type = U<Ts...>;
            };
        }

        template<class T, template <class...> class U>
        using convert = typename detail::convert<T, U>::type;

        /* mutate:
         * aliases a variadic template container with its elements changed from another container
         * examples:
         * 1) using list1       = type_list<a, b>;      //type_list<a, b>
         *    using vector_list = mutate<list1, vector> //type_lst<vector<a, allocator<a>>, vector<b, allocator<b>>>
         */

        namespace detail
        {
            template<class T, template <class...> class U>
            struct mutate;

            template<template <class...> class T, class... Ts, template <class...> class U>
            struct mutate<T<Ts...>, U>
            {
                using type = T<U<Ts>...>;
            };
        }

        template<class T, template <class...> class U>
        using mutate = typename detail::mutate<T, U>::type;

        /* type_list
         */

        template<class... Ts>
        struct type_list
        {
            type_list()  = delete;
            ~type_list() = delete;


        };
    }
}