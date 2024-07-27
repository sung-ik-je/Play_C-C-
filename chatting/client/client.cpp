#include <iostream>
#include <cstring>     // std::memset
#include <unistd.h>    // close
#include <arpa/inet.h> // sockaddr_in, inet_addr
#include <sys/socket.h> // socket, connect, send, recv

int main() {
    // 서버 주소 및 포트 설정
    const char* server_ip = "127.0.0.1";  // 서버 IP 주소
    int server_port = 8080;               // 서버 포트

    // 소켓 생성
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // 서버 주소 구조체 설정
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = inet_addr(server_ip);

    // 서버에 연결
    if (connect(client_socket, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed\n";
        close(client_socket);
        return 1;
    }

    // 서버로 데이터 전송
    const char* message = "Hello, server!";
    if (send(client_socket, message, std::strlen(message), 0) < 0) {
        std::cerr << "Send failed\n";
        close(client_socket);
        return 1;
    }

    // 서버로부터 데이터 수신
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        std::cerr << "Receive failed\n";
        close(client_socket);
        return 1;
    }

    std::cout << "Received from server: " << buffer << std::endl;

    // 소켓 닫기
    // close(client_socket);

    while(true) { }

    return 0;
}
