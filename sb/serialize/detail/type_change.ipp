#ifndef sb_serialize_type_change_h
# error "Direct inclusion error."
#endif


#include <sb/serialize/compress_unsigned.h>

namespace sb {
namespace serialize {

// enum to a compressed value.

template<typename T>
inline size_t compress_enum<T>::size(const T& obj) {
   return compress_unsigned::size(static_cast<uint64_t>(obj));
}

template<typename T>
inline size_t compress_enum<T>::serialize(const T& obj, void* buf) {
   return compress_unsigned::serialize(static_cast<uint64_t>(obj), buf);
}

template<typename T>
inline size_t compress_enum<T>::deserialize(T& obj, const void* buf) {
   uint64_t temp=0;
   size_t rv = compress_unsigned::deserialize(temp, buf);
   obj = static_cast<T>(temp);
   return rv;
}


// Use this to serialize an enum to a fixed width uint: 16, 32, or 64.
// e.g. type_change<my_enum_t,uint32_t>

template<typename T, typename U, typename SERIALIZE>
inline size_t type_change<T,U,SERIALIZE>::size(const T& /*obj*/) {
   impl_t temp=0;
   return SERIALIZE::size(temp);
}

template<typename T, typename U, typename SERIALIZE>
inline size_t type_change<T,U,SERIALIZE>::serialize(const T& obj, void* buf) {
   return SERIALIZE::serialize( impl_t(obj), buf);
}

template<typename T, typename U, typename SERIALIZE>
inline size_t type_change<T,U,SERIALIZE>::deserialize(T& obj, const void* buf, size_t len) {
   impl_t temp;
   size_t rv = SERIALIZE::deserialize(temp, buf, len);
   obj = T(temp);
   return rv;
}

} // namespace serialize
} // namespace sb
