#ifndef sb_serialize_self_h
#define sb_serialize_self_h

#include <sb/serialize/simple.h>

namespace sb {
namespace serialize {

// A self serializing base class useful for the self serialize template:
class self_serialize
{
public:
   virtual ~self_serialize()=default;
   /// @return bytes necessary for serialization
   virtual size_t serialized_size() const =0;
   /// @param buf pointer to the buffer that will receive the serialized object
   /// @param len size of the buffer
   /// @return bytes consumed in serialization
   virtual size_t serialize(void* buf) const =0;
   /// @param buf pointer to the buffer that contains the serialized object
   /// @param len size of the buffer
   /// @return bytes consumed in deserialization
   virtual size_t deserialize(const void* buf) =0;
};


// use this template to deserialize objects that conform to the self_serialize interface
template<typename T>
struct self
{
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};


} // namespace serialize
} // namespace sb


#include <sb/serialize/detail/self.ipp>
#endif
