#pragma once

#include "meta.hpp"


namespace mlc::parser {

    template <character Min, character Max> requires (Min::value < Max::value)
    struct Is_in_between {
        template <character C>
        struct F : Returns<std::bool_constant<Min::value <= C::value && C::value <= Max::value>> {};
    };

    template <class... Fs>
    struct Satisfies_one_of {
        template <character C>
        struct F : Returns<std::disjunction<typename Fs::template F<C>::Result...>> {};
    };

    template <character... Cs>
    struct Is_one_of {
        template <character C>
        struct F : Returns<std::disjunction<std::is_same<C, Cs>...>> {};
    };

    template <character... Cs>
    struct Is_not_one_of {
        template <character C>
        struct F : Returns<std::conjunction<std::negation<std::is_same<C, Cs>>...>> {};
    };

    using Is_digit = Is_in_between<Character<'0'>, Character<'9'>>;
    using Is_lower = Is_in_between<Character<'a'>, Character<'z'>>;
    using Is_upper = Is_in_between<Character<'A'>, Character<'Z'>>;
    using Is_alpha = Satisfies_one_of<Is_lower, Is_upper>;
    using Is_alnum = Satisfies_one_of<Is_alpha, Is_digit>;
    using Is_space = Is_one_of<mlc::Character<' '>, mlc::Character<'\n'>>;

}