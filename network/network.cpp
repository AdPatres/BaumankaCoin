#include "network.hpp"
#include <boost/bind.hpp>



using namespace network;


struct _Node {
  boost::asio::ip::address_v4 addr;
  unsigned short port;

  friend class boost::serialization::access;
  template<typename Archive>
  void serialize(Archive & ar, const unsigned int version)
	{ ar & addr & port; }
}


server::server(boost::asio::io_service& io_service)
: _M_acceptor(io_service, 
              boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 
                                            DEFAULT_PORT))
{
  start_accept();
  io_service.run();
};


peer::peer(boost::asio::io_service& ios)
: _M_socket(ios)
{ }


void
server::start_accept()
{
    peer::pointer new_peer_ptr = peer::create(_M_acceptor.get_io_service());
    if (!_M_acceptor.is_open())
      _M_acceptor.listen();
    _M_acceptor.async_accept(new_peer_ptr->socket(), 
                              boost::bind(&server::handle_accept, this, 
                                        boost::asio::placeholders::error,
                                        new_peer_ptr));
}


void
server::handle_accept(const boost::system::error_code& error, 
                      peer::pointer peer_ptr)
{
  _Node = {peer_ptr->socket().remote_endpoint().address(), 
            peer_ptr->socket().remote_endpoint().port()};
  if (!error)
    peer_ptr->write(std::to_string(peer_ptr->socket().remote_endpoint().port())+'\n');
  this->start_accept();
}


void
peer::write(const string& msg)
{
  
  boost::asio::async_write(_M_socket, boost::asio::buffer(msg), 
                            boost::bind(&peer::write_handler, this,
                                      boost::asio::placeholders::error, 
                                      boost::asio::placeholders::bytes_transferred));
}


void
peer::write_handler(const boost::system::error_code& ec, 
                    std::size_t bytes_transferred)
{

}