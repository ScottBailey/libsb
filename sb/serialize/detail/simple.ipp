#ifndef sb_serialize_simple_h
# error "Direct inclusion error."
#endif

#include <sb/serialize/compress_unsigned.h>
#include <sb/endian.h>          // swap_net<>()
#include <string>

namespace sb {
namespace serialize {

// Default simple implementation.
template<typename T>
inline size_t simple<T>::size(const T& /*obj*/) {
   return sizeof(T);
}

template<typename T>
size_t simple<T>::serialize(const T& obj, void* buf) {
   size_t sz=size(obj);
   *reinterpret_cast<T*>(buf) = endian::swap_net(obj);
   return sz;
}

template<typename T>
inline size_t simple<T>::deserialize(T& obj, const void* buf) {
   size_t sz=size(obj);
   obj = endian::swap_net( *reinterpret_cast<const T*>(buf) );
   return sz;
}


// Bool
template<>
struct simple<bool>
{
   using impl_t = uint8_t;
   static inline size_t size(const bool& /*obj*/) {
      return sizeof(impl_t);
   }
   static inline size_t serialize(const bool& obj, void* buf) {
      return simple<impl_t>::serialize( impl_t(obj), buf);
   }
   static inline size_t deserialize(bool& obj, const void* buf) {
      impl_t temp;
      size_t rv = simple<impl_t>::deserialize(temp, buf);
      obj = (temp)?(true):(false);
      return rv;
   }
};


template<>
struct simple<std::string>
{
   static inline size_t size(const std::string& obj) {
      size_t sz = obj.size();
      return compress_unsigned::size(sz) + obj.size();
   }
   static inline size_t serialize(const std::string& obj, void* buf) {
      uint8_t* pos = reinterpret_cast<uint8_t*>(buf);
      size_t elements = obj.size();
      size_t consumed = compress_unsigned::serialize(elements,pos);
      memcpy( pos+consumed, obj.data(), elements);
      return consumed + elements;
   }
   static inline size_t deserialize(std::string& obj, const void* buf)
   {
      const uint8_t* pos = reinterpret_cast<const uint8_t*>(buf);
      size_t elements = 0;
      const size_t consumed = compress_unsigned::deserialize(elements,pos);
      pos += consumed;
      obj.clear();
      obj.append( reinterpret_cast<const char*>(pos), elements );
      return consumed + elements;
   }
};

} // namespace serializer
}  // namespace sb
