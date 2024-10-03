#include <boost/asio.hpp>
#include <iostream>
#include <string>

// UDP 클라이언트 클래스
class UdpClient {
public:
    UdpClient(boost::asio::io_context& io_context, const std::string& host, unsigned short port)
        : socket_(io_context), endpoint_(boost::asio::ip::address::from_string(host), port) {
        socket_.open(boost::asio::ip::udp::v4());
    }

    void sendProcess() {
      std::string message = "Hello, UDP server!";
      send(message);
      std::cout << "Message sent: " << message << std::endl;
    }

    void send(const std::string& message) {
        socket_.send_to(boost::asio::buffer(message), endpoint_);
    }

    void receive() {
        std::array<char, 1024> recv_buffer;
        boost::asio::ip::udp::endpoint sender_endpoint;

        // 동기적으로 수신 (필요에 따라 비동기로 변경 가능)
        size_t len = socket_.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);
        std::cout << "Response from server: " << std::string(recv_buffer.data(), len) << std::endl;
    }

private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint endpoint_;
};

int main() {
    try {
        boost::asio::io_context io_context;

        UdpClient client(io_context, "192.168.0.7", 12345);

        client.sendProcess();

        client.receive();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
