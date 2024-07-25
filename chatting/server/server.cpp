#include <thread>
#include <arpa/inet.h>
#include <queue>
#include <iostream>
#include <unistd.h>

class MessageQueue {
public:
    void push(const std::string& message) {
        std::unique_lock<std::mutex> lock(mtx);   // 임계 구역 잠금
        q.push(message);
        cv.notify_one();  // 대기 중인 스레드(pop에서의 wait thread) 깨워 큐에서 메시지를 꺼낼 수 있도록 알림
    }

    std::string pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !q.empty(); });    // 큐가 빈 경우 mutex 반환 및 스레드 대기 상태 전환, 큐가 비어 있지 않은 경우 mutex 획득 및 스레드 실행
        std::string message = q.front();
        q.pop();
        return message;
    }

private:
    std::queue<std::string> q;
    std::mutex mtx;
    std::condition_variable cv;
};

void process_messages(MessageQueue& mq) {
    while (true) {
        std::string message = mq.pop();   // 비어 있는 경우 빈 메시지 반환
        std::cout << "Processed message: " << message << std::endl;
    }
}

void handle_client(int client_socket, MessageQueue& mq) {
    char buffer[1024];
    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            close(client_socket);
            break;
        }
        std::string message(buffer);
        mq.push(message);
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);   // 동시에 최대 연결 가능 Client

    MessageQueue mq;
    std::thread processor(process_messages, std::ref(mq));  // 객체 복사본이 아닌 참조 형태로 제공

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);  
        std::thread(handle_client, client_socket, std::ref(mq)).detach(); // 스레드 생성 후 독립적으로 실행되도록(부모-자식 관계 끊는)
        /*
        std::thread th()와 같은 thread 기본 형태와 detach와의 차이점 유의
        기본 형태의 경우 부모(여기선 main) 스레드와 상관 관계 존재, main이 종료되면 thread도 종료 => main이 스레드 상태 관리가 용이
        detach의 경우 부모와 별도로 실행, 메인 스레드는 계속해서 client의 연결을 받을 수 있다(비동기적 운영), 하지만 부모 입장에서 thread 관리 방법 존재 X
        thread 종료 시 resource 자동 반환되며 

        만약 생성할 thread의 종류가 정해져 있는 경우라면 기본 형태로 정의해도 되겠지만 현재 토이 프로젝트처럼 다수의 client의 요청을 수신하는 경우라면 
        모든 client들과 관련된 thread들을 하나 하나 join을 통해 운영할 수 없다
        */
    }

    processor.join();
    close(server_socket);
    return 0;
}
