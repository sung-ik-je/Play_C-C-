cmake_minimum_required(VERSION 3.10)
project(AboutCert)

set(CMAKE_CXX_STANDARD 17)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add executable
add_executable(generate_cert generate_cert.cpp)
add_executable(generate_pk generate_pk.cpp)
add_executable(generate_sign generate_sign.cpp)
add_executable(verify_sign verify_sign.cpp)

# Link OpenSSL libraries
target_link_libraries(generate_cert PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(generate_pk PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(generate_sign PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(verify_sign PRIVATE OpenSSL::SSL OpenSSL::Crypto)
