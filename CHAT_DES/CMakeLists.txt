cmake_minimum_required(VERSION 3.4)
project(chat)

set(CMAKE_CXX_STANDARD 11)

add_executable(server server.cpp)
add_executable(client client.cpp)

# OpenSSL 라이브러리를 찾습니다.
find_package(OpenSSL REQUIRED)

# OpenSSL 헤더 파일을 포함합니다.
set(OPENSSL_ROOT_DIR /usr/local)
include_directories(${OPENSSL_ROOT_DIR}/include)

# OpenSSL 라이브러리를 연결합니다.
target_link_libraries(server ${OPENSSL_LIBRARIES})
target_link_libraries(client ${OPENSSL_LIBRARIES})