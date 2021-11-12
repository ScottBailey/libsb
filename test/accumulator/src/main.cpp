#include <sb/message/accumulator.h>
#include <vector>
#include <stdint.h>
#include <iostream>

int main(int,char**) {

   // Create and load an accumulator with data.
   sb::message::accumulator<std::vector<uint8_t>> accum;
   {
      std::vector<uint8_t> v {0, 1, 2, 3};
      accum.push_back(v);
   }
   {
      std::vector<uint8_t> v {4, 5, 6, 7};
      accum.push_back(v);
   }
   {
      std::vector<uint8_t> v {8, 9, 10, 11};
      accum.push_back(v);
   }

   // Print it out. For fun.
   for(const auto& a : accum) {
      std::cout << unsigned( a ) << " ";
   }
   std::cout << std::endl;

   // Test data.
   for(size_t i=0; i < accum.size(); ++i) {
      if( i != size_t(accum[i]))
         std::cerr << "line " << __LINE__ << ": element " << i << " of accumulator has the wrong value: " << unsigned(accum[i]) << std::endl;
   }

   // Test iteration.
   {
      auto a = accum.begin();
      if(*a != 0)
         std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << std::endl;
      ++a;
      if(*a != 1)
         std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << std::endl;
      std::advance(a,3);
      if(*a != 4)
         std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << std::endl;
      --a;
      if(*a != 3)
         std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << std::endl;
      std::advance(a,8);
      if(*a != 11)
         std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << std::endl;
      ++a;
      if(a != accum.end())
         std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << std::endl;
   }


   // Cause the elements contained inside the accumulator to be resized/moved around.
   {
      const auto& a = accum.front(6);
      if(a.size() != 6)
         std::cerr << "line " << __LINE__ << ": unexpected size: " << a.size() << std::endl;
   }

   // Test data.
   for(size_t i=0; i < accum.size(); ++i) {
      if( i != size_t(accum[i]))
         std::cerr << "line " << __LINE__ << ": element " << i << " of accumulator has the wrong value: " << unsigned(accum[i]) << std::endl;
   }

   // Cause the elements contained inside the accumulator to be resized/moved around.
   {
      const auto& a = accum.back(6);
      if(a.size() != 6)
         std::cerr << "line " << __LINE__ << ": unexpected size: " << a.size() << std::endl;
   }

   // Test data.
   for(size_t i=0; i < accum.size(); ++i) {
      if( i != size_t(accum[i]))
         std::cerr << "line " << __LINE__ << ": element " << i << " of accumulator has the wrong value: " << unsigned(accum[i]) << std::endl;
   }


   {
      const auto& a = accum.back();
      if(a.size() != 6)
         std::cerr << "line " << __LINE__ << ": unexpected size: " << a.size() << std::endl;
   }


   // Add more tests as time allows.

   return 0;
}
