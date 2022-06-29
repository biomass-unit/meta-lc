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
    using I = mlc::Make_string<R"(\a b.a b a)">;
    using R = mlc::parser::Parse_expression::template F<I>::Result;

    std::cout << to_runtime_string(typename mlc::To_string::template F<R>::Result {});
}