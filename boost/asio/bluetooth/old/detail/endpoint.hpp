//
// bluetooth/detail/endpoint.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Scott Bailey
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_BLUETOOTH_DETAIL_ENDPOINT_HPP
#define BOOST_ASIO_BLUETOOTH_DETAIL_ENDPOINT_HPP

//#include <boost/asio/bluetooth/basic_endpoint.hpp>

namespace boost {
namespace asio {
namespace bluetooth {
namespace detail {

typedef bdaddr_t socket_addr_type;


class endpoint
{
public:
   endpoint()
      : addr_()
      , port_num_(0)
   {
   }


   endpoint(const boost::asio::bluetooth::address& addr, unsigned short port_num)
      : addr_(addr)
      , port_num_(port_num)
   {
   }

   bdaddr_t* data()
   {
      return &bdaddr_;
   }

   std::size_t size() const
   {
      return sizeof(bdaddr_);
   }

   std::size_t capacity() const
   {
      return sizeof(bdaddr_);
   }

   unsigned short port() const
   {
      return port_num_;
   }

   void port(unsigned short port_num)
   {
      port_num_ = port_num;
   }


private:
   boost::asio::bluetooth::address addr_;
   unsigned short port_num_;
   socket_addr_type bdaddr_;
};


} // namespace detail
} // namespace bluetooth
} // namespace asio
} // namespace boost

#endif
