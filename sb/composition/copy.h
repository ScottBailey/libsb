#ifndef sb_composition_copy_h
#define sb_composition_copy_h


namespace sb {
namespace composition {


/// Copy ELEMENT type data if it exists from src to dst, if src is invalid or dismissed, dst is cleared.
/// @param dst The destination object
/// @param src The source object
template<typename ELEMENT, typename D, typename S>
void element_copy(D& dst, const S& src);


/// Copy either valid src data or default_value into src.
/// @param dst The destination object.
/// @param src The source object.
/// @param def The default data to copy.
/// @return true if src.is_valid() and copied to dst; otherwise, false if default_value is copied to src.
template<typename ELEMENT, typename D, typename S>
void element_copy_default(D& dst, const S& src, const typename ELEMENT::data_type& default_value);


/// Copy ELEMENT type data from one object to another if src is in 'valid' or 'dismissed' state.
/// @note If src.is_dismissed() would return true, dst is cleared.
/// @param dst The destination object.
/// @param src The source object.
/// @return true if an operation was performed on dst (i.e. set() or clear() was called); false is returned if src<ELEMENT> is dismissed.
template<typename ELEMENT, typename DST, typename SRC>
bool element_try_copy(DST& dst, const SRC& src);



} // namespace composition
} // namespace sb

#include <sb/composition/detail/copy.ipp>

#endif
