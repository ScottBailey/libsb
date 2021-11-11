//
// bluetooth/rfcomm.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Scott Bailey
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_BLUETOOTH_RFCOMM_ENDPOINT_HPP
#define BOOST_ASIO_BLUETOOTH_RFCOMM_ENDPOINT_HPP

#include <boost/asio/basic_socket.hpp>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <iostream>

namespace boost {
namespace asio {
namespace bluetooth {


class rfcomm;

class rfcomm_endpoint
{
public:
   typedef rfcomm protocol_type;
   //typedef sockaddr_rc address_type;
   typedef boost::asio::detail::socket_addr_type address_type;

   rfcomm_endpoint();
   rfcomm_endpoint(uint8_t channel);
   rfcomm_endpoint(const bdaddr_t& addr, uint8_t channel);
   /// Copy constructor.
   rfcomm_endpoint(const rfcomm_endpoint& other);
#if defined(BOOST_ASIO_HAS_MOVE)
   /// Move constructor.
   rfcomm_endpoint(const rfcomm_endpoint&& other);
#endif // defined(BOOST_ASIO_HAS_MOVE)

   address_type* data() const;

   bdaddr_t addr() const;
   uint8_t channel() const;


   protocol_type protocol() const;

   /// Assign from another endpoint.
   rfcomm_endpoint& operator=(const rfcomm_endpoint& other);

#if defined(BOOST_ASIO_HAS_MOVE)
   /// Move-assign from another endpoint.
   rfcomm_endpoint& operator=(const rfcomm_endpoint&& other);
#endif // defined(BOOST_ASIO_HAS_MOVE)

   /// Get the underlying size of the endpoint in the native type.
   std::size_t size() const;

   /// resize, not really.
   void resize(std::size_t size);

   /// Get the capacity of the endpoint in the native type.
   std::size_t capacity() const;

   /// Compare two endpoints for equality.
   friend bool operator==(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2);
   /// Compare two endpoints for inequality.
   friend bool operator!=(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2);
   /// Compare endpoints for ordering.
   friend bool operator<(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2);
   /// Compare endpoints for ordering.
   friend bool operator>(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2);
   /// Compare endpoints for ordering.
   friend bool operator<=(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2);
   /// Compare endpoints for ordering.
   friend bool operator>=(const rfcomm_endpoint& e1, const rfcomm_endpoint& e2);

private:
   sockaddr_rc addr_;
};


} // namespace bluetooth
} // namespace asio
} // namespace boost

//std::ostream& operator<<(std::ostream& os, const bdaddr_t& addr);
//std::ostream& operator<<(std::ostream& os, const boost::asio::bluetooth::rfcomm_endpoint& endpoint);

#include <boost/asio/bluetooth/detail/rfcomm_endpoint.ipp>

#endif
