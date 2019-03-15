#include "server.hpp"
#include <map>
#include <vector>

std::unordered_map<std::string, con_handler *> conn_map;

int main()
{
    try
    {
        // Create IO service
        boost::asio::io_service io_service;
        cout << "Server starting" << endl;
        // Starts server to run the service
        Server server(io_service);
        // Run service
        io_service.run();
    }
    catch (std::exception &e)
    {   
        // Catch server creation error
        std::cerr << e.what() << endl;
    }
    return 0;
}