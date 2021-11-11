#ifndef sb_composition_element_h
# error "Direct inclusion error."
#endif

#include <sb/serialize/compress_unsigned.h>
#include <limits> // std::numeric_limit

namespace {

static_assert((sizeof(size_t) >= sizeof(uint64_t)), "size_t needs to be 64 bit or larger for DISMISS_SIZE to work cross platform.");
constexpr uint64_t DISMISS_SIZE = std::numeric_limits<uint64_t>::max();

} // anonymous namespace


namespace sb {
namespace composition {

//--- support functions ------------------------------------------------------------------------------------------------

inline std::pair<ELEMENT_TAG_T, size_t> element_info(const void* buffer) {

   const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

   std::pair<ELEMENT_TAG_T, size_t> rv{std::numeric_limits<ELEMENT_TAG_T>::max(), 0};

   // Deal with index.
   size_t consumed = serialize::compress_unsigned::deserialize(rv.first, p);

   // Payload size
   size_t payload_size = 0;
   consumed += serialize::compress_unsigned::deserialize(payload_size, p+consumed); // Payload size.

   // Convert payload data to element size, making sure to include the index tag and payload size as part of the element.
   // In the special case of DISMISS_SIZE being transmitted, payload size is actually 0.
   rv.second = consumed;
   if(payload_size != DISMISS_SIZE) {
      rv.second += payload_size;
   }

   return rv;
}


inline bool element_info(ELEMENT_TAG_T& index_value, size_t& element_size, const void* buffer, size_t sz) {

   const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

   // Deal with index.
   size_t need = serialize::compress_unsigned::need(p);
   if(!need || (need > sz))
      return false;
   size_t consumed = serialize::compress_unsigned::deserialize(index_value,p);

   // Ensure we have length for payload size.
   need = serialize::compress_unsigned::need(p+consumed);
   if(!need || ((need+consumed) > sz))
      return false;

   // Get payload size.
   size_t payload_size=0;
   consumed += serialize::compress_unsigned::deserialize(payload_size, p+consumed); // Payload size.

   // Convert payload data to element size, making sure to include the index tag and payload size as part of the element.
   // In the special case of DISMISS_SIZE being transmitted, payload size is actually 0.
   element_size = consumed;
   if(payload_size != DISMISS_SIZE) {
      element_size += payload_size;
   }

   return true;
}


//--- constructors/destructor ------------------------------------------------------------------------------------------

template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline element<INDEX,DATA_TYPE,SERIALIZER>::element()
   : m_control{control::invalid}
{
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline element<INDEX,DATA_TYPE,SERIALIZER>::element(const struct dismiss&)
   : m_control{control::dismissed}
{
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline element<INDEX,DATA_TYPE,SERIALIZER>::element(const value_type& value)
   : m_control{control::valid}
   , m_value{value}
{
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline element<INDEX,DATA_TYPE,SERIALIZER>::~element() = default;


//--- operators --------------------------------------------------------------------------------------------------------

#if defined(__llvm__) || defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wfloat-equal"
#endif

template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline bool element<INDEX,DATA_TYPE,SERIALIZER>::operator==(const typename element<INDEX,DATA_TYPE,SERIALIZER>::self_type& rhs) const {

   switch(m_control) {

      case control::invalid:
         return (rhs.m_control == control::invalid);

      case control::dismissed:
         return (rhs.m_control == control::dismissed);

      case control::valid:
         return (rhs.m_control == control::valid) && (m_value == rhs.m_value);

   }
   return false;
}

#if defined(__llvm__) || defined(__GNUC__)
# pragma GCC diagnostic pop
#endif


//--- alphabetic -------------------------------------------------------------------------------------------------------

template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline void element<INDEX,DATA_TYPE,SERIALIZER>::clear() {
   m_control = control::invalid;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline size_t element<INDEX,DATA_TYPE,SERIALIZER>::deserialize(const void* buffer) {

   const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

   // Double check that index matches while stepping over those bytes.
   ELEMENT_TAG_T temp = std::numeric_limits<ELEMENT_TAG_T>::max();
   size_t rv = serialize::compress_unsigned::deserialize(temp, p);
   if(!rv || (temp != INDEX)) {
      m_control = control::invalid;
      return 0;
   }

   // Load payload size and check for dismiss, otherwise ignoring the results.Step over payload size bytes, ignoring results.
   size_t payload_size=0;
   rv += serialize::compress_unsigned::deserialize(payload_size, p+rv);
   if(payload_size == DISMISS_SIZE) {
      m_control = control::dismissed;
      return rv;
   }

   rv += SERIALIZER::deserialize(m_value, p+rv);
   m_control = control::valid;
   return true;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline void element<INDEX,DATA_TYPE,SERIALIZER>::dismiss() {
   m_control = control::dismissed;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline const typename element<INDEX,DATA_TYPE,SERIALIZER>::value_type& element<INDEX,DATA_TYPE,SERIALIZER>::get() const {
   switch(m_control) {
      case control::invalid: throw element_invalid{};
      case control::dismissed: throw element_dismissed{};
      case control::valid:
         break;
   }
   return m_value;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline typename element<INDEX,DATA_TYPE,SERIALIZER>::value_type element<INDEX,DATA_TYPE,SERIALIZER>::get_default(const typename element<INDEX,DATA_TYPE,SERIALIZER>::value_type& default_value) const {
   if(m_control == control::valid)
      return m_value;
   return default_value;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline bool element<INDEX,DATA_TYPE,SERIALIZER>::is_dismissed() const {
   return m_control == control::dismissed;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline bool element<INDEX,DATA_TYPE,SERIALIZER>::is_special() const {
   return m_control != control::valid;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline bool element<INDEX,DATA_TYPE,SERIALIZER>::is_valid() const {
   return m_control == control::valid;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline size_t element<INDEX,DATA_TYPE,SERIALIZER>::serialize(void* buffer) const {

   uint8_t* p = reinterpret_cast<uint8_t*>(buffer);

   switch(m_control) {
      case control::invalid:
         return 0;

      case control::dismissed: {
         size_t rv = serialize::compress_unsigned::serialize(INDEX, p);
         rv += serialize::compress_unsigned::serialize(DISMISS_SIZE, p+rv);
         return rv;
      }

      case control::valid: {
         const size_t payload_size = SERIALIZER::size(m_value);

         size_t rv = serialize::compress_unsigned::serialize(INDEX, p);
         rv += serialize::compress_unsigned::serialize(payload_size, p+rv);
         rv += SERIALIZER::serialize(m_value, p+rv);
         return rv;
      }
   }

   return 0;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline size_t element<INDEX,DATA_TYPE,SERIALIZER>::serialized_size() const {

   switch(m_control) {
      case control::invalid:
         return 0;

      case control::dismissed:
         return serialize::compress_unsigned::size(INDEX) + serialize::compress_unsigned::size(DISMISS_SIZE);

      case control::valid: {
         const size_t payload_size = SERIALIZER::size(m_value);
         return serialize::compress_unsigned::size(INDEX) + serialize::compress_unsigned::size(payload_size) + payload_size;
      }
   }

   return 0;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline void element<INDEX,DATA_TYPE,SERIALIZER>::set(const value_type& value) {
   m_control = control::valid;
   m_value = value;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline control_t element<INDEX,DATA_TYPE,SERIALIZER>::state() const {
   return m_control;
}


template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER>
inline bool element<INDEX,DATA_TYPE,SERIALIZER>::try_get(value_type& value) const {
   if(is_special())
      return false;
   value = get();
   return true;
}



} // namespace composition
} // namespace sb
