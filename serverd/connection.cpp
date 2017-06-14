#include "connection.hpp"

using namespace serverd;

connection::connection(boost::asio::io_service& ios)
: m_sock(ios)
{ }

std::pair<std::string, messages::payload_t>
connection::receive()
{
  char command[messages::command_size];
  m_sock.receive(boost::asio::buffer(command, sizeof(command)));
  uint32_t size;
  m_sock.receive(boost::asio::buffer(&size, sizeof(size)));
  messages::payload_t payload(size);
  m_sock.receive(boost::asio::buffer(payload.data(), size));

  std::cerr << "get " << command << std::endl;
  return std::make_pair(command, std::move(payload));
}
