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

}