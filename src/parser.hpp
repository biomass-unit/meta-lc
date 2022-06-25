#pragma once

#include "meta.hpp"


namespace mlc::parser {

    template <class T>
    concept character_predicate = requires {
        { T::template F<Character<' '>>::Result::value } -> std::convertible_to<bool>;
    };


    template <character Min, character Max> requires (Min::value < Max::value)
    struct Is_in_between {
        template <character C>
        struct F : Returns<std::bool_constant<Min::value <= C::value && C::value <= Max::value>> {};
    };

    template <character_predicate... Ps>
    struct Satisfies_one_of {
        template <character C>
        struct F : Returns<std::disjunction<typename Ps::template F<C>::Result...>> {};
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
    using Is_space = Is_one_of<Character<' '>, Character<'\n'>>;


    template <class, string>
    struct Success {};

    struct Null : Returns<Null> {};


    template <class Parser, string Input>
    using Parse = typename Parser::template Parse<Input>;

    template <class Parser, string Input>
    using Parse_result = Parse<Parser, Input>::Result;


    template <character_predicate P>
    struct PredP {
        template <string>
        struct Parse : Failure {};
        template <character C, character... Cs> requires P::template F<C>::Result::value
        struct Parse<String<C, Cs...>> : Returns<Success<C, String<Cs...>>> {};
    };

    template <character C>
    using CharP = PredP<Is_one_of<C>>;

    struct EndP {
        template <string>
        struct Parse : Returns<Success<Null, String<>>> {};
        template <character C, character... Cs>
        struct Parse<String<C, Cs...>> : Failure {};
    };


    template <class P1, class P2>
    struct Seq2P {
    private:
        template <class, class>
        struct Helper1 : Failure {};
        template <class R1, class R2, string I>
        struct Helper1<R1, Success<R2, I>> : Returns<Success<Pair<R1, R2>, I>> {};

        template <class>
        struct Helper2 : Failure {};
        template <class R, string I>
        struct Helper2<Success<R, I>> : Helper1<R, Parse_result<P2, I>> {};
    public:
        template <string I>
        struct Parse : Helper2<Parse_result<P1, I>> {};
    };


    template <class P, class... Ps>
    using SeqP = Fold_left<Adapt_template<Seq2P>>::template F<P, List<Ps...>>::Result;

}