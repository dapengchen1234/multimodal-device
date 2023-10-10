cmake_minimum_required(VERSION 3.12)
project(zmq_video_stream)


set(OpenCV_DIR "C://Users//chan//workspace//workspace_code//cpp_projects//opencv//install")
set(cppzmq_DIR "C://Users//chan//workspace//workspace_code//cpp_projects//cppzmq_install//share//cmake//cppzmq")



find_package(OpenCV REQUIRED)
find_package(cppzmq REQUIRED)

message("WARNING" "OpenCV_INCLUDE_DIRS:" ${OpenCV_INCLUDE_DIRS})
message("WARNING" "OpenCV_LIBS: " ${OpenCV_LIBS} )
message("WARNING" "ZeroMQ_INCLUDE_DIR:" ${cppzmq_INCLUDE_DIR})
message("WARNING" "ZeroMQ_LIBRARY:" cppzmq)


add_executable(zmq_video_stream main.cpp)
target_link_libraries(zmq_video_stream PRIVATE ${OpenCV_LIBS} cppzmq)