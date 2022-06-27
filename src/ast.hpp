#pragma once

#include "meta.hpp"


namespace mlc::ast {

    namespace dtl {
        template <class>
        struct Is_expression : std::false_type {};
    }


    template <class T>
    concept expression = dtl::Is_expression<T>::value;

    template <string Name>
    struct Variable {};

    template <expression Function, expression Argument>
    struct Application {};

    template <string Parameter, expression Body>
    struct Abstraction {};


    template <string Name>
    struct dtl::Is_expression<Variable<Name>> : std::true_type {};
    template <expression Function, expression Argument>
    struct dtl::Is_expression<Application<Function, Argument>> : std::true_type {};
    template <string Parameter, expression Body>
    struct dtl::Is_expression<Abstraction<Parameter, Body>> : std::true_type {};

}