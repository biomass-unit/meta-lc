#include <source_location>
#include <type_traits>
#include <concepts>
#include <cstddef>


namespace mlc {

    using Usize = std::size_t;


    template <class T>
    struct Returns {
        using Result = T;
    };

    struct Failure : Returns<Failure> {};


    template <class... Ts>
    struct List {
        static constexpr Usize size = sizeof...(Ts);

        struct Concat {
            template <class...>
            struct F;
            template <>
            struct F<> : Returns<List> {};
            template <class... Us, class... Ls>
            struct F<List<Us...>, Ls...> : List<Ts..., Us...>::Concat::template F<Ls...> {};
        };
    };


    template <class Fst, class Snd>
    struct Pair {
        using First  = Fst;
        using Second = Snd;
    };


    template <unsigned char c>
    struct Character : std::integral_constant<decltype(c), c> {};


    namespace dtl {
        template <class>
        struct Is_character : std::false_type {};
        template <unsigned char c>
        struct Is_character<Character<c>> : std::true_type {};
    }


    template <class T>
    concept character = dtl::Is_character<T>::value;


    template <character... Cs>
    using String = List<Cs...>;


    namespace dtl {
        template <class>
        struct Is_list : std::false_type {};
        template <class... Ts>
        struct Is_list<List<Ts...>> : std::true_type {};

        template <class>
        struct Is_string : std::false_type {};
        template <class... Cs>
        struct Is_string<String<Cs...>> : std::true_type {};
    }


    template <class T>
    concept list = dtl::Is_list<T>::value;

    template <class T>
    concept string = dtl::Is_string<T>::value;


    struct Concat {
        template <list L, list... Ls>
        struct F : L::Concat::template F<Ls...> {};
    };


    namespace dtl {
        template <Usize n>
        struct Metastring {
            unsigned char characters[n];

            static constexpr Usize size = n;

            consteval Metastring(char const* const str) {
                for (Usize i = 0; i != n; ++i) {
                    characters[i] = static_cast<unsigned char>(str[i]);
                }
            }
        };

        template <Usize n>
        Metastring(char const(&)[n]) -> Metastring<n - 1>;

        template <Metastring, class>
        struct To_string_helper;
        template <Metastring str, Usize... is>
        struct To_string_helper<str, std::index_sequence<is...>> :
            Returns<String<Character<str.characters[is]>...>> {};
    }


    template <dtl::Metastring str>
    using Make_string = dtl::To_string_helper<str, std::make_index_sequence<str.size>>::Result;


    template <template <class...> class G>
    struct Adapt_template {
        template <class... Ts>
        struct F : Returns<G<Ts...>> {};
    };


    template <class G>
    struct Fold_left {
        template <class I, list>
        struct F : Returns<I> {};
        template <class I, class T, class... Ts>
        struct F<I, List<T, Ts...>> :
            F<typename G::template F<I, T>::Result, List<Ts...>> {};
    };

    template <class G>
    struct Fold_right {
        template <class I, list>
        struct F : Returns<I> {};
        template <class I, class T, class... Ts>
        struct F<I, List<T, Ts...>> :
            G::template F<T, typename F<I, List<Ts...>>::Result> {};
    };


    struct String_to_integer {
    private:
        template <Usize accumulator, string>
        struct Helper : Returns<std::integral_constant<Usize, accumulator>> {};

        template <Usize accumulator, character C, character... Cs>
        struct Helper<accumulator, String<C, Cs...>> :
            Helper<accumulator * 10 + (C::value - '0'), String<Cs...>> {};
    public:
        template <string>
        struct F : Failure {};
        template <string S> requires (S::size != 0)
        struct F<S> : Helper<0, S> {};
    };

    struct Integer_to_string {
    private:
        template <Usize n>
        struct Helper : Helper<n / 10>::Result::Concat::template F<String<Character<'0' + (n % 10)>>> {};
        template <>
        struct Helper<0> : Returns<String<>> {};
    public:
        template <class>
        struct F;
        template <Usize n>
        struct F<std::integral_constant<Usize, n>> : Helper<n> {};
        template <>
        struct F<std::integral_constant<Usize, 0>> : Returns<String<Character<'0'>>> {};
    };

}