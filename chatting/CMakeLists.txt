# CMake의 최소 버전을 설정합니다.
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 버전을 설정합니다.
project(MessageQueueServer VERSION 1.0)

# C++17을 사용하도록 설정합니다.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(${PROJECT_SOURCE_DIR}/server)
add_subdirectory(${PROJECT_SOURCE_DIR}/client)

# 링크할 라이브러리 목록을 설정합니다.
# 이 예제에서는 별도의 라이브러리가 필요하지 않지만, 
# 필요시 아래와 같이 추가할 수 있습니다.
# target_link_libraries(MessageQueueServer <library_name>)
