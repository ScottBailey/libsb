#ifndef sb_serialize_boost_h
# error "Direct inclusion error."
#endif

#include <sb/serialize/simple.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/uuid/uuid.hpp>


namespace sb {
namespace serialize {

template<>
struct simple<boost::posix_time::time_duration>
{
   using implt_t = uint64_t;
   static inline size_t size(const boost::posix_time::time_duration& /*obj*/) {
      const implt_t temp(0);
      return simple<implt_t>::size(temp);
   }
   static inline size_t serialize(const boost::posix_time::time_duration& obj, void* buf) {
      implt_t temp = obj.total_microseconds();
      return simple<implt_t>::serialize(temp,buf);
   }
   static inline size_t deserialize(boost::posix_time::time_duration& obj, const void* buf) {
      implt_t temp;
      size_t rv = simple<implt_t>::deserialize(temp, buf);
      obj = boost::posix_time::microseconds(temp);
      return rv;
   }
};


template<>
struct simple<boost::posix_time::ptime>
{
   using impl_t = uint64_t;
   static inline size_t size(const boost::posix_time::ptime& /*obj*/) {
      const impl_t temp(0);
      return simple<impl_t>::size(temp);
   }
   static inline size_t serialize(const boost::posix_time::ptime& obj, void* buf) {
      impl_t temp;
      if( !obj.is_special() )
      {
         const boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
         temp = (obj - epoch).total_microseconds();
      }
      else if( obj.is_pos_infinity() )
         temp = std::numeric_limits<impl_t>::max();
      else if( obj.is_neg_infinity() )
         temp = std::numeric_limits<impl_t>::min()+1;
      else // if( obj.is_not_a_date_time() )
         temp = std::numeric_limits<impl_t>::min();
      return simple<impl_t>::serialize(temp,buf);
   }
   static inline size_t deserialize(boost::posix_time::ptime& obj, const void* buf) {
      impl_t temp;
      size_t rv = simple<impl_t>::deserialize(temp, buf);

      // decode special values:
      switch( temp ) {
         case std::numeric_limits<impl_t>::max():    obj = boost::posix_time::pos_infin;        return rv;
         case std::numeric_limits<impl_t>::min()+1:  obj = boost::posix_time::neg_infin;        return rv;
         case std::numeric_limits<impl_t>::min():    obj = boost::posix_time::not_a_date_time;  return rv;
      }

      try
      {
         const boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
         obj = epoch + boost::posix_time::microseconds(temp);
      }
      catch(std::out_of_range&)
      {
         // out of range we will set the value to not a date time
         obj = boost::posix_time::not_a_date_time;
      }
      return rv;
   }
};


template<>
struct simple<boost::posix_time::time_period>
{
   using implt_t = uint64_t;
   static inline size_t size(const boost::posix_time::time_period& obj)
   {
      if(obj.is_null()) {
         return sizeof(implt_t);
      }
      return 2*sizeof(implt_t);
   }
   static inline size_t serialize(const boost::posix_time::time_period& obj, void* buf) {
      const boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
      implt_t* p = reinterpret_cast<implt_t*>(buf);
      // null
      if(obj.is_null()) {
         *p = 0;
         return sizeof(implt_t);
      }
      // time and duration
      const implt_t temp1 = (obj.begin() - epoch).total_microseconds();
      *p = endian::swap_net(temp1);

      const implt_t temp2 = obj.length().total_microseconds();
      *(p+1) = endian::swap_net(temp2);

      return 2 * sizeof(implt_t);
   }
   static inline size_t deserialize(boost::posix_time::time_period& obj, const void* buf) {
      const boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
      const implt_t* p = reinterpret_cast<const implt_t*>(buf);
      // read the first uint64_t, indicates null or begin time
      implt_t temp1 = endian::swap_net(*p);
      // zero indicates a null time_period
      if(!temp1) {
         obj = boost::posix_time::time_period{};
         return sizeof(implt_t);
      }
      // read duration
      implt_t temp2 = endian::swap_net( *(p+1) );

      // convert temp1 to a ptime
      boost::posix_time::ptime when = epoch + boost::posix_time::microseconds(temp1);

      // and set obj
      obj = boost::posix_time::time_period{ when, boost::posix_time::microseconds(temp2) };
      return 2 * sizeof(implt_t);
   }
};


template<>
struct simple<boost::property_tree::ptree>
{
   static inline size_t size(const boost::property_tree::ptree& obj) {
      std::stringstream ss;
      boost::property_tree::xml_parser::write_xml( ss, obj );
      return simple<std::string>::size(ss.str());
   }
   static inline size_t serialize(const boost::property_tree::ptree& obj, void* buf) {
      std::stringstream ss;
      boost::property_tree::xml_parser::write_xml(ss,obj);
      return simple<std::string>::serialize(ss.str(),buf);
   }
   static inline size_t deserialize(boost::property_tree::ptree& obj, const void* buf) {
      std::string s;
      const size_t rv = simple<std::string>::deserialize(s,buf);
      std::stringstream ss(s);
      obj.clear();
      // A try/catch block should be here!
      boost::property_tree::xml_parser::read_xml(ss,obj, boost::property_tree::xml_parser::trim_whitespace);
      return rv;
   }
};


template<>
struct simple<boost::uuids::uuid>
{
   static inline size_t size(const boost::uuids::uuid& obj) {
      return obj.size();
   }
   static inline size_t serialize(const boost::uuids::uuid& obj, void* buf) {
      size_t sz=size(obj);
      std::copy(obj.begin(), obj.end(), reinterpret_cast<uint8_t*>(buf));
      return sz;
   }
   static inline size_t deserialize(boost::uuids::uuid& obj, const void* buf) {
      size_t rv = sizeof(obj);
      const uint8_t* p=reinterpret_cast<const uint8_t*>(buf);
      std::copy(p,p+rv,obj.begin());
      return rv;
   }
};


} // namespace serialize
} // namespace sb
