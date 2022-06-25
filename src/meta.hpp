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


    template <char... cs>
    struct String {
        static constexpr Usize size = sizeof...(cs);

        struct Concat {
            template <class>
            struct F;
            template <char... other_cs>
            struct F<String<other_cs...>> : Returns<String<cs..., other_cs...>> {};
        };
    };


    namespace dtl {
        template <class>
        struct Is_list : std::false_type {};
        template <class... Ts>
        struct Is_list<List<Ts...>> : std::true_type {};

        template <class>
        struct Is_string : std::false_type {};
        template <char... cs>
        struct Is_string<String<cs...>> : std::true_type {};
    }


    template <class T>
    concept list = dtl::Is_list<T>::value;

    template <class T>
    concept string = dtl::Is_string<T>::value;

}