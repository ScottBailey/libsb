#ifndef sb_composition_element_h
#define sb_composition_element_h

#include <utility>              // std::pair
#include <sb/serialize.h>       // include the serializers

namespace sb {
namespace composition {

using ELEMENT_TAG_T = unsigned;

struct element_dismissed{}; /// exception
struct element_invalid{};   /// exception

enum class control : uint8_t {
   invalid = 0,
   valid = 1,
   dismissed = 2,
};
using control_t = control;


struct dismiss{}; /// Messaging type for the dismiss constructor.


/// Determine the index value and element size of a serialized element.
/// @param buffer  Source for element data. Must be valid!
/// @return std::pair<> containing the index_value and the element size.
std::pair<ELEMENT_TAG_T, size_t> element_info(const void* buffer);
/// Determine the index value and element size of a serialized element.
/// @param index_value  Return storage for the index value.
/// @param element_size  Return storage for the length of the element as serialized into buffer.
/// @param buffer  Source for element data. Must be valid!
/// @param sz  Size in bytes of data in buffer. Must be non-zero!
/// @return false is returned if buffer was too small to completely decode index_value and element_size. Otherwise true.
bool element_info(ELEMENT_TAG_T& index_value, size_t& element_size, const void* buffer, size_t sz);


/// Serializable element.
///
/// @note Serialization is implemented by writing the compressed index, the compressed payload size in bytes, then the serialized
/// data. Control values (i.e. `dismiss`) are signaled using very large 64 bit values in place of the payload size.
template<ELEMENT_TAG_T INDEX, typename DATA_TYPE, typename SERIALIZER=serialize::simple<DATA_TYPE>>
class element final {
public:
   using self_type = element;
   using tag_type = ELEMENT_TAG_T;
   using serializer = SERIALIZER;
   using value_type = DATA_TYPE; /// Convinience type.

   /// Construct an element in the invalid state.
   element();
   /// Construct an element in the dismissed state.
   element(const struct dismiss&);
   /// Construct an element in the valid state with value.
   /// @param value  The value to store.
   element(const value_type& value);

   /// destructor
   ~element();

   /// Returns the type.
   static constexpr tag_type get_type() { return INDEX; }


   /// Compare elements.
   bool operator==(const self_type& rhs) const;


   /// @return true if this is a valid non-dismissed element.
   bool is_valid() const;
   /// @return true if this element is dismissed.
   bool is_dismissed() const;
   /// @return false if this is a valid element, true if invalid or dismissed.
   bool is_special() const;
   /// @return The element's internal state.
   control_t state() const;

   /// Get the value stored in this element (or throw).
   /// @return A const reference to the value.
   /// @throws if element_dismissed or element_invalid is_special() would return true.
   const value_type& get() const;
   /// Get an element's value if it's valid, otherwise return the default value.
   /// @param default_value  The default value. Returned if the element would return true for is_special().
   /// @return Either the elment's value or the passed in default value.
   value_type get_default(const value_type& default_value) const;
   /// Non throwing get.
   /// @param value  Return storage for the element. Only set if return is true.
   /// @return false if is_special would return true; otherwise, true and value is set
   bool try_get(value_type& value) const;

   /// Convert to an invalid element.
   void clear();
   /// Convert to a dismissed element.
   void dismiss();
   /// Convert to a valid element with value.
   void set(const value_type& value);

   /// Report serialized size of this object.
   /// @return the size necessary to serialize this element
   size_t serialized_size() const;
   /// Serialize this data.
   /// @param buffer  The data buffer to serialize this object into. It must be valid and sized to contain at minimum serialized_size() bytes.
   /// @return The number of buffer bytes consumed in the serialization.
   size_t serialize(void* buffer) const;
   /// Deserialize a buffer into this object.
   /// @param buffer  Data buffer to deserialize from. It must contain adequate bytes. `element_info()` may be used to determine size.
   /// @return zero indicates an error; otherwise, the bytes cosumed by the deserialization process are returned.
   size_t deserialize(const void* buffer);

private:

   control m_control;
   value_type m_value;
};



} // namespace composition
} // namespace sb

#include <sb/composition/detail/element.ipp>

#endif
