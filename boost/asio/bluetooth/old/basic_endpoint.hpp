//
// bluetooth/basic_endpoint.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Scott Bailey
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_BLUETOOTH_BASIC_ENDPOINT_HPP
#define BOOST_ASIO_BLUETOOTH_BASIC_ENDPOINT_HPP

#include <bluetooth/bluetooth.h>
#include <boost/asio/bluetooth/address.hpp>
#include <boost/asio/bluetooth/detail/endpoint.hpp>


namespace boost {
namespace asio {
namespace bluetooth {


template<typename BluetoothProtocol>
class bluetooth_endpoint
{
public:
   /// The protocol type associated with the endpoint.
   typedef BluetoothProtocol protocol_type;

   /// The type of the endpoint structure. This type is dependent on the
   /// underlying implementation of the socket layer.
#if defined(GENERATING_DOCUMENTATION)
   typedef implementation_defined data_type;
#else
   typedef boost::asio::bluetooth::detail::socket_addr_type data_type;
#endif



   /// Default constructor.
   bluetooth_endpoint()
      : impl_()
   {
   }

   /// Construct an endpoint using a port number and a Bluetooth address. This
   /// constructor may be used for accepting connections on a specific interface
   /// or for making a connection to a remote endpoint.
   bluetooth_endpoint(const boost::asio::bluetooth::address& addr, unsigned short port_num)
      : impl_(addr, port_num)
   {
   }

   /// Copy constructor.
   bluetooth_endpoint(const bluetooth_endpoint& other)
      : impl_(other.impl_)
   {
   }

#if defined(BOOST_ASIO_HAS_MOVE)
   /// Move constructor.
   bluetooth_endpoint(bluetooth_endpoint&& other)
      : impl_(other.impl_)
   {
   }
#endif // defined(BOOST_ASIO_HAS_MOVE)

   /// Assign from another endpoint.
   bluetooth_endpoint& operator=(const bluetooth_endpoint& other)
   {
      impl_ = other.impl_;
      return *this;
   }

#if defined(BOOST_ASIO_HAS_MOVE)
   /// Move-assign from another endpoint.
   bluetooth_endpoint& operator=(bluetooth_endpoint&& other)
   {
      impl_ = other.impl_;
      return *this;
   }
#endif // defined(BOOST_ASIO_HAS_MOVE)

   /// Get the underlying endpoint in the native type.
   data_type* data()
   {
      return impl_.data();
   }

   /// Get the underlying size of the endpoint in the native type.
   std::size_t size() const
   {
      return impl_.size();
   }

   /// Get the capacity of the endpoint in the native type.
   std::size_t capacity() const
   {
      return impl_.capacity();
   }

   /// Get the port associated with the endpoint. The port number is always in
   /// the host's byte order.
   unsigned short port() const
   {
      return impl_.port();
   }

   /// Set the port associated with the endpoint. The port number is always in
   /// the host's byte order.
   void port(unsigned short port_num)
   {
      impl_.port(port_num);
   }

   /// Get the Bluetooth address associated with the endpoint.
   boost::asio::bluetooth::address address() const
   {
      return impl_.address();
   }

   /// Set the Bluetooth address associated with the endpoint.
   void address(const boost::asio::bluetooth::address& addr)
   {
      impl_.address(addr);
   }

   /// Compare two endpoints for equality.
   friend bool operator==(const bluetooth_endpoint<BluetoothProtocol>& e1,
         const bluetooth_endpoint<BluetoothProtocol>& e2)
   {
      return e1.impl_ == e2.impl_;
   }

   /// Compare two endpoints for inequality.
   friend bool operator!=(const bluetooth_endpoint<BluetoothProtocol>& e1,
         const bluetooth_endpoint<BluetoothProtocol>& e2)
   {
      return !(e1 == e2);
   }

   /// Compare endpoints for ordering.
   friend bool operator<(const bluetooth_endpoint<BluetoothProtocol>& e1,
         const bluetooth_endpoint<BluetoothProtocol>& e2)
   {
      return e1.impl_ < e2.impl_;
   }

   /// Compare endpoints for ordering.
   friend bool operator>(const bluetooth_endpoint<BluetoothProtocol>& e1,
         const bluetooth_endpoint<BluetoothProtocol>& e2)
   {
      return e2.impl_ < e1.impl_;
   }

   /// Compare endpoints for ordering.
   friend bool operator<=(const bluetooth_endpoint<BluetoothProtocol>& e1,
         const bluetooth_endpoint<BluetoothProtocol>& e2)
   {
      return !(e2 < e1);
   }

   /// Compare endpoints for ordering.
   friend bool operator>=(const bluetooth_endpoint<BluetoothProtocol>& e1,
         const bluetooth_endpoint<BluetoothProtocol>& e2)
   {
      return !(e1 < e2);
   }

private:
   // The underlying Bluetooth endpoint.
   boost::asio::bluetooth::detail::endpoint impl_;
};

} // namespace bluetooth
} // namespace asio
} // namespace boost


#endif
