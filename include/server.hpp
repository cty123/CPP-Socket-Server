#include "con_handler.hpp"

class Server
{
  private:
    tcp::acceptor acceptor_;
    void start_accept();

  public:
    Server(boost::asio::io_service &io_service);
    void handle_accept(con_handler::pointer connection, const boost::system::error_code &err);
};