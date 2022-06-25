#include <iostream>
#include "parser.hpp"


auto main() -> int {
    using R = mlc::parser::Is_space::F<mlc::Character<'a'>>::Result;
    std::cout << std::boolalpha << R::value;
}