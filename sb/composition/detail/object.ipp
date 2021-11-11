#ifndef sb_composition_object_h
# error "Direct inclusion error."
#endif

#include <sb/serialize/compress_unsigned.h>



namespace sb {
namespace composition {

namespace {
# include <sb/composition/detail/tuple_print.ipp>
# include <sb/composition/detail/tuple_valid.ipp>
# include <sb/composition/detail/tuple_deserialize.ipp>
# include <sb/composition/detail/tuple_has_type.ipp>
}

//--- support functions ------------------------------------------------------------------------------------------------

inline bool object_info(OBJECT_TAG_T& tag, size_t& object_size, const void* buffer, size_t sz) {

   const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

   if(!sz)
      return false;

   // Deal with tag.
   size_t need = serialize::compress_unsigned::need(p);
   if(!need || (need > sz))
      return false;
   size_t consumed = serialize::compress_unsigned::deserialize(tag,p);

   // Ensure we have length for payload size.
   need = serialize::compress_unsigned::need(p+consumed);
   if(!need || ((need+consumed) > sz))
      return false;

   // Get payload size.
   size_t payload_size=0;
   consumed += serialize::compress_unsigned::deserialize(payload_size, p+consumed); // Payload size.

   // Convert payload data to object size, making sure to include the tag and payload size as part of the object.
   object_size = consumed + payload_size;
   return true;
}


template<typename T>
inline bool object_info(OBJECT_TAG_T& tag_, size_t& object_size, T& accum) {

   if(accum.empty())
      return false;

   // Start with tag.

   // Get the first message in the accumulator.
   auto msg = accum.front();
   const uint8_t* p = reinterpret_cast<const uint8_t*>(msg.data());

   // Figure out how much data we need for the tag plus the control byte of the payload size. Return if accum lacks the bytes.
   size_t need = 1 + serialize::compress_unsigned::need(p);
   if(!need || (need > accum.size()))
      return false;

   // Ensure msg and p contain enough data, then decode the tag.
   if(need > msg.size()) {
      // If msg changes, p needs to change!
      msg = accum.front(need);
      p = reinterpret_cast<const uint8_t*>(msg.data());
   }
   size_t consumed = serialize::compress_unsigned::deserialize(tag_,p);

   // Move on to payload size.

   // Find the size of the payload field. In the code above (`need = 1 +...`), we ensured we had adequate length in msg to decode
   // this control byte.
   size_t temp = serialize::compress_unsigned::need(p+consumed);
   if(!temp)
      return false;
   need = temp + consumed;
   if(need > accum.size())
      return false;

   // Ensure msg and p contain enough data, then decode payload size.
   if(need > msg.size()) {
      // If msg changes, p needs to change!
      msg = accum.front(need);
      p = reinterpret_cast<const uint8_t*>(msg.data());
   }

   // Get payload size.
   size_t payload_size=0;
   consumed += serialize::compress_unsigned::deserialize(payload_size, p+consumed); // Payload size.

   // Convert payload data to object size, making sure to include the tag and payload size as part of the object.
   object_size = consumed + payload_size;
   return true;
}



//--- constructors/destructor ------------------------------------------------------------------------------------------

template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::object(typename object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::deserialization_flag) {
   // Construct the object for later population via deserialization.
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename... Ts>
inline object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::object(const Ts&... ts) {
   static_assert( (sizeof...(ts) == std::tuple_size<typename REQUIRED_ELEMENTS::tuple>{}), "Argument list doesn't match required list.");
   construct<0>(ts...);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::~object() {
}


//--- operators --------------------------------------------------------------------------------------------------------

template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline bool object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::operator==(const typename object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::self_type& rhs) const {
   return m_data == rhs.m_data;
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline bool object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::operator!=(const typename object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::self_type& rhs) const {
   return m_data != rhs.m_data;
}


//--- alphabetic -------------------------------------------------------------------------------------------------------

template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline void object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::clear(){

   tuple_ensure_type<E,optional_t>();

   std::get<E>(m_data).clear();
}

template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<size_t N, typename T, typename... Ts>
inline void object<TAG, REQUIRED_ELEMENTS, OPTIONAL_ELEMENTS>::construct(const T& t, const Ts&... ts) {
   std::get<N>(m_data).set(t);
   construct<N+1>(ts...);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<size_t N, typename T>
inline void object<TAG, REQUIRED_ELEMENTS, OPTIONAL_ELEMENTS>::construct(const T& t) {
   std::get<N>(m_data).set(t);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<size_t N>
inline void object<TAG, REQUIRED_ELEMENTS, OPTIONAL_ELEMENTS>::construct() {
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
typename object<TAG, REQUIRED_ELEMENTS, OPTIONAL_ELEMENTS>::ptr object<TAG, REQUIRED_ELEMENTS, OPTIONAL_ELEMENTS>::deserialize(const void* buffer) {

   const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

   // Double check that tag matches while stepping over those bytes.
   OBJECT_TAG_T temp = std::numeric_limits<OBJECT_TAG_T>::max();
   size_t consumed = serialize::compress_unsigned::deserialize(temp, p);
   if(!consumed || (temp != TAG)) {
      return ptr{};
   }

   // Step over payload size bytes, ignoring results.
   size_t payload_size=0;
   consumed += serialize::compress_unsigned::deserialize(payload_size, p+consumed);

   // Create an object using the special deserialization flag.
   ptr rv = std::make_shared<self_type>(deserialization_flag{});

   // Iterate over the serialized elements.
   const uint8_t* end = p + consumed + payload_size;
   while(p+consumed < end) {
      const uint8_t* pos = p + consumed;

      // Determine tag and object size for the next element.
      const auto[tag_, element_size] = element_info(pos);
      // Deserialize the element, print to stderr if the element was unknown.
      if(!tuple_deserialize(tag_, pos, rv->m_data)) {
         std::cerr
            << __FILE__ << "(" << __LINE__ << "): "<< __func__ << "() - "
            << "unknown element with tag: " << unsigned(tag_) << ", size: " <<  element_size
            << std::endl
            ;
      }

      consumed += element_size;
   }

   return rv;
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline void object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::dismiss() {

   tuple_ensure_type<E,optional_t>();

   std::get<E>(m_data).dismiss();
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline constexpr void object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::ensure_element() {
   tuple_ensure_type<E,data_t>();
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline const typename E::value_type& object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::get() const {
   return std::get<E>(m_data).get();
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline typename E::value_type object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::get_default(const typename E::value_type& default_value) const {
   return std::get<E>(m_data).get_default(default_value);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
const E& object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::get_element() const {
   return std::get<E>(m_data);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
const E& object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::get_element_maybe() const {
   if(!has_element<E>())
      throw unknown_element{};
   return tuple_maybe_get<E>(m_data);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline typename object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::tag_type object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::get_tag() const {
   return TAG;
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline bool object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::has_valid_optional() const {
   constexpr size_t i = std::tuple_size<typename REQUIRED_ELEMENTS::tuple>{};
   constexpr size_t max = std::tuple_size<typename OPTIONAL_ELEMENTS::tuple>{} + i;
   return tuple_any_valid<max,i>(m_data);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline constexpr bool object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::has_element() {
   return tuple_has_type<E,data_t>();
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline bool object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::is_dismissed() const {
   return std::get<E>(m_data).is_dismissed();
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline bool object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::is_special() const {
   return std::get<E>(m_data).is_special();
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline bool object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::is_valid() const {
   return std::get<E>(m_data).is_valid();
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline void object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::print(std::ostream& os) const {
   tuple_print(os,m_data);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline bool object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::required_valid() const {
   return tuple_valid<std::tuple_size<typename REQUIRED_ELEMENTS::tuple>{}>(m_data);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline size_t object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::serialize(void* buffer) const {

   uint8_t* p = reinterpret_cast<uint8_t*>(buffer);

   // Get the size of the payload.
   size_t obj_size = 0;
   std::apply([&obj_size](auto&&... args) {((obj_size += args.serialized_size()), ...);}, m_data);

   // Serialize in the index and the payload size.
   size_t rv = serialize::compress_unsigned::serialize(TAG,p);
   rv += serialize::compress_unsigned::serialize(obj_size,p+rv);

   // Serialize the objects one at at time!
   std::apply([&](auto&&... args) {(
       (rv += args.serialize(p+rv)),
               ...);}, m_data);

   return rv;
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
inline size_t object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::serialized_size() const {

   size_t obj_size = 0;
   std::apply([&obj_size](auto&&... args) {(
       (obj_size += args.serialized_size()),
               ...);}, m_data);
   return serialize::compress_unsigned::size(TAG) + serialize::compress_unsigned::size(obj_size) + obj_size;
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline void object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::set(const typename E::value_type& v) {
   std::get<E>(m_data).set(v);
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline void object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::set_element(const E& el) {
   std::get<E>(m_data) = el;
}


template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline void object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::set_element_maybe(const E& el) {
   if(!has_element<E>())
      throw unknown_element{};
   tuple_maybe_get<E>(m_data) = el;
}

template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline control_t object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::state() const {
   return std::get<E>(m_data).state();
}

template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
template<typename E>
inline bool object<TAG,REQUIRED_ELEMENTS,OPTIONAL_ELEMENTS>::try_get(typename E::value_type& val) const {
   return std::get<E>(m_data).try_get(val);
}


} // namespace composition
} // namespace sb
