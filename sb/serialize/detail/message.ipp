#ifndef sb_serialize_message_h
# error "Direct inclusion error."
#endif


//#include <sb/message/shared.h>
//#include <sb/message/unique.h>

#include <sb/serialize/simple.h>
#include <stdint.h>             // various width integers
#include <stddef.h>             // size_t
#include <sb/serialize/compress_unsigned.h>
#include <sb/endian.h>          // swap_net<>()


namespace sb {
namespace serialize {

#if defined(sb_message_shared_h)
template<>
struct simple<sb::message::shared<uint8_t>> {

   using value_type = sb::message::shared<uint8_t>;

   static inline size_t size(const value_type& obj) {
      size_t sz = obj.size();
      return sz + compress_unsigned::size(sz);
   }

   static inline size_t serialize(const value_type& obj, void* buf) {
      uint8_t* p = reinterpret_cast<uint8_t*>(buf);
      size_t consumed = compress_unsigned::serialize(obj.size(), p);
      memcpy(p + consumed, obj.data(), obj.size());
      return consumed + obj.size();
   }

   static inline size_t deserialize(value_type& obj, const void* buf) {
      const uint8_t* p = reinterpret_cast<const uint8_t*>(buf);
      size_t payload_sz=0;
      size_t consumed = compress_unsigned::deserialize(payload_sz, p);
      obj.reset(payload_sz);
      if(!payload_sz)
         return consumed;
      memcpy(obj.data(), p + consumed, payload_sz);
      return consumed + payload_sz;
   }
};
#endif


#if defined(sb_message_unique_h)
template<>
struct simple<sb::message::unique<uint8_t>> {

   using value_type = sb::message::shared<uint8_t>;

   static inline size_t size(const value_type& obj) {
      size_t sz = obj.size();
      return sz + compress_unsigned::size(sz);
   }

   static inline size_t serialize(const value_type& obj, void* buf) {
      uint8_t* p = reinterpret_cast<uint8_t*>(buf);
      size_t consumed = compress_unsigned::serialize(obj.size(), p);
      memcpy(p + consumed, obj.data(), obj.size());
      return consumed + obj.size();
   }

   static inline size_t deserialize(value_type& obj, const void* buf) {
      const uint8_t* p = reinterpret_cast<const uint8_t*>(buf);
      size_t payload_sz=0;
      size_t consumed = compress_unsigned::deserialize(payload_sz, p);
      obj.reset(payload_sz);
      if(!payload_sz)
         return consumed;
      memcpy(obj.data(), p + consumed, payload_sz);
      return consumed + payload_sz;
   }
};
#endif


} // namespace serialize
} // namespace sb
