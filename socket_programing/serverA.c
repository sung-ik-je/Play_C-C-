#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[256];

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 소켓 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 바인딩
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("바인딩 실패");
        exit(EXIT_FAILURE);
    }

    printf("서버 시작. 클라이언트 데이터 수신 대기 중...\n");

    while (1) {
      ssize_t bytes_received = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
      if (bytes_received == -1) {
          perror("데이터 수신 실패");
          exit(EXIT_FAILURE);
      }
      printf("수신한 데이터: %s\n", buffer);
    }

    // 클라이언트에 응답
    const char* message = "서버에서 응답: Hello, client!";
    ssize_t bytes_sent = sendto(server_fd, message, strlen(message), 0, (struct sockaddr*)&client_addr, client_len);
    if (bytes_sent == -1) {
        perror("데이터 전송 실패");
        exit(EXIT_FAILURE);
    }

    // 소켓 닫기
    // close(server_fd);

    return 0;
}