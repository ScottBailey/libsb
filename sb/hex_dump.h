#ifndef sb_hex_dump_h
#define sb_hex_dump_h

#include <iostream>

namespace sb {

inline void hex_dump(std::ostream& os, const void* const v, const size_t& sz) {
   if( !v || !sz )
      return;

   const uint8_t* p = reinterpret_cast<const uint8_t*>(v);
   const uint8_t* const end = p+sz;

   os << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << unsigned(*p);
   ++p;

   while( p < end ) {
      os << " " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << unsigned(*p);
      ++p;
   }
}


template <typename T>
inline void hex_dump(std::ostream& os, const T& t) {
   hex_dump(os, &t, sizeof(t));
}



} // namespace sb

#endif
