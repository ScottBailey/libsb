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

#include <boost/asio/bluetooth/basic_endpoint.hpp>

namespace boost {
namespace asio {
namespace bluetooth {

class rfcomm
{
public:
   typedef rfcomm_endpoint endpoint;


   /// socket type
   typedef basic_stream_socket<rfcomm> socket;

   /// acceptor type
   typedef basic_socket_acceptor<rfcomm> acceptor;
};

} // namespace bluetooth
} // namespace asio
} // namespace boost

#endif
