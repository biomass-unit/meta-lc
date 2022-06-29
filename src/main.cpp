#include <iostream>
#include <string_view>

#include "meta.hpp"
#include "parser.hpp"
#include "to_string.hpp"
#include "ast.hpp"
#include "lc-parser.hpp"


template <mlc::character... Cs>
auto to_runtime_string(mlc::String<Cs...>) -> std::string_view {
    static constexpr char string[] { static_cast<char>(Cs::value)..., '\0' };
    return string;
}


auto main() -> int {
    /*using I = mlc::Make_string<R"(\f.(\x.f (x x)) (\x.f (x x)))">;
    using R = mlc::Parse_expression::template F<I>::Result;

    std::cout << to_runtime_string(typename mlc::To_string::template F<R>::Result {});*/

    using I = mlc::Make_string<R"(
        true  = \a b.a
        false = \a b.b
        and   = \a b.a b a
        or    = \a b.a a b
        not   = \a.a false true
    )">;
    using R = mlc::Parse_environment::template F<I>::Result;

    std::cout << to_runtime_string(typename mlc::To_string::template F<R>::Result {});
}