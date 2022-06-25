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
            template <class>
            struct F;
            template <class... Us>
            struct F<List<Us...>> : Returns<List<Ts..., Us...>> {};
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
    using To_string = dtl::To_string_helper<str, std::make_index_sequence<str.size>>::Result;

}