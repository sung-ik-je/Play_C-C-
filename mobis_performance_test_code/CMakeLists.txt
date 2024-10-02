cmake_minimum_required(VERSION 3.10)
project(UdpServerClient)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Boost 라이브러리 경로 설정 (필요한 경우 수정)
set(Boost_USE_STATIC_LIBS ON)  # 정적 라이브러리 사용
set(Boost_USE_MULTITHREADED ON)  # 멀티스레드 지원 활성화
set(BOOST_ROOT "/opt/homebrew/opt/boost")  # Boost 설치 경로 지정

# Boost 패키지 찾기 (system 및 asio를 찾도록 설정)
find_package(Boost 1.70 REQUIRED COMPONENTS system thread)

if (Boost_FOUND)
    message(STATUS "Boost found: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Could not find Boost!")
endif()

# 서버 소스 파일
add_executable(server
    server.cpp  # 서버 소스 파일
)

# 클라이언트 소스 파일
add_executable(client
    client.cpp  # 클라이언트 소스 파일
)

# Boost 라이브러리와 링크
target_link_libraries(server
    Boost::system
    Boost::thread
)

target_link_libraries(client
    Boost::system
    Boost::thread
)
