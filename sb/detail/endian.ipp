/*
   Copyright (c) 2013-2019, Scott Bailey
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
       * Neither the name of the <organization> nor the
         names of its contributors may be used to endorse or promote products
         derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL SCOTT BAILEY BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef sb_endian_h
# error "detail/endian.ipp must not be directly encluded!"
#endif

#include <stdint.h>
#include <boost/predef/other/endian.h>

#if defined(_WIN32) // for MSVS and MINGW
# define _CRT_SECURE_DEPRECATE_MEMORY
# include <memory.h>  //memcpy()
#else
# include <string.h>  //memcpy()
#endif

// ensure we know what the endianness is
#if defined(BOOST_ENDIAN_LITTLE_BYTE)
#  define SB_LITTLE_ENDIAN BOOST_ENDIAN_LITTLE_BYTE
#elif defined(BOOST_ENDIAN_BIG_BYTE)
#  define SB_BIG_ENDIAN BOOST_ENDIAN_BIG_BYTE
#elif defined(BOOST_ENDIAN_LITTLE_WORD)
#  error ("Unimplemented endian byte order: endian little word")
#elif defined(BOOST_ENDIAN_BIG_WORD)
#  error ("Unimplemented endian byte order: endian big word")
#else
#  error ("Unknown endian byte order")
#endif

namespace sb {
namespace endian {
namespace detail {


template<int N>
inline void copy( void* dst, const void* src)
{
   const uint8_t* s = reinterpret_cast<const uint8_t*>(src); // first byte of source
   uint8_t* d = reinterpret_cast<uint8_t*>(dst) + N - 1;     // first byte of dest + length-1 = last byte of dest
   while( d >= dst)   // while more data
   {
      *d = *s;  // copy the byte
      ++s;      // increment the source pointer
      --d;      // increment the destination pointer
   }
}

template<>
inline void copy<1>( void* dst, const void* src)
{
   const uint8_t* s = reinterpret_cast<const uint8_t*>(src); // only byte of source
   uint8_t* d = reinterpret_cast<uint8_t*>(dst);             // only byte of dest
   *d = *s;  // copy the byte
}

template<>
inline void copy<2>( void* dst, const void* src)
{
   const uint8_t* s = reinterpret_cast<const uint8_t*>(src); // first byte of source
   uint8_t* d = reinterpret_cast<uint8_t*>(dst) + (2 - 1);     // first byte of dest + length-1 = last byte of dest
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  // copy the byte
}

template<>
inline void copy<4>( void* dst, const void* src)
{
   const uint8_t* s = reinterpret_cast<const uint8_t*>(src); // first byte of source
   uint8_t* d = reinterpret_cast<uint8_t*>(dst) + (4 - 1);     // first byte of dest + length-1 = last byte of dest
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  // copy the byte
}

template<>
inline void copy<8>( void* dst, const void* src)
{
   const uint8_t* s = reinterpret_cast<const uint8_t*>(src); // first byte of source
   uint8_t* d = reinterpret_cast<uint8_t*>(dst) + (8 - 1);     // first byte of dest + length-1 = last byte of dest
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  ++s;  --d;  // copy the byte,  update the source pointer && dest pointers
   *d = *s;  // copy the byte
}


template<typename T>
inline void swap( T* loc)
{
   T temp(*loc);
   copy<sizeof(T)>(loc,&temp);
}



} // namespace detail




template<typename SRC, typename RET>
inline RET swap_be(const SRC& s)
{
   static_assert(sizeof(RET) == sizeof(SRC), "mismatched type sizes");
   RET rv;
#if defined(SB_LITTLE_ENDIAN)
   detail::copy<sizeof(SRC)>(&rv,&s);
#else
   memcpy(&rv,&s,sizeof(SRC));
#endif
   return rv;
}


template<typename SRC, typename RET>
inline RET swap_le(const SRC& s)
{
   static_assert(sizeof(RET) == sizeof(SRC), "mismatched type sizes");
   RET rv;
#if defined(SB_LITTLE_ENDIAN)
   memcpy(&rv,&s,sizeof(SRC));
#else
   detail::copy<sizeof(SRC)>(&rv,&s);
#endif
   return rv;
}


template<typename SRC, typename RET>
inline RET swap_net(const SRC& s)
{
   return swap_be<SRC,RET>(s);
}


} // namespace endian
} // namespace sb
