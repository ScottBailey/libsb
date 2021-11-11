#ifndef sb_serialize_simple_h
#define sb_serialize_simple_h

#include <stdint.h>             // various width integers
#include <stddef.h>             // size_t

namespace sb {
namespace serialize {

template<typename T>
struct simple // aka default serialize
{
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};

} // namespace serializer
}  // namespace sb


// Includes implementation for:
//   default, should handle most scalars
//   bool
//   std::string

#include <sb/serialize/detail/simple.ipp>

#endif
