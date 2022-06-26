#pragma once

//#include "meta.hpp"
#include "parser.hpp"


namespace mlc {

    struct To_string {
        template <class>
        struct F;
    };


    template <string S>
    struct To_string::F<S> : Returns<S> {};

    template <character C>
    struct To_string::F<C> : Returns<String<C>> {};

    template <>
    struct To_string::F<List<>> : Returns<String<>> {};

    template <class T, class... Ts>
    struct To_string::F<List<T, Ts...>> :
        std::conditional_t<
            sizeof...(Ts) == 0,
            typename To_string::template F<T>::Result,
            typename To_string::template F<T>::Result::Concat::template F<String<Character<','>, Character<' '>>>::Result
        >::Concat::template F<typename F<List<Ts...>>::Result> {};

    template <Usize n>
    struct To_string::F<std::integral_constant<Usize, n>> :
        Integer_to_string::template F<std::integral_constant<Usize, n>> {};

    template <class T, string I>
    struct To_string::F<parser::Success<T, I>> : Concat::template F<
        Make_string<"Result: ">,
        typename To_string::F<T>::Result,
        Make_string<", with remaining: ">,
        I
    > {};

}