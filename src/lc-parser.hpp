#pragma once

#include "parser.hpp"
#include "ast.hpp"


namespace mlc::parser {

    struct Make_church {
    private:
        template <std::size_t n>
        struct Helper : Returns<
            ast::Application<
                ast::Variable<String<Character<'f'>>>,
                typename Helper<n - 1>::Result
            >
        > {};
        template <>
        struct Helper<0> : Returns<ast::Variable<String<Character<'x'>>>> {};
    public:
        template <class>
        struct F;
        template <std::size_t n>
        struct F<std::integral_constant<std::size_t, n>> : Returns<
            ast::Abstraction<
                String<Character<'f'>>,
                ast::Abstraction<
                    String<Character<'x'>>,
                    typename Helper<n>::Result
                >
            >
        > {};
    };

    using IntP = MapP<
        One_or_moreP<PredP<Is_digit>>,
        Compose2::template F<Make_church, String_to_integer>::Result
    >;

} 