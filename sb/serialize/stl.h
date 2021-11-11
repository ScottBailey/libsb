#ifndef sb_serialize_stl_h
#define sb_serialize_stl_h

//
// This file contains simple serialize support for certain std types
//

namespace sb {
namespace serialize {

template<typename T, typename SER1=sb::serialize::simple<typename T::first_type>, typename SER2=sb::serialize::simple<typename T::second_type>>
struct pair
{
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};


// Deserialize any list with basic std behavior, including boost::devector.
template<typename T, typename SERIALIZE=sb::serialize::simple<typename T::value_type>>
struct list
{
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};


// Deserialize an stl map
template<typename T, typename SER1=sb::serialize::simple<typename T::key_type>, typename SER2=sb::serialize::simple<typename T::mapped_type>>
struct map
{
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};

// deserialize an stl multimap
template<typename T, typename SER1=sb::serialize::simple<typename T::key_type>, typename SER2=sb::serialize::simple<typename T::mapped_type>>
struct multimap
{
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};


// deserialize an stl set
template<typename T, typename SERIALIZE=sb::serialize::simple<typename T::key_type>>
struct set
{
   static size_t size(const T& obj);
   static size_t serialize(const T& obj, void* buf);
   static size_t deserialize(T& obj, const void* buf);
};


}  // namespace serialize
} // namespace sb


#include <sb/serialize/detail/stl.ipp>
#endif
