#ifndef sb_serialize_self_h
# error "Direct inclusion error."
#endif

#include <sb/serialize/simple.h>

namespace sb {
namespace serialize {


template<typename T>
inline size_t self<T>::size(const T& obj) {
   return obj.serialized_size();
}
template<typename T>
inline size_t self<T>::serialize(const T& obj, void* buf) {
   return obj.serialize(buf);
}
template<typename T>
inline size_t self<T>::deserialize(T& obj, const void* buf) {
   return obj.deserialize(buf);
}


} // namespace serialize
} // namespace sb
