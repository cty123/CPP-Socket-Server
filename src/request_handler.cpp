#include "request_handler.hpp"

json request_handler::handle_request(json j)
{
    try
    {
        std::string method = j["method"];

    }
    catch (const json::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}