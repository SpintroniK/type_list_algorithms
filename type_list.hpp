#pragma once

#include <tuple>
#include <type_traits>

// Type list definition
template <typename... Ts>
struct type_list{};


// Get list size (i.e.: its length)
template <typename List>
struct size;

template <typename... Ts>
struct size<type_list<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)>{};


// Get a type using its index in the list
template <auto N, typename List>
struct at;

template <auto N, typename... Ts>
struct at<N, type_list<Ts...>>
{
    using type = std::tuple_element_t<N, std::tuple<Ts...>>;
};

template <auto N, typename... Ts>
using at_t = typename at<N, type_list<Ts...>>::type;


// Convert type_list into a std::tuple
template <typename List>
struct to_tuple;

template <typename... Ts>
struct to_tuple<type_list<Ts...>>
{
    using type = std::tuple<Ts...>;
};


// Check if any element matches a predicate. 
// Returns a std::bool_constant type.
template <typename List, auto Pred>
struct any_of;

template <typename... Ts, auto Pred>
struct any_of<type_list<Ts...>, Pred> : std::type_identity<decltype
(
    []<auto... Is>(std::index_sequence<Is...>)
    {
        return std::bool_constant<(Pred(at_t<Is, Ts...>{}) || ...)>{};
    }(std::index_sequence_for<Ts...>{})
)>{};


// Concatenate two type lists
template <typename List1, typename List2>
struct concat;

template <typename... Ts, typename... Us>
struct concat<type_list<Ts...>, type_list<Us...>>
{
    using type = type_list<Ts..., Us...>;
};


// zip
template <typename List1, typename List2>
struct zip;

template <typename... Ts, typename... Us>
struct zip<type_list<Ts...>, type_list<Us...>>
{
    using type = type_list<type_list<Ts, Us>...>;
};


// transform
template <typename List, auto F>
struct transform;

template <typename... Ts, auto F>
struct transform<type_list<Ts...>, F> : std::type_identity<decltype
(
    []<auto... Is>(std::index_sequence<Is...>)
    {
        return type_list<decltype(F(at_t<Is, Ts...>{}))...>{};
    }(std::index_sequence_for<Ts...>{})
)>{};


// count_if
template <typename List, auto Pred>
struct count_if;

template <typename... Ts, auto Pred>
struct count_if<type_list<Ts...>, Pred> : std::type_identity<decltype
(
    []<auto... Is>(std::index_sequence<Is...>)
    {
        return std::integral_constant<std::size_t, (std::size_t(0) + ... + (Pred(at_t<Is, Ts...>{}) ? 1 : 0))>{};
    }(std::index_sequence_for<Ts...>{})
)>{};

// count
template <typename List, typename T>
struct count;

template <typename... Ts, typename T>
struct count<type_list<Ts...>, T> : count_if<type_list<Ts...>, []<typename U>(U) { return std::is_same_v<T, U>; }>{};


