//
// bluetooth/rfcomm.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Scott Bailey
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_BLUETOOTH_RFCOMM_HPP
#define BOOST_ASIO_BLUETOOTH_RFCOMM_HPP

#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/basic_socket_acceptor.hpp>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


namespace boost {
namespace asio {
namespace bluetooth {

class rfcomm_endpoint;

class rfcomm
{
public:
   typedef rfcomm_endpoint endpoint;

   static rfcomm get()
   {
      return rfcomm();
   }

   /// Obtain an identifier for the type of the protocol.
   int type() const
   {
      return SOCK_STREAM;
   }

   /// Obtain an identifier for the protocol.
   int protocol() const
   {
      return BTPROTO_RFCOMM;
   }

   /// Obtain an identifier for the protocol family.
   int family() const
   {
      return AF_BLUETOOTH;
   }

   /// socket type
   typedef basic_stream_socket<rfcomm> socket;

   /// acceptor type
   typedef basic_socket_acceptor<rfcomm> acceptor;
};

} // namespace bluetooth
} // namespace asio
} // namespace boost

#include <boost/asio/bluetooth/rfcomm_endpoint.hpp>



#endif
