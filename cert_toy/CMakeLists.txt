cmake_minimum_required(VERSION 3.10)
project(GenerateCert)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add executable
add_executable(generate_cert generate_cert.cpp)
add_executable(generate_pk generate_pk.cpp)

# Link OpenSSL libraries
target_link_libraries(generate_cert PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(generate_pk PRIVATE OpenSSL::SSL OpenSSL::Crypto)
