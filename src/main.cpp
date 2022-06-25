#include <iostream>
#include "parser.hpp"


auto main() -> int {
    using P = mlc::parser::CharP<mlc::Character<'a'>>;
    using I = mlc::To_string<"abc">;
    using R = P::template Parse<I>::Result;

    std::cout << typeid(R).name();
}