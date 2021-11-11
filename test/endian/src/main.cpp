#include <iostream>
#include <sstream>
#include <sb/endian.h>
#include <stdint.h>
#include <iomanip>

template<typename T>
std::string dex(const T& t) {
   std::ostringstream ss;
   ss << std::hex << std::uppercase << std::setw(sizeof(T)*2) << std::setfill('0') << uint64_t(t);
   return ss.str();
}


int main(int,char**) {

   using namespace sb::endian;

   uint16_t a{0x00FF};
   uint16_t b{ swap_net(a) };
   uint16_t c{ swap_net(b) };

   uint32_t d1{c};
   swap_le(d1);
   uint32_t d2{c};
   swap_be(d2);
   uint32_t d3{d2};
   swap_net(d3);


   float f {12.34};
   uint32_t g{ swap_net<float, uint32_t>(f) };
   float h{ swap_net<uint32_t, float>(g) };

   uint32_t f2, h2;
   memcpy(&f2,&f,sizeof(f));
   memcpy(&h2,&h,sizeof(h));


   uint16_t a2{ swap_le(a) };

   std::cout
      << dex(a) << std::endl
      << dex(a2) << std::endl
      << dex(b) << std::endl
      << dex(c) << std::endl
      << dex(d1) << std::endl
      << dex(d2) << std::endl
      << dex(d3) << std::endl
      << dex(f2) << " " << f << std::endl
      << dex(g) << std::endl
      << dex(h2) << " " << h << std::endl
      ;


   return 0;
}
