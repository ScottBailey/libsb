#ifndef sb_serialize_throwing_h
# error "Direct inclusion error."
#endif


#include <stdlib.h> // size_t

namespace sb {
namespace serialize {


template<typename T>
inline size_t throwing<T>::size(const T& /*obj*/) {
   throw invalid_serializer{};
   return 0;
}

template<typename T>
inline size_t throwing<T>::serialize(const T& /*obj*/, void* /*buf*/) {
   throw invalid_serializer{};
   return 0;
}

template<typename T>
inline size_t throwing<T>::deserialize(T& /*obj*/, const void* /*buf*/) {
   throw invalid_serializer{};
   return 0;
}


} // namespace serialize
} // namespace sb
