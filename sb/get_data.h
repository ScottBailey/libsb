#ifndef sb_get_data_h
#define sb_get_data_h

namespace sb {

/// call either data() or get() of t (prefering data)
/// @return the result of either data() or get() as a pointer
template<typename T>
auto* get_data(T& t);

/// call either data() or get() of t ( prefering data)
/// @return the result of either data() or get() as a const pointer
template<typename T>
const auto* get_data(const T& t);

} // namespace sb

#include <sb/detail/get_data.ipp>

#endif
