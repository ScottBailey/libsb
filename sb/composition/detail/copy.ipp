#ifndef sb_composition_copy_h
# error "Direct inclusion error."
#endif


namespace sb {
namespace composition {


template<typename ELEMENT, typename D, typename S>
inline void element_copy(D& dst, const S& src) {

   if(src.template is_valid<ELEMENT>())
      dst.template set<ELEMENT>(src.template get<ELEMENT>());
   else
      dst.template clear<ELEMENT>();
}


template<typename ELEMENT, typename D, typename S>
inline void element_copy_default(D& dst, const S& src, const typename ELEMENT::data_type& default_value) {

   if(src.template is_valid<ELEMENT>())
      dst.template set<ELEMENT>(src.template get<ELEMENT>());
   else
      dst.template set<ELEMENT>(default_value);

}


template<typename ELEMENT, typename DST, typename SRC>
inline bool element_try_copy(DST& dst, const SRC& src) {

   switch(src.template state<ELEMENT>()) {

      case control::valid:
         dst.template set<ELEMENT>(src.template get<ELEMENT>());
         return true;

      case control::dismissed:
         dst.template clear<ELEMENT>();
         return true;

      case control::invalid:
         break;
   }

   return false;
}


} // namespace composition
} // namespace sb
