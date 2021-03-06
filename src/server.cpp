#include "server.hpp"

Server::Server(boost::asio::io_service &io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234))
{
    start_accept();
}

void Server::start_accept()
{
    // Get connection pointer
    con_handler::pointer connection = con_handler::create(acceptor_.get_io_service());

    // Asynchronous accept operation and wait for a new connection.
    acceptor_.async_accept(connection->socket(),
                           boost::bind(&Server::handle_accept, this, connection,
                                       boost::asio::placeholders::error));
}

void Server::handle_accept(con_handler::pointer connection, const boost::system::error_code &err)
{
    if (!err)
    {
        connection->start();
    }

    start_accept();
}