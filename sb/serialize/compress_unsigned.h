#ifndef sb_serialize_compress_unsigned_h
#define sb_serialize_compress_unsigned_h

namespace sb {
namespace serialize {
namespace compress_unsigned {

/// Determine how many bytes scalar T with value n requires.
/// @param n  The value to determine the size of.
/// @return The size necessary to serialize value n of type T.
template<typename T>
size_t size(T n);

/// Compress and serialize scalar T's value of n into buffer.
/// @param n  The value to serialize into buffer.
/// @param buffer  The buffer to serialize n into.
/// @return The number of bytes of buffer consumed.
template<typename T>
size_t serialize(T n, void* buffer);

/// Deserialize a compressed scalar of type T.
/// @param n  Storage for the result of deserialization.
/// @param buffer  The serialized data source.
/// @return The number of bytes of buff consumed by this operation. 0 indicates a failure state.
template<typename T>
size_t deserialize(T& n, const void* buffer);

/// Report the number of bytes needed to deserialize a compressed value.
/// @param buffer  The serialized data source.
/// @return The total number of bytes needed to deserialize the compressed value.
size_t need(const void* buffer);


} // namespace compress_unsigned
} // namespace serialize
} // namespace sb

#include <sb/serialize/detail/compress_unsigned.ipp>

#endif
