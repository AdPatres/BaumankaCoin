#pragma once

#ifndef CONNECTION_H
#define CONNECTION_H

#include "./messages/message.hpp"

#include <iostream>
#include <memory> // shared_ptr
#include <utility>

#include <boost/asio.hpp>

namespace serverd
{
  class connection // network connection repr
  {
  public:
    using pointer = std::shared_ptr<connection>;

    static pointer
    create(boost::asio::io_service& ios)
    {
      return pointer(new connection(ios));
    }

    boost::asio::ip::tcp::socket&
    socket()
    {
      return m_sock;
    }

    template <typename Message>
    void
    send(const Message& payload); // send message

    std::pair<std::string, messages::payload_t>
    receive(); // receive message, return <command, payload>

  private:
    connection(boost::asio::io_service&);

    boost::asio::ip::tcp::socket m_sock;
  };

  template <typename Message>
  void
  connection::send(const Message& msg)
  {
    messages::payload_t payload;
    payload << msg;
    m_sock.send(boost::asio::buffer(msg.command, sizeof(msg.command)));
    uint32_t size = static_cast<uint32_t>(payload.size());
    m_sock.send(boost::asio::buffer(&size, sizeof(size)));
    m_sock.send(boost::asio::buffer(payload.data(), payload.size()));
    std::cerr << "send " << msg.command << " message" << std::endl;
  }
} // namespace serverd

#endif // CONNECTION_H
