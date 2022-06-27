#include <iostream>
#include <string>

#include "meta.hpp"
#include "parser.hpp"
#include "to_string.hpp"
#include "ast.hpp"
#include "lc-parser.hpp"


template <mlc::character... Cs>
auto to_runtime_string(mlc::String<Cs...>) -> std::string {
    return { Cs::value... };
}


auto main() -> int {
    /*using P1 = mlc::parser::CharP<mlc::Character<'a'>>;
    using P2 = mlc::parser::CharP<mlc::Character<'b'>>;
    using P3 = mlc::parser::CharP<mlc::Character<'c'>>;

    using P = mlc::parser::ParenP<mlc::parser::Zero_or_moreP<mlc::parser::OrP<P1, P2, P3>>>;
    using I = mlc::Make_string<"(abcabc)w">;
    using R = mlc::parser::Parse_result<P, I>;*/

    using P = mlc::parser::IntP;
    using I = mlc::Make_string<"150">;
    using R = mlc::parser::Parse_result<P, I>;

    std::cout << to_runtime_string(typename mlc::To_string::template F<R>::Result {});
}