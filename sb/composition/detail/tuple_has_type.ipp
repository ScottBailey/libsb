#ifndef tuple_has_type_ipp
#define tuple_has_type_ipp



/// Compile time test to ensure TUPLE contains T.
/// @return true if TUPLE has type T.
template <typename T, typename TUPLE>
constexpr void tuple_ensure_type();

/// Compile time test to determine if TUPLE contains T.
/// @return true if TUPLE has type T.
template <typename T, typename TUPLE>
constexpr bool tuple_has_type();

/// Exception struct
/// thrown from tuple_maybe_get<>()
struct tuple_unknown_type { };

/// Used in conjunction with `tuple_has_type<T>()`, return a const ref to the type in TUPLE t or throw.
/// @return const ref to the type T contained in t, if it exists; otherwise throw.
/// @throw tuple_unknown_type if t doesn't contain T.
/*
template <typename T, typename TUPLE>
const T& tuple_maybe_get(const TUPLE& t);
*/
/// Used in conjunction with `tuple_has_type<T>()`, return a ref to the type in TUPLE t or throw.
/// @return const ref to the type T contained in t, if it exists; otherwise throw.
/// @throw tuple_unknown_type if t doesn't contain T.
/*
template <typename T, typename TUPLE>
T& tuple_maybe_get(TUPLE& t);
*/




namespace detail {

template <typename T, typename TUPLE>
struct has_type;

template <typename T>
struct has_type<T, std::tuple<>> : std::false_type {};

template <typename T, typename U, typename... Ts>
struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {};

template <typename T, typename... Ts>
struct has_type<T, std::tuple<T, Ts...>> : std::true_type {};

} // namespace detail


template <typename T, typename TUPLE>
constexpr inline void tuple_ensure_type() {
   static_assert( std::is_same<typename detail::has_type<T, TUPLE>::type, std::true_type>::value, "TUPLE does not have a member of type T.");
}


template <typename T, typename TUPLE>
constexpr inline bool tuple_has_type() {
   return std::is_same<typename detail::has_type<T, TUPLE>::type, std::true_type>::value;
}


template <typename T, typename TUPLE>
inline typename std::enable_if<!std::is_same<typename detail::has_type<T, TUPLE>::type, std::true_type>::value, const T&>::type tuple_maybe_get(const TUPLE&) {
   throw tuple_unknown_type{};
}

template <typename T, typename TUPLE>
inline typename std::enable_if<std::is_same<typename detail::has_type<T, TUPLE>::type, std::true_type>::value, const T&>::type tuple_maybe_get(const TUPLE& t) {
   return std::get<T>(t);
}

template <typename T, typename TUPLE>
inline typename std::enable_if<!std::is_same<typename detail::has_type<T, TUPLE>::type, std::true_type>::value, T&>::type tuple_maybe_get(TUPLE&) {
   throw tuple_unknown_type{};
}

template <typename T, typename TUPLE>
inline typename std::enable_if<std::is_same<typename detail::has_type<T, TUPLE>::type, std::true_type>::value, T&>::type tuple_maybe_get(TUPLE& t) {
   return std::get<T>(t);
}


#endif
