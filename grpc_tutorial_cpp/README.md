


# Server
.proto에 정의한 service는 추상 클래스 형태로 자동으로 셍성된다. 추상 메서드를 구현체에서 직접 구현하여 동작한다.<br>
ServerBuilder는 gRPC 서버 객체를 생성하고 구성하는 역할을 한다.
- 현재 코드에선 Listen Port 추가하고 service를 등록하고 있다.
- BuildAndStart()로 서버를 실행 가능한 상태로 빌드하고 시작한다.(서버 객체를 생성)
  - 서버는 데몬 형태로 실행되며 명시적으로 중단될 때까지 실행 상태를 유지하며 클라이언트 요청을 계속 대기한다. 


<br><br>

# Client
Channel은 클라이언트와 서버 간 통신 채널을 나타내며 클라이언트는 이 채널을 통해 gRPC 호출을 수행한다.<br>
Stub은 클라이언트가 서버의 gRPC 메서드를 호출할 수 있게 해주는 프록시 객체로 서버 측에서 정의한 gRPC 인터페이스에 해당하는 클라이언트 구현체라고 보면 된다.
- 클라이언트는 Stub을 통해 서버의 메서드를 호출하며 Stub 내부에서 채널을 통해 서버와 통신이 이루어진다.
- 서버의 원격 메서드 호출을 추상화한 것
<br>
stub을 이용해 함수를 호출할 때 메타데이터, 요청문, 응답 받을 포인터 변수 3가지를 매개변수로 사용하고 있다.


<br><br>



# proto 파일 컴파일 명령어
protoc -I=. --grpc_out=. --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin) test.proto
protoc -I=. --cpp_out=. test.proto