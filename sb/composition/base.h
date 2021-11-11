#ifndef sb_composition_base_h
#define sb_composition_base_h

#include <iostream>
#include <memory>
#include <stdlib.h> // size_t
#include <sb/devector.h>


namespace sb {
namespace composition {

using OBJECT_TAG_T = unsigned;

class base {
public:
   using ptr = std::shared_ptr<base>;
   using list_t = devector<ptr>;

   using tag_type = OBJECT_TAG_T;


   base() = default;
   virtual ~base() = default;

   /// Return the tag of this object.
   virtual tag_type get_tag() const =0;
   /// @return true if all required elements are valid.
   virtual bool required_valid() const =0;

   virtual void print(std::ostream& os) const =0;

   virtual size_t serialized_size() const =0;
   virtual size_t serialize(void* target) const =0;
};


} // namespace composition
} // namespace sb

inline std::ostream& operator<<(std::ostream& os, const sb::composition::base& obj) { obj.print(os); return os; }

#endif
