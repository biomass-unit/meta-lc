#pragma once

#include "meta.hpp"


namespace mlc::ast {

    namespace dtl {
        template <class>
        struct Is_expression : std::false_type {};
        template <class>
        struct Is_binding : std::false_type {};
        template <class>
        struct Is_environment : std::false_type {};
    }

    template <class T>
    concept expression = dtl::Is_expression<T>::value;

    template <class T>
    concept binding = dtl::Is_binding<T>::value;

    template <class T>
    concept environment = dtl::Is_environment<T>::value;


    template <string Name>
    struct Variable {};

    template <expression Function, expression Argument>
    struct Application {};

    template <string Parameter, expression Body>
    struct Abstraction {};


    template <string Name, expression Expression>
    struct Binding {};


    template <string Name>
    struct dtl::Is_expression<Variable<Name>> : std::true_type {};
    template <expression Function, expression Argument>
    struct dtl::Is_expression<Application<Function, Argument>> : std::true_type {};
    template <string Parameter, expression Body>
    struct dtl::Is_expression<Abstraction<Parameter, Body>> : std::true_type {};
    template <string Name, expression Expression>
    struct dtl::Is_binding<Binding<Name, Expression>> : std::true_type {};
    template <binding... Bindings>
    struct dtl::Is_environment<List<Bindings...>> : std::true_type {};


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