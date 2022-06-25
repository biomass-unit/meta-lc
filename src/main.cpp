#include <iostream>
#include "parser.hpp"


auto main() -> int {
    using P1 = mlc::parser::CharP<mlc::Character<'a'>>;
    using P2 = mlc::parser::CharP<mlc::Character<'b'>>;
    using P3 = mlc::parser::CharP<mlc::Character<'c'>>;

    using P = mlc::parser::Zero_or_moreP<mlc::parser::OrP<P1, P2, P3>>;
    using I = mlc::To_string<"abcabcd">;
    using R = mlc::parser::Parse_result<P, I>;

    std::cout << typeid(R).name();
}