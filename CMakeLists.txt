cmake_minimum_required(VERSION 3.25)
project(Code)

set(CMAKE_CXX_STANDARD 11)

add_executable(hello_server CH-1/1.1hello_server.cpp)
add_executable(1.2hello_client CH-1/1.2hello_client.cpp)
add_executable(1.3hello_client CH-1/1.3hello_client.cpp)
add_executable(4.1echo_server CH-4/4.1echo_server.cpp)
add_executable(4.2echo_client CH-4/4.2echo_client.cpp)

add_executable(5.1echo_server CH-5/5.1echo_server.cpp)
add_executable(5.2echo_client CH-5/5.2echo_client.cpp)
add_executable(5.3op_server CH-5/5.3op_server.cpp)
add_executable(5.4op_client CH-5/5.4op_client.cpp)
add_executable(6.1echo_server CH-6/6.1UDP_echo_server.cpp)
add_executable(6.2echo_client CH-6/6.2UDP_echo_client.cpp)
add_executable(6.3echo_client_connect_socket CH-6/6.3UDP_echo_client_connect_socket.cpp)