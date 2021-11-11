#ifndef sb_composition_object_h
#define sb_composition_object_h

#include <sb/composition/base.h>
#include <sb/composition/element.h>
#include <sb/devector.h>
#include <tuple>


namespace sb {
namespace composition {


/// Exception for get_element<T>().
struct unknown_element {};


/// Determine the tag and object size of a serialized element.
/// @param tag  Return storage for the tag.
/// @param object_size  Return storage for the length of the object as serialized into buffer.
/// @param buffer  Source for object data. Must be valid!
/// @param sz  Size in bytes of data in buffer. Must be non-zero!
/// @return false is returned if buffer was too small to completely decode tag_value and object_size. Otherwise true.
bool object_info(OBJECT_TAG_T& tag, size_t& object_size, const void* buffer, size_t sz);
/// Determine the tag and object size of a serialized element.
/// @param tag  Return storage for the tag.
/// @param object_size  Return storage for the length of the object as serialized into buffer.
/// @param accum  An accumulator type that supports `empty()`, `size()`, `front()`, and `front(size_t)` where the return type from front implements `size()` and `data()` such that data returns a pointer.
/// @return false is returned if buffer was too small to completely decode tag_value and object_size. Otherwise true.
template<typename T>
bool object_info(OBJECT_TAG_T& tag, size_t& object_size, T& accum);



template<typename... Ts> struct required { using tuple = std::tuple<Ts...>; };
template<typename... Ts> struct optional { using tuple = std::tuple<Ts...>; };



template<OBJECT_TAG_T TAG, typename REQUIRED_ELEMENTS, typename OPTIONAL_ELEMENTS>
class object
   : public base
{
public:
   using self_type = object;

   using ptr = std::shared_ptr<object>;
   using list_t = sb::devector<ptr>;

   /// constructor
   template<typename... Ts>
   object(const Ts&... ts);
   /// Deserialization flag and constructor. For internal use in deserilize() factory function.
   struct deserialization_flag {};
   object(deserialization_flag);
   /// destructor
   virtual ~object();

   /// The object's tag.
   static constexpr tag_type tag = TAG;
   /// Return the tag of this object.
   virtual tag_type get_tag() const override;


   /// Compare object elements.
   bool operator==(const self_type& rhs) const;
   /// Compare object elements.
   bool operator!=(const self_type& rhs) const;


   /// @return true if all the required elements are valid.
   virtual bool required_valid() const override;
   /// @return true if there are any optional elments that are valid.
   bool has_valid_optional() const;

   /// @return true if state() would return `valid`.
   template<typename E> bool is_valid() const;
   /// @return true if state() would return `dismissed`.
   template<typename E> bool is_dismissed() const;
   /// @return true if state() would NOT return `valid`.
   template<typename E> bool is_special() const;
   /// @return The element's internal state (i.e. `invalid`, `valid`, or `dismissed`).
   template<typename E> control_t state() const;


   /// Get a const ref to the element's value.
   /// @throw element_dismissed or element_invalid if the elment is dismissed or invalid.
   template<typename E> const typename E::value_type& get() const;
   /// Get an element's value if it's valid, otherwise return the default value.
   /// @param default_value  The default value. Returned if the element would return true for is_special().
   /// @return Either the elment's value or the passed in default value.
   template<typename E> typename E::value_type get_default(const typename E::value_type& default_value) const;
   /// Get an element's value and return true if it's in the valid state, return false otherwise.
   template<typename E> bool try_get( typename E::value_type& val) const;

   /// Set an element's value ensuring it's valid.
   template<typename E> void set(const typename E::value_type& val);
   /// Clear an element's values, making it invalid.
   template<typename E> void clear();
   /// Change an element to the dismissed state.
   template<typename E> void dismiss();

   /// Get an element for this object.
   /// @return A constant reference to the element.
   template<typename E>
   const E& get_element() const;
   /// Set an element.
   /// @param el  The element to set.
   template<typename E>
   void set_element(const E& el);


   /// Ensure an object has a given element at compile time.
   template<typename E>
   static constexpr void ensure_element();
   /// Test to see if the objects has a given element.
   /// @return true if m_data tuple includes T; false otherwise.
   template<typename E>
   static constexpr bool has_element();
   /// Get an element for this object or throw `unknown_element` exception.
   /// @note Calls to this function should be protected with `has_element<T>()` or `ensure_element<T>()`.
   /// @return A constant reference to the element.
   /// @throws unknown_element if T has_element<T>() would return false
   template<typename E>
   const E& get_element_maybe() const;
   /// Set an element for this object or throw `unknown_element` exception.
   /// @note Calls to this function should be protected with `has_element<T>()` or `ensure_element<T>()`.
   /// @param el  The element to set.
   /// @throws unknown_element if T has_element<T>() would return false.
   template<typename E>
   void set_element_maybe(const E& el);


   /// Debugging method.
   virtual void print(std::ostream& os) const override;

   /// @return The number of bytes this object will consume in a call to `serialize()`.
   virtual size_t serialized_size() const override;
   /// @param buffer  The target buffer to receive the serialized object. It *must* be at least `serialized_size()` bytes in size.
   /// @return the number of bytes "consumed" by serialization into buffer.
   virtual size_t serialize(void* buffer) const override;

   /// @note `object_info()` should be used to ensure buffer is adequately sized for deserialization.
   /// @note This function does not test the validity of the object, simply deserializes it. Call `required_valid()` to test validity.
   /// @return A ptr to this a newly created dispatch object of this type.
   static ptr deserialize(const void* buffer);


private:
   /// Helper functions for constructor.
   template<size_t N, typename T, typename... Ts>
   void construct(const T& t, const Ts&... ts);
   template<size_t N, typename T>
   void construct(const T& t);
   template<size_t N>
   void construct();

   using required_t = typename REQUIRED_ELEMENTS::tuple;
   using optional_t = typename OPTIONAL_ELEMENTS::tuple;

   using data_t = decltype( std::tuple_cat(typename REQUIRED_ELEMENTS::tuple{}, typename OPTIONAL_ELEMENTS::tuple{}) );
   data_t m_data;
};


} // namespace composition
} // namespace sb

#include <sb/composition/detail/object.ipp>


#endif
