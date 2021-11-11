#ifndef sb_serialize_throwing_h
#define sb_serialize_throwing_h

#include <stdlib.h> // size_t

namespace sb {
namespace serialize {

struct any_invalid_serializer {};

template< typename T>
struct throwing
{
   struct invalid_serializer : public any_invalid_serializer {};

   /// @throws invalid_serializer
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};


} // namespace serialize
} // namespace sb


#include <sb/serialize/detail/throwing.ipp>

#endif
