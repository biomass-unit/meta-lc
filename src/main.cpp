#include <iostream>
#include "meta.hpp"


auto main() -> int {
    using L1 = mlc::List<int, float>;
    using L2 = L1::Concat::F<mlc::List<char, double>>::Result;

    std::cout << typeid(L2).name();
}