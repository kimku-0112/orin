#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <nlohmann/json.hpp>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

void run_client(const std::string& host, const std::string& port) {
    try {
        boost::asio::io_context ioc;
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        auto const results = resolver.resolve(host, port);
        auto ep = boost::asio::connect(ws.next_layer(), results);
        ws.handshake(host, "/");

        // Subscribe to Detect_data
        nlohmann::json sub_detect_data = {
            {"subscribe", {{"topic", "Detect_data"}}}
        };
        ws.write(boost::asio::buffer(sub_detect_data.dump()));

        // Read response
        for(;;) {
            boost::beast::multi_buffer buffer;
            ws.read(buffer);
            std::cout << boost::beast::buffers_to_string(buffer.data()) << std::endl;
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    std::string host = "192.168.0.50";
    std::string port = "3761";

    std::thread t([&host, &port] {
        run_client(host, port);
    });
    t.join();

    return 0;
}
