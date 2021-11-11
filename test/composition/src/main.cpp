#include <iostream>
#include <sb/composition/object.h>
#include <sb/composition/element.h>
#include <sb/composition/copy.h>
#include <sb/message/accumulator.h>
#include <sb/serialize.h>
#include <sb/serialize.h>
#include <vector>



enum tags {
   zero =0,
   one,
   two,
   three,
   four,
};

class unprintable {
public:
   unprintable() : m_a{-1} {}
   unprintable(int a) : m_a{a} {}
   const int& a() const { return m_a; }
private:
   int m_a;
};


using e0 = sb::composition::element<tags::zero, int>;
using e1 = sb::composition::element<tags::one, double>;
using e2 = sb::composition::element<tags::two, std::string>;
using e3 = sb::composition::element<tags::three, std::string>;
using e4 = sb::composition::element<tags::four, unprintable, sb::serialize::none<unprintable>>;


enum enumerants {
   alpha = 1,
   bravo = 2,
};


using container1_t = sb::composition::object<enumerants::alpha,
                                             sb::composition::required<e0
                                                                       , e2
                                                                       >,
                                             sb::composition::optional<e1
                                                                       , e3
                                                                       >
                                             >;


using container2_t = sb::composition::object<enumerants::bravo,
                                             sb::composition::required<
                                                >,
                                             sb::composition::optional<e0
                                                                       , e1
                                                                       >
                                             >;

using container3_t = sb::composition::object<enumerants::bravo,
                                             sb::composition::required< e4 >,
                                             sb::composition::optional< >
                                             >;

int main(int,char**) {

   //std::cout << sb::can_print<unprintable>() << std::endl;

   sb::message::accumulator<std::vector<uint8_t>> accum;

   {
      container1_t m1{2,"hello"};
      //m1.dismiss<e1>();

      container2_t m2{};
      m2.set<e1>(4.4);
      //m2.dismiss<e1>();

      //container3_t m3{ unprintable(4) };

      std::cout
         << m1.get<e0>() << std::endl
         //<< m1.get<e1>() << std::endl
         << m1 << std::endl
         << "Serialize size: " << m1.serialized_size() << std::endl
         << "Serialize size: " << m2.serialized_size() << std::endl
         //<< m1.get<e2>() << std::endl
         ;

      //m1.clear<e0>();

      std::cout
         << "m1 e2: " << m1.has_element<e2>() << std::endl
         << "m2 e2: " << m2.has_element<e2>() << std::endl
         ;
      //if(m2.has_element<e2>()) {
      try {

         std::cout << "e2? " << std::flush;
         const auto& e = m2.get_element_maybe<e2>();
         std::cout << e.get() << std::endl;
      }
      catch(sb::composition::unknown_element&) {
         std::cout << "caught sb::composition::unknown_element!" << std::endl;
      }


      if(0) {
         //m1.clear<e0>();
         std::cout
            << m1.required_valid() << std::endl
            << m2.required_valid() << std::endl
            ;
      }

      m1.ensure_element<e3>();

      //sb::composition::element_try_copy<e2>(m1,m2);


      const size_t ssz = m1.serialized_size();
      std::vector<uint8_t> b1(ssz);
      std::vector<uint8_t> b2(m2.serialized_size());
      const size_t rssz = m1.serialize(b1.data());
      std::cout << ssz << " " << rssz << std::endl;
      m2.serialize(b2.data());
      accum.push_back(std::move(b1));
      accum.push_back(std::move(b2));
   }

   {
      std::cout << "Deserializing!" << std::endl;

      std::vector<sb::composition::base::ptr> v;

      unsigned tag;
      size_t sz;
      while(sb::composition::object_info(tag,sz,accum) && (accum.size() >= sz)) {
         sb::composition::base::ptr temp;
         auto msg = accum.pop_front(sz);
         switch(static_cast<enumerants>(tag)) {
            case enumerants::alpha: temp = container1_t::deserialize(msg.data()); break;
            case enumerants::bravo: temp = container2_t::deserialize(msg.data()); break;
         }
         if(!temp) {
            std::cerr << "Error deserializing object with tag: " << tag << ", size: " << sz << std::endl;
            continue;
         }
         if(!temp->required_valid()) {
            std::cerr << "Missing required elements for object: " << *temp << std::endl;
         }
         else {
            v.push_back(temp);
         }
      }

      for(const auto& a : v) {
         std::cout << *a << std::endl;
      }


   }


   {
      container1_t m1{2,"hello"};
      container1_t m2{2,"hello"};
      container1_t m3{2,"goodbye"};

      std::cout
         << "m1 == m2 " << (m1 == m2) << std::endl
         << "m1 != m2 " << (m1 != m2) << std::endl
         << "m2 != m3 " << (m2 != m3) << std::endl
         ;
   }



   return 0;
}
