#pragma once

#include "ast.hpp"


namespace mlc::env {

    struct Lookup {
        template <string, ast::environment>
        struct F : Failure {};
        template <string Name, ast::expression Expression, ast::binding... Bindings>
        struct F<Name, List<ast::Binding<Name, Expression>, Bindings...>> : Returns<Expression> {};
        template <string Name, ast::binding Binding, ast::binding... Bindings>
        struct F<Name, List<Binding, Bindings...>> : F<Name, List<Bindings...>> {};
    };

    struct Insert {
        template <ast::binding, ast::environment>
        struct F;
        template <ast::binding Binding, ast::binding... Bindings>
        struct F<Binding, List<Bindings...>> : Returns<List<Bindings..., Binding>> {};
    };

    struct Is_defined {
        template <string Name, ast::environment Environment>
        using F = Returns<
            std::negation<std::is_same<
                typename Lookup::template F<Name, Environment>::Result,
                Failure
            >>
        >;
    };

}