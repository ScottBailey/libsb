#include <iostream>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sb/serialize.h>

int main(int,char**) {

   std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(0xFFFF);


   using ser=sb::serialize::simple<boost::posix_time::ptime>;
   {
      auto now = boost::posix_time::second_clock::local_time();
      //size_t len = ser::size(now);
      ser::serialize(now,buffer.get());
   }

   {
      boost::posix_time::ptime when;
      //size_t len = ser::size(when);
      ser::deserialize(when,buffer.get());

      std::cout << "val: " << when << std::endl;
   }


   return 0;
}
