#include "connection.hpp"

using namespace serverd;

connection::connection(boost::asio::io_service& ios)
: m_sock(ios)
{ }
