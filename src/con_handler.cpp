#include "con_handler.hpp"

extern std::unordered_map<std::string, con_handler *> conn_map;

// Constructor
con_handler::con_handler(boost::asio::io_service &io_service) : sock(io_service)
{
}

// Start function, bind read and write function
void con_handler::start()
{
    sock.async_read_some(
        boost::asio::buffer(data, max_length),
        boost::bind(&con_handler::handle_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

// Read handler
void con_handler::handle_read(const boost::system::error_code &err, size_t bytes_transferred)
{
    if (!err)
    {
        // Receive string
        std::string str = data;
        str = str.substr(0, bytes_transferred);

        // Parse JSON
        try
        {
            // Get input json
            auto j = json::parse(str);

            // Handle request
            this->handle_request(j);
            // con_handler::pointer con = conn_map[0];

            // con.get()->socket().async_write_some(
            //     boost::asio::buffer(message, max_length),
            //     boost::bind(&con_handler::handle_write,
            //                 shared_from_this(),
            //                 boost::asio::placeholders::error,
            //                 boost::asio::placeholders::bytes_transferred));
        }
        catch (const json::exception &e)
        {
            // Catch JSON exception when parsing fails
            std::cerr << e.what() << '\n';
        }
    }
    else
    {
        std::cerr << "error: " << err.message() << std::endl;
        sock.close();
    }

    // Schedule next read function
    start();
}

// Write handler
void con_handler::handle_write(const boost::system::error_code &err, size_t bytes_transferred)
{
    if (!err)
    {
        cout << "Server sent message!" << endl;
    }
    else
    {
        std::cerr << "error: " << err.message() << endl;
        sock.close();
    }
}

void con_handler::handle_request(json j)
{
    std::string method = j["method"];

    switch (m[method])
    {
    // Handle Login action
    case 1:
        handle_login(j);
        break;
    case 3:
        handle_send_msg(j);
        break;
    default:
        break;
    }
}

void con_handler::handle_login(json j)
{
    // Get username and password
    string username = j["username"];
    string password = j["password"];

    // Perform some validation
    conn_map[username] = this;

    sock.async_write_some(
        boost::asio::buffer("You have logged in to the account", max_length),
        boost::bind(&con_handler::handle_write,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void con_handler::handle_send_msg(json j)
{
    // Get the message body
    string message = j["message"];
    string username = j["username"];

    conn_map[username]->socket().async_write_some(
            boost::asio::buffer(message, max_length),
            boost::bind(&con_handler::handle_write,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}