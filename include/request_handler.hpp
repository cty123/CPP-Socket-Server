#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

using json = nlohmann::json;

class request_handler
{
  private:
    const std::unordered_map<std::string, int> m{
        {"login", 1 },
        {"register", 2 },
        {"send_message", 3 },
    };
  public:
    request_handler();
    json handle_request(json);
    ~request_handler();
};

request_handler::request_handler()
{
}

request_handler::~request_handler()
{
}
