#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12000
#define SERVER_IP "127.0.0.1"

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[256];

    // 소켓 생성
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("주소 변환 실패");
        exit(EXIT_FAILURE);
    }

    // 데이터 전송
    const char* message = "안녕하세요, 서버!";
    ssize_t bytes_sent = sendto(client_fd, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bytes_sent == -1) {
        perror("데이터 전송 실패");
        exit(EXIT_FAILURE);
    }

    // 데이터 수신
    ssize_t bytes_received = recvfrom(client_fd, buffer, sizeof(buffer), 0, NULL, NULL);
    if (bytes_received == -1) {
        perror("데이터 수신 실패");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0'; // 문자열 끝을 표시하기 위해 널 문자 추가
    printf("수신한 데이터: %s\n", buffer);

    // 소켓 닫기
    close(client_fd);

    return 0;
}