#include "message.hpp"

#include <iostream>
#include <string>

using namespace serverd;

net_addr::net_addr(boost::asio::ip::tcp::endpoint endp)
: m_ip(endp.address().to_v4().to_bytes()),
  m_port(static_cast<uint16_t>(endp.port()))
{ }
