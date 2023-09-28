#include "type_list.hpp"

template <typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };


int main()
{
    using list1 = type_list<int, float, double>;
    using list2 = type_list<char, int>;
    using concat_list = concat<list1, list2>::type;

    // size
    static_assert(size<list1>::value == 3);

    // any_of
    constexpr auto is_int = []<typename T>(T) { return std::is_same_v<T, int>; };
    static_assert(any_of<list1, is_int>::type::value == true);

    // concat
    static_assert(std::is_same_v<concat_list, type_list<int, float, double, char, int>>);

    // transform
    constexpr auto f = overloaded
    {
        [](int)   { return float{}; },
        [](float) { return int{};   },
        [](double) { return char{}; },
    };
    static_assert(any_of<transform<concat_list, f>::type, is_int>::type::value == true);
}