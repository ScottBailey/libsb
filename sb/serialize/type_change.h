#ifndef sb_serialize_type_change_h
#define sb_serialize_type_change_h

namespace sb {
namespace serialize {

// Use this to serialize an enum to a compressed value.
template<typename T>
struct compress_enum {
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};


// Use this to serialize an enum to a fixed width uint: 16, 32, or 64.
// e.g. type_change<my_enum_t,uint32_t>
template<typename T, typename U, typename SERIALIZE = sb::serialize::simple<U> >
struct type_change {

   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf, size_t len);

   using impl_t = U;
   static_assert(sizeof(T) > sizeof(U), "Error: bad type_change!");
};

} // namespace serialize
} // namespace sb


#include <sb/serialize/detail/type_change.ipp>

#endif
