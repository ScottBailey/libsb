#ifndef sb_serialize_stl_h
# error "Direct inclusion error."
#endif




#include <sb/serialize/simple.h>

namespace sb {
namespace serialize {

// pair implementation
template<typename T, typename SER1, typename SER2>
inline size_t pair<T,SER1,SER2>::size(const T& obj) {
   return SER1::size(obj.first) + SER2::size(obj.second);
}
template<typename T, typename SER1, typename SER2>
inline size_t pair<T,SER1,SER2>::serialize(const T& obj, void* buf) {
   size_t rv = SER1::serialize( obj.first, buf);
   rv += SER2::serialize( obj.second, reinterpret_cast<uint8_t*>(buf)+rv);
   return rv;
}
template<typename T, typename SER1, typename SER2>
inline size_t pair<T,SER1,SER2>::deserialize(T& obj, const void* buf) {
   size_t rv = SER1::deserialize( obj.first, buf);
   rv += SER2::deserialize( obj.second, reinterpret_cast<const uint8_t*>(buf)+rv);
   return rv;
}


// any list with basic std behavior, including boost::devector.
template<typename T, typename SERIALIZE>
inline size_t list<T,SERIALIZE>::size(const T& obj) {
   size_t rv = compress_unsigned::size(obj.size());
   for(typename T::const_iterator i=obj.begin(); i != obj.end(); ++i)
      rv += SERIALIZE::size(*i);
   return rv;
}
template<typename T, typename SERIALIZE>
inline size_t list<T,SERIALIZE>::serialize(const T& obj, void* buf) {
   size_t rv = compress_unsigned::serialize(obj.size(),buf);
   uint8_t* pos = reinterpret_cast<uint8_t*>(buf); // position
   for(typename T::const_iterator i=obj.begin(); i != obj.end(); ++i) {
      rv += SERIALIZE::serialize(*i, pos+rv);
   }
   return rv;
}
template<typename T, typename SERIALIZE>
inline size_t list<T,SERIALIZE>::deserialize(T& obj, const void* buf) {
   obj.clear();
   size_t elements = 0;
   size_t rv = compress_unsigned::deserialize(elements, buf);
   const uint8_t* pos = reinterpret_cast<const uint8_t*>(buf); // position
   typename T::value_type temp;
   while(elements > 0) {
      rv += SERIALIZE::deserialize(temp, pos+rv);
      obj.push_back(temp);
      --elements;
   }
   return rv;
}


// stl map implementation
template<typename T, typename SER1, typename SER2>
inline size_t map<T,SER1,SER2>::size(const T& obj) {
   size_t rv = compress_unsigned::size(obj.size());
   for(typename T::const_iterator i=obj.begin(); i != obj.end(); ++i) {
      rv += SER1::size(i->first);
      rv += SER2::size(i->second);
   }
   return rv;
}

template<typename T, typename SER1, typename SER2>
inline size_t map<T,SER1,SER2>::serialize(const T& obj, void* buf) {
   size_t rv = compress_unsigned::serialize(obj.size(),buf);
   uint8_t* pos = reinterpret_cast<uint8_t*>(buf); // position
   for(typename T::const_iterator i=obj.begin(); i != obj.end(); ++i)
   {
      rv += SER1::serialize(i->first, pos+rv);
      rv += SER2::serialize(i->second,pos+rv);
   }
   return rv;
}
template<typename T, typename SER1, typename SER2>
inline size_t map<T,SER1,SER2>::deserialize(T& obj, const void* buf) {
   obj.clear();
   size_t elements = 0;
   size_t rv = compress_unsigned::deserialize(elements, buf);
   const uint8_t* pos = reinterpret_cast<const uint8_t*>(buf); // position
   typename T::key_type key_temp;
   typename T::mapped_type mapped_temp;
   while(elements > 0) {
      rv += SER1::deserialize(key_temp, pos+rv);
      rv += SER2::deserialize(mapped_temp, pos+rv);
      obj[key_temp] = mapped_temp;
      --elements;
   }
   return rv;
}


// std::multimap implementation
template<typename T, typename SER1, typename SER2>
inline size_t multimap<T,SER1,SER2>::size(const T& obj) {
   size_t rv = compress_unsigned::size(obj.size());
   for(typename T::const_iterator i=obj.begin(); i != obj.end(); ++i) {
      rv += SER1::size(i->first);
      rv += SER2::size(i->second);
   }
   return rv;
}

template<typename T, typename SER1, typename SER2>
inline size_t multimap<T,SER1,SER2>::serialize(const T& obj, void* buf) {
   size_t rv = compress_unsigned::serialize(obj.size(),buf);
   uint8_t* pos = reinterpret_cast<uint8_t*>(buf); // position
   for(typename T::const_iterator i=obj.begin(); i != obj.end(); ++i) {
      rv += SER1::serialize(i->first, pos+rv);
      rv += SER2::serialize(i->second,pos+rv);
   }
   return rv;
}

template<typename T, typename SER1, typename SER2>
inline size_t multimap<T,SER1,SER2>::deserialize(T& obj, const void* buf) {
   obj.clear();
   size_t elements = 0;
   size_t rv = compress_unsigned::deserialize(elements, buf);
   const uint8_t* pos = reinterpret_cast<const uint8_t*>(buf); // position
   typename T::key_type key_temp;
   typename T::mapped_type mapped_temp;
   while(elements > 0) {
      rv += SER1::deserialize(key_temp, pos+rv);
      rv += SER2::deserialize(mapped_temp, pos+rv);
      obj.insert( typename T::value_type(key_temp,mapped_temp) );
      --elements;
   }
   return rv;
}


// std::set implementation
template<typename T, typename SERIALIZE>
inline size_t set<T,SERIALIZE>::size(const T& obj) {
   size_t rv = compress_unsigned::size(obj.size());
   for(typename T::const_iterator i=obj.begin(); i != obj.end(); ++i)
      rv += SERIALIZE::size(*i);
   return rv;
}

template<typename T, typename SERIALIZE>
inline size_t set<T,SERIALIZE>::serialize(const T& obj, void* buf) {
   size_t rv = compress_unsigned::serialize(obj.size(),buf);
   uint8_t* pos = reinterpret_cast<uint8_t*>(buf); // position
   for(typename T::const_iterator i=obj.begin(); i != obj.end(); ++i) {
      rv += SERIALIZE::serialize(*i, pos+rv);
   }
   return rv;
}

template<typename T, typename SERIALIZE>
inline size_t set<T,SERIALIZE>::deserialize(T& obj, const void* buf) {
   obj.clear();
   size_t elements = 0;
   size_t rv = compress_unsigned::deserialize(elements, buf);
   const uint8_t* pos = reinterpret_cast<const uint8_t*>(buf); // position
   typename T::key_type key_temp;
   while(elements > 0) {
      rv += SERIALIZE::deserialize(key_temp, pos+rv);
      obj.insert(key_temp);
      --elements;
   }
   return rv;
}


}  // namespace serialize
} // namespace sb
