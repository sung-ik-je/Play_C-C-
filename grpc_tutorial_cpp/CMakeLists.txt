cmake_minimum_required(VERSION 3.10)
project(testWorldGrpc)

find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

add_executable(server server.cpp test.pb.cc test.grpc.pb.cc)
target_link_libraries(server gRPC::grpc++ protobuf::libprotobuf)

add_executable(client client.cpp test.pb.cc test.grpc.pb.cc)
target_link_libraries(client gRPC::grpc++ protobuf::libprotobuf)