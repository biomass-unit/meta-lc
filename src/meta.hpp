#pragma once

#include <source_location>
#include <type_traits>
#include <concepts>
#include <cstddef>


namespace mlc {

    // A metafunction returns a type T by inheriting from mlc::Returns<T>.
    template <class T>
    struct Returns {
        using Result = T;
    };

    // Metafunctions that fail to produce a type can inherit from mlc::Failure.
    // This prevents cryptic error messages about missing ::Result types.
    struct Failure : Returns<Failure> {};


    // Converts a regular template into a conforming metafunction.
    template <template <class...> class G>
    struct Adapt_template {
        template <class... Ts>
        struct F : Returns<G<Ts...>> {};
    };


    namespace dtl {
        template <class>
        struct Is_list : std::false_type {};
        template <class>
        struct Is_string : std::false_type {};
        template <class>
        struct Is_character : std::false_type {};
    }

    template <class T>
    concept list = dtl::Is_list<T>::value;

    template <class T>
    concept string = dtl::Is_string<T>::value;

    template <class T>
    concept character = dtl::Is_character<T>::value;


    // Used for delaying template instantiation
    template <class T>
    concept delayer = std::is_void_v<T>;


    template <class... Ts>
    struct List {
        static constexpr std::size_t size = sizeof...(Ts);

        struct Concat {
            template <list...>
            struct F;
            template <>
            struct F<> : Returns<List> {};
            template <class... Us, list... Ls>
            struct F<List<Us...>, Ls...> : List<Ts..., Us...>::Concat::template F<Ls...> {};
        };

        struct Map {
            template <class G>
            struct F : Returns<List<typename G::template F<Ts>::Result...>> {};
        };
    };


    template <unsigned char c>
    struct Character : std::integral_constant<decltype(c), c> {};


    template <character... Cs>
    using String = List<Cs...>;


    namespace dtl {
        template <class... Ts>
        struct Is_list<List<Ts...>> : std::true_type {};
        template <character... Cs>
        struct Is_string<String<Cs...>> : std::true_type {};
        template <unsigned char c>
        struct Is_character<Character<c>> : std::true_type {};
    }


    template <class, class>
    struct Pair {};


    template <class G>
    struct Adapt_pair_argument {
        template <class>
        struct F;
        template <class Fst, class Snd>
        struct F<Pair<Fst, Snd>> : G::template F<Fst, Snd> {};
    };


    template <class G>
    struct Flip {
        template <class T, class U>
        struct F : G::template F<U, T> {};
    };


    struct Concat {
        template <list L, list... Ls>
        struct F : L::Concat::template F<Ls...> {};
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


    struct Compose2 {
        template <class G, class H>
        struct F {
            struct Result {
                template <class... Ts>
                struct F : G::template F<typename H::template F<Ts...>::Result> {};
            };
        };
    };

    struct Compose {
        template <class G, class... Hs>
        struct F : Fold_left<Compose2>::template F<G, List<Hs...>> {};
    };


    struct String_to_integer {
    private:
        template <std::size_t accumulator, string>
        struct Helper : Returns<std::integral_constant<std::size_t, accumulator>> {};
        template <std::size_t accumulator, character C, character... Cs>
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
        template <std::size_t n>
        struct Helper : Helper<n / 10>::Result::Concat::template F<String<Character<'0' + (n % 10)>>> {};
        template <>
        struct Helper<0> : Returns<String<>> {};
    public:
        template <class>
        struct F;
        template <std::size_t n>
        struct F<std::integral_constant<std::size_t, n>> : Helper<n> {};
        template <>
        struct F<std::integral_constant<std::size_t, 0>> : Returns<String<Character<'0'>>> {};
    };


    namespace dtl {
        template <std::size_t n>
        struct Metastring {
            unsigned char characters[n];

            static constexpr std::size_t size = n;

            consteval Metastring(char const* const str) {
                for (std::size_t i = 0; i != n; ++i) {
                    characters[i] = static_cast<unsigned char>(str[i]);
                }
            }
        };

        template <std::size_t n>
        Metastring(char const(&)[n]) -> Metastring<n - 1>;

        template <Metastring, class>
        struct Make_string_helper;
        template <Metastring str, std::size_t... is>
        struct Make_string_helper<str, std::index_sequence<is...>> :
            Returns<String<Character<str.characters[is]>...>> {};
    }


    template <dtl::Metastring str>
    using Make_string = dtl::Make_string_helper<str, std::make_index_sequence<str.size>>::Result;

}