#pragma once

#include "parser.hpp"
#include "ast.hpp"


namespace mlc::parser {

    using WsP = IgnoreP<Zero_or_moreP<PredP<Is_space>>>;

    template <class P>
    using WsdP = SeqP<WsP, P, WsP>;

    template <class P>
    using ParenP = SeqP<IgnoreP<CharP<Character<'('>>>, P, IgnoreP<CharP<Character<')'>>>>;

    using NameP = One_or_moreP<PredP<Is_alpha>>;


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


    DECLARE_PARSER(ExprP);


    using VarP = MapP<NameP, Adapt_template<ast::Variable>>;

    template <delayer T>
    struct AbsP : MapP<
        SeqP<
            IgnoreP<CharP<Character<'\\'>>>,
            One_or_moreP<WsdP<NameP>>,
            IgnoreP<CharP<Character<'.'>>>,
            ExprP<T>
        >,
        Adapt_pair_argument<Flip<Fold_right<Adapt_template<ast::Abstraction>>>>
    > {};

    template <delayer T>
    using Simple_exprP = WsdP<OrP<VarP, AbsP<T>, IntP, ParenP<ExprP<T>>>>;


    DEFINE_PARSER(T, ExprP, MapP<
        Seq2P<Simple_exprP<T>, Zero_or_moreP<Simple_exprP<T>>>,
        Adapt_pair_argument<Fold_left<Adapt_template<ast::Application>>>
    >);


    struct Parse_expression {
        template <string I>
        struct F : Parse_result<ExprP<void>, I> {};
    };

}