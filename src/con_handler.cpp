#include <nlohmann/json.hpp>

#include "con_handler.hpp"

// For convenience
using json = nlohmann::json;

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
            auto j = json::parse(str);
            cout << j.dump() << endl;
        }
        catch(const json::exception& e)
        {   
            // Catch JSON exception when parsing fails
            std::cerr << e.what() << '\n';
        }

        if (!str.compare("Hello"))
        {
            //Write back data
            sock.async_write_some(
                boost::asio::buffer(message, max_length),
                boost::bind(&con_handler::handle_write,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
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
        cout << "Server sent Hello message!" << endl;
    }
    else
    {
        std::cerr << "error: " << err.message() << endl;
        sock.close();
    }
}