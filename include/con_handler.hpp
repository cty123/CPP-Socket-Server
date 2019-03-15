#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>

using json = nlohmann::json;
using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
using std::string;

class con_handler : public boost::enable_shared_from_this<con_handler>
{
  private:
    tcp::socket sock;
    std::string message = "Hello From Server!";
    enum
    {
        max_length = 1024
    };
    char data[max_length];
    std::unordered_map<std::string, int> m{
        {"login", 1 },
        {"register", 2 },
        {"send_message", 3 },
    };
    void handle_request(json j);
    void handle_login(json j);
    void handle_send_msg(json j);
  public:
    // Smart pointer
    typedef boost::shared_ptr<con_handler> pointer;
    // Constructor
    con_handler(boost::asio::io_service &io_service);
    // Create pointer
    static pointer create(boost::asio::io_service &io_service)
    {
        return pointer(new con_handler(io_service));
    }
    // Return socket
    tcp::socket &socket()
    {
        return sock;
    }
    // Start 
    void start();
    // Read function
    void handle_read(const boost::system::error_code &err, size_t bytes_transferred);
    // Write function
    void handle_write(const boost::system::error_code &err, size_t bytes_transferred);
};