#pragma once

#include <type_traits>
#include <utility>

// Type list definition
template <typename... Ts>
struct type_list{};


// Get list size (i.e.: its length)
template <typename List, typename... Ts>
struct size;

template <template <typename...> typename List, typename... Ts>
struct size<List<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)>{};


// Get a type using its index in the list
template <std::size_t N, typename T, typename...Ts>
struct at
{
    using type = typename at<N - 1, Ts...>::type;
};

template <typename T, typename...Ts>
struct at<0, T, Ts...>
{
    using type = T;
};

template <auto N, typename... Ts>
using at_t = typename at<N, Ts...>::type;

// Check if any element matches a predicate. 
// Returns a std::bool_constant type.
template <typename List, auto Pred>
struct any_of;

template <template <typename...> typename List, typename... Ts, auto Pred>
struct any_of<List<Ts...>, Pred> : std::type_identity<decltype
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

template <template <typename...> typename List, typename... Ts, typename... Us>
struct zip<List<Ts...>, List<Us...>>
{
    using type = List<List<Ts, Us>...>;
};


// transform
template <typename List, auto F>
struct transform;

template <template <typename...> typename List, typename... Ts, auto F>
struct transform<List<Ts...>, F> : std::type_identity<decltype
(
    []<auto... Is>(std::index_sequence<Is...>)
    {
        return type_list<decltype(F(at_t<Is, Ts...>{}))...>{};
    }(std::index_sequence_for<Ts...>{})
)>{};


// count_if
template <typename List, auto Pred>
struct count_if;

template <template <typename...> typename List, typename... Ts, auto Pred>
struct count_if<List<Ts...>, Pred> : std::type_identity<decltype
(
    []<auto... Is>(std::index_sequence<Is...>)
    {
        return std::integral_constant<std::size_t, (std::size_t{0} + ... + (Pred(at_t<Is, Ts...>{}) ? 1 : 0))>{};
    }(std::index_sequence_for<Ts...>{})
)>{};


// count
template <typename List, typename T>
struct count;

template <template <typename...> typename List, typename... Ts, typename T>
struct count<List<Ts...>, T> : count_if<List<Ts...>, []<typename U>(U) { return std::is_same_v<T, U>; }>{};


struct not_found{};

template <auto Pred, typename List>
struct index_if;

template <template <typename...> typename List, auto Pred, typename... Ts>
struct index_if<Pred, List<Ts...>> : std::type_identity<decltype
(
    []<auto... Is>(std::index_sequence<Is...>)
    {
        return std::integral_constant<int, (0 + ... + (Pred(at_t<Is, not_found, Ts...>{}) ? int{Is} : 0)) - 1>{};
    }(std::index_sequence_for<not_found, Ts...>{})
)>{};


template <typename T, typename List>
struct index_of;

template <template <typename...> typename List, typename T, typename... Ts>
struct index_of<T, List<Ts...>> : index_if<[]<typename U>(U) { return std::is_same_v<T, U>; }, List<Ts...>>{}; 


//find_if
template <auto Pred, typename NotFound, typename List>
struct find_if;

template <auto Pred, typename NotFound, template <typename...> typename List, typename... Ts>
struct find_if<Pred, NotFound, List<Ts...>> : std::type_identity<decltype
(
    []
    {
        constexpr auto index = index_if<Pred, List<Ts...>>::type::value;
        if constexpr (index == -1)
        {
            return NotFound{};
        }
        else
        {
            return at_t<index, Ts...>{};
        }
    }()
)>{};
