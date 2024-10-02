#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <thread>
#include <chrono>

// UDP 서버 클래스
class UdpServer {
public:
    UdpServer(boost::asio::io_context& io_context, unsigned short port)
        : socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
          work_guard_(boost::asio::make_work_guard(io_context)) {
        start_receive();
    }

    void start_receive() {
        /*
          비동기적으로 수신
            서버가 비동기적으로 패킷을 수신하는 것은 동시에 여러 클라이언트의 요청을 처리하고자 비동기 이용
            동기적으로 수신하는 경우 패킷 수신 후 작업이 처리되는 동안 추가 패킷을 수신하지 못하는 문제 발생
            비동기로 수신하는 경우 패킷 수신 후 해당 패킷의 대한 작업이 비동기적으로 수행되며 작업이 수행되는 동안 클라이언트로부터 추가적인 요청 처리할 수 있다
        */
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&UdpServer::handle_receive, this, boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            std::string data(recv_buffer_.data(), bytes_transferred);
            std::cout << "Received: " << data << " from " << remote_endpoint_ << std::endl;

            // detach를 이용해 main 스레드와 별도의 스레드를 생성해 verify_data 작업을 처리하고 작업 처리가 끝난 경우 종료
            std::thread(&UdpServer::verify_data, this, data).detach();

            // 만약 start_receive()를 다시 호출해주지 않는 경우 main에서 io_context.run()으로 생성한 스레드가 패킷을 수신할 때마다 스레드가 1개씩 줄어든다
            start_receive();
        } else {
            std::cerr << "Error on receive: " << error.message() << std::endl;
        }
    }

    // 검증 작업 (비동기적으로 실행됨)
    void verify_data(const std::string& data) {
        std::this_thread::sleep_for(std::chrono::seconds(1));  // 검증 작업 시뮬레이션 (1초 딜레이)
        std::cout << "Verified: " << data << std::endl;
    }

private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;  // io_context 종료 방지
};

int main() {
    try {
        boost::asio::io_context io_context;   // I/O 처리하는 Event Loop 역할, 특정 작업을 구분하지 않고 application에서 사용하는 모든 비동기 이벤트 다룸

        // UDP 서버를 12345 포트에서 실행
        UdpServer server(io_context, 12345);

        // IO 스레드 풀 구성
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([&io_context]() { io_context.run(); });    // io_context(event loop)에 등록된 작업을 처리할 thread 생성
        }

        // 스레드 종료 대기
        for (auto& t : threads) {
            t.join();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
