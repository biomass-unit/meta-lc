#pragma once

#include "meta.hpp"
#include "parser.hpp"
#include "ast.hpp"


namespace mlc {

    struct To_string {
        template <class>
        struct F;
    };


    template <>
    struct To_string::F<Failure> : Returns<Make_string<"mlc::Failure">> {};

    template <character... Cs>
    struct To_string::F<String<Cs...>> : Returns<String<Cs...>> {};

    template <character C>
    struct To_string::F<C> : Returns<String<C>> {};

    template <>
    struct To_string::F<List<>> : Returns<String<>> {};

    template <class T, class... Ts> requires (!character<T> || !(character<Ts> && ...))
    struct To_string::F<List<T, Ts...>> :
        std::conditional_t<
            sizeof...(Ts) == 0,
            typename To_string::template F<T>::Result,
            typename To_string::template F<T>::Result::Concat::template F<String<Character<','>, Character<' '>>>::Result
        >::Concat::template F<typename F<List<Ts...>>::Result> {};

    template <std::size_t n>
    struct To_string::F<std::integral_constant<std::size_t, n>> :
        Integer_to_string::template F<std::integral_constant<std::size_t, n>> {};

    template <class T, string I>
    struct To_string::F<parser::Success<T, I>> : Concat::template F<
        Make_string<"Result: ">,
        typename To_string::F<T>::Result,
        Make_string<", with remaining: ">,
        I
    > {};


    template <string Name>
    struct To_string::F<ast::Variable<Name>> : Returns<Name> {};

    template <ast::expression Function, ast::expression Argument>
    struct To_string::F<ast::Application<Function, Argument>> : Concat::template F<
        String<Character<'('>>,
        typename To_string::template F<Function>::Result,
        String<Character<' '>>,
        typename To_string::template F<Argument>::Result,
        String<Character<')'>>
    > {};

    template <string Parameter, ast::expression Body>
    struct To_string::F<ast::Abstraction<Parameter, Body>> : Concat::template F<
        String<Character<'\\'>>,
        Parameter,
        String<Character<'.'>>,
        typename To_string::template F<Body>::Result
    > {};

    template <string Name, ast::expression Expression>
    struct To_string::F<ast::Binding<Name, Expression>> : Concat::template F<
        Name,
        String<Character<' '>, Character<'='>, Character<' '>>,
        typename To_string::template F<Expression>::Result
    > {};

}