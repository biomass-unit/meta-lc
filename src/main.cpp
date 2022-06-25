#include <iostream>
#include "parser.hpp"


struct Increment {
    template <mlc::character C>
    struct F : mlc::Returns<mlc::Character<C::value + 1>> {};
};


auto main() -> int {
    /*using P1 = mlc::parser::CharP<mlc::Character<'a'>>;
    using P2 = mlc::parser::MapP<mlc::parser::PredP<mlc::parser::Is_digit>, Increment>;
    using P3 = mlc::parser::StrP<mlc::To_string<"Hello">>;
    using P4 = mlc::parser::EndP;

    using P = mlc::parser::SeqP<P1, P2, P3, P4>;
    using I = mlc::To_string<"a5Hello">;
    using R = mlc::parser::Parse_result<P, I>;*/

    using P = mlc::parser::One_or_moreP<mlc::parser::CharP<mlc::Character<'a'>>>;
    using I = mlc::To_string<"aaaabbb">;
    using R = mlc::parser::Parse_result<P, I>;

    std::cout << typeid(R).name();
}