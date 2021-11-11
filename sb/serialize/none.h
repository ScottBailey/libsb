#ifndef sb_serialize_none_h
#define sb_serialize_none_h

#include <stdlib.h> // size_t

namespace sb {
namespace serialize {


template< typename T>
struct none
{
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};


} // namespace serialize
} // namespace sb


#include <sb/serialize/detail/none.ipp>

#endif
