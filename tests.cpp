#include "type_list.hpp"

int main()
{
    using list = type_list<int, float, double>;

    // size
    static_assert(size<list>::value == 3);

    // any_of
    constexpr auto is_int = []<typename T>(T) { return std::is_same_v<T, int>; };
    static_assert(any_of<list, is_int>::type::value == true);
}