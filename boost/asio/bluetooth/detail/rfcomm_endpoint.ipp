//
// bluetooth/rfcomm.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016-2021 Scott Bailey
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_BLUETOOTH_DETAIL_RFCOMM_ENDPOINT_HPP
#define BOOST_ASIO_BLUETOOTH_DETAIL_RFCOMM_ENDPOINT_HPP

#include <boost/asio/bluetooth/rfcomm.hpp>
#include <string.h> // memcmp()

namespace boost {
namespace asio {
namespace bluetooth {


inline std::ostream& operator<<(std::ostream& os, const bdaddr_t& addr) {
   std::unique_ptr<char[]> temp = std::make_unique<char[]>(32);
   if(!ba2str(&addr,temp.get()))
      os << "addr error";
   else
      os << temp.get();
   return os;
}


inline std::ostream& operator<<(std::ostream& os, const boost::asio::bluetooth::rfcomm_endpoint& endpoint) {
   os << endpoint.addr() << " : " << unsigned(endpoint.channel());
   return os;
}


//--- file locals -----------------------------------------------------------------------------------------------------

const bdaddr_t addr_any = {{0, 0, 0, 0, 0, 0}};

namespace {

inline bool operator==(const bdaddr_t& lhs, const bdaddr_t& rhs) {
   return (memcmp(&lhs,&rhs,sizeof(lhs)) == 0);
}


inline bool operator!=(const bdaddr_t& lhs, const bdaddr_t& rhs) {
   return (memcmp(&lhs,&rhs,sizeof(lhs)) != 0);
}


static inline bool operator>(const bdaddr_t& lhs, const bdaddr_t& rhs) {
   return (memcmp(&lhs,&rhs,sizeof(lhs)) > 0);
}


inline bool operator>=(const bdaddr_t& lhs, const bdaddr_t& rhs) {
   return (memcmp(&lhs,&rhs,sizeof(lhs)) >= 0);
}


inline bool operator<(const bdaddr_t& lhs, const bdaddr_t& rhs) {
   return (memcmp(&lhs,&rhs,sizeof(lhs)) < 0);
}


inline bool operator<=(const bdaddr_t& lhs, const bdaddr_t& rhs) {
   return (memcmp(&lhs,&rhs,sizeof(lhs)) <= 0);
}


} // anonymous namespace


//--- constructors/destructor ------------------------------------------------------------------------------------------

inline rfcomm_endpoint::rfcomm_endpoint()
{
   addr_.rc_family = AF_BLUETOOTH;
   addr_.rc_bdaddr = addr_any;
   //memcpy( &addr_.rc_bdaddr, BDADDR_ANY, sizeof( addr_.rc_bdaddr) );
   addr_.rc_channel = 1;
}


inline rfcomm_endpoint::rfcomm_endpoint(uint8_t in_channel)
{
   addr_.rc_family = AF_BLUETOOTH;
   addr_.rc_bdaddr = addr_any;
   addr_.rc_channel = in_channel;
}


inline rfcomm_endpoint::rfcomm_endpoint(const bdaddr_t& in_addr, uint8_t in_channel)
{
   addr_.rc_family = AF_BLUETOOTH;
   addr_.rc_bdaddr = in_addr;
   addr_.rc_channel = in_channel;
}


inline rfcomm_endpoint::rfcomm_endpoint(const rfcomm_endpoint& other)
   : addr_(other.addr_)
{
}


#if defined(BOOST_ASIO_HAS_MOVE)

inline rfcomm_endpoint::rfcomm_endpoint(const rfcomm_endpoint&& other)
   : addr_(other.addr_)
{
}

#endif // defined(BOOST_ASIO_HAS_MOVE)

//--- operators ------------------------------------------------------------------------------------------

inline rfcomm_endpoint& rfcomm_endpoint::operator=(const rfcomm_endpoint& other)
{
   addr_ = other.addr_;
   return *this;
}


#if defined(BOOST_ASIO_HAS_MOVE)
inline rfcomm_endpoint& rfcomm_endpoint::operator=(const rfcomm_endpoint&& other)
{
   addr_ = other.addr_;
   return *this;
}
#endif // defined(BOOST_ASIO_HAS_MOVE)


//--- friend operators ------------------------------------------------------------------------------------------

inline bool operator==(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2)
{
   return (e1.addr_.rc_bdaddr == e2.addr_.rc_bdaddr) && (e1.addr_.rc_channel == e2.addr_.rc_channel);
}


inline bool operator!=(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2)
{
   return (e1.addr_.rc_bdaddr != e2.addr_.rc_bdaddr) || (e1.addr_.rc_channel != e2.addr_.rc_channel);
}


inline bool operator<(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2)
{
   return (e1.addr_.rc_bdaddr < e2.addr_.rc_bdaddr)
      || ((e1.addr_.rc_bdaddr == e2.addr_.rc_bdaddr) && (e1.addr_.rc_channel < e2.addr_.rc_channel));
}


inline bool operator>(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2)
{
   return (e1.addr_.rc_bdaddr > e2.addr_.rc_bdaddr)
      || ((e1.addr_.rc_bdaddr == e2.addr_.rc_bdaddr) && (e1.addr_.rc_channel > e2.addr_.rc_channel));
}


inline bool operator<=(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2)
{
   return (e1.addr_.rc_bdaddr < e2.addr_.rc_bdaddr)
      || ((e1.addr_.rc_bdaddr == e2.addr_.rc_bdaddr) && (e1.addr_.rc_channel <= e2.addr_.rc_channel));
}


inline bool operator>=(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2)
{
   return (e1.addr_.rc_bdaddr > e2.addr_.rc_bdaddr)
      || ((e1.addr_.rc_bdaddr == e2.addr_.rc_bdaddr) && (e1.addr_.rc_channel >= e2.addr_.rc_channel));
}


//--- alphabetic ------------------------------------------------------------------------------------------


inline bdaddr_t rfcomm_endpoint::addr() const
{
   return addr_.rc_bdaddr;
}


inline std::size_t rfcomm_endpoint::capacity() const
{
   return sizeof(addr_);
}


inline uint8_t rfcomm_endpoint::channel() const
{
   return addr_.rc_channel;
}


inline rfcomm_endpoint::address_type* rfcomm_endpoint::data() const
{
   return const_cast<rfcomm_endpoint::address_type*>( reinterpret_cast<const rfcomm_endpoint::address_type*>(&addr_) );
}


inline rfcomm_endpoint::protocol_type rfcomm_endpoint::protocol() const
{
   return protocol_type();
}


inline void rfcomm_endpoint::resize(std::size_t sz)
{
   if ( sz > sizeof(sockaddr_rc) )
   {
      boost::system::system_error e(boost::asio::error::invalid_argument);
      boost::throw_exception(e);
   }
}


inline std::size_t rfcomm_endpoint::size() const
{
   return sizeof(addr_);
}




}
}
}



#endif
