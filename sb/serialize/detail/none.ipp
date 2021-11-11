#ifndef sb_serialize_none_h
# error "Direct inclusion error."
#endif


#include <stdlib.h> // size_t

namespace sb {
namespace serialize {


template<typename T>
inline size_t none<T>::size(const T& /*obj*/) {
   return 0;
}

template<typename T>
inline size_t none<T>::serialize(const T& /*obj*/, void* /*buf*/) {
   return 0;
}

template<typename T>
inline size_t none<T>::deserialize(T& /*obj*/, const void* /*buf*/) {
      return 0;
}


} // namespace serialize
} // namespace sb
