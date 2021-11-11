#ifndef sb_serialize_compress_unsigned_h
# error "Direct inclusion error."
#endif

#include <stdint.h>
#include <limits>

namespace sb {
namespace serialize {
namespace compress_unsigned {

/*
  Algorithm:

  control field:      1 byte, includes values < 250

  The control field is decoded as follows:
    0..=250 : value is solely contained in the control field.     Max value 250
    251     : value is 251 + the value in the following 1 byte.   Max value 506
    252     : value is 251 + the value in the following 2 bytes.  Max Value 65786
    253     : value is 251 + the value in the following 4 bytes.  Max Value 4294967546
    254     : value is 251 + the value in the following 8 bytes.  Max Value 18446744073709551866
    255     : value is 251 + the value in the following 16 bytes.
                             Max Value 340282366920938463463374607431768211706340282366920938463463374607431768211706

*/



template<typename T>
inline size_t deserialize(T& n, const void* buffer) {
   if(!buffer )
      return 0;

   const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

   // Find how many extra bytes we need.
   size_t need = 0;
   switch(*p) {
      case 251: need = 1; break;
      case 252: need = 2; break;
      case 253: need = 4; break;
      case 254: need = 8; break;
      case 255: need = 16; break;
      default: // values [0..250] are stored in the control byte
	 n = *p;
         return 1;
   }

   // Set the number of bytes for the return value since need is destroyed in the loop.
   const size_t rv = need + 1;

   // Clear the value of n and then shift data into it.
   n = 0;
   while(need) {
      ++p;      // Incerement the pointer. (On entry it points to the control byte.)
      n <<= 8;  // Shift n up a byte and then...
      n += *p;  // ...add in the value at the current pointer location.
      --need;   // Decrement remaining need.
   }

   // Increment n by 251.
   n += 251;

   return rv;
}


template<typename T>
inline size_t serialize(T n, void* buffer) {

   // Sanity check the pointer.
   if(!buffer)
      return 0;

   uint8_t* p = reinterpret_cast<uint8_t*>(buffer);

   // Figure out how many bytes we need.
   size_t need = size(n) - 1;
   switch(need) {
      case 0: *p = static_cast<uint8_t>(n); return 1;  // One byte is sufficient to hold the value.
      case 1: *p = static_cast<uint8_t>(251); break;
      case 2: *p = static_cast<uint8_t>(252); break;
      case 4: *p = static_cast<uint8_t>(253); break;
      case 8: *p = static_cast<uint8_t>(254); break;
      case 16: *p = static_cast<uint8_t>(255); break;
   }
   n -= 251; // Values of 250 and less are stored in a single byte.

   // Increment p past the control byte.
   ++p;

   // Traverse the bytes in n backwards writing them into the buffer.
   static_assert((size_t(0)-1) > 0, "Error: this code depends on the rollover behavior of unsigned ints!");
   for(size_t i=(need - 1); i < need; --i) {
      // Write the Least Significant Byte (LSB) into the buffer...
      p[i] = static_cast<uint8_t>( n & 0xFF );
      n >>= 8; // ...and shift the byte off.
   }

   return need+1;  // The size of bytes we wrote plus the control byte.
}


template<typename T>
inline size_t size(T n) {

   if(n <= 250)
      return 1;
   n -= 251;

   if(n <= std::numeric_limits<uint8_t>::max())
      return 2;
   if(n <= std::numeric_limits<uint16_t>::max())
      return 3;
   if(n <= std::numeric_limits<uint32_t>::max())
      return 5;
   if(n <= std::numeric_limits<uint64_t>::max())
      return 9;

   //if(n <= std::numeric_limits<uint128_t>::max())
   return 17;
}


inline size_t need(const void* buffer) {

   const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);
   switch(*p) {
      case 251: return 2;
      case 252: return 3;
      case 253: return 5;
      case 254: return 9;
      case 255: return 17;
   }
   return 1;
}


} // namespace compress_unsigned
} // namespace serialize
} // namespace sb
