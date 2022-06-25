#include <iostream>
#include "parser.hpp"


auto main() -> int {
    using P1 = mlc::parser::CharP<mlc::Character<'a'>>;
    using P2 = mlc::parser::PredP<mlc::parser::Is_digit>;
    using P3 = mlc::parser::EndP;

    using P = mlc::parser::SeqP<P1, P2, P3>;
    using I = mlc::To_string<"a5">;
    using R = mlc::parser::Parse_result<P, I>;

    std::cout << typeid(R).name();
}