cmake_minimum_required(VERSION 3.10.2)

project(demo)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(OpenCV_DIR "/home/iirc/workspace/thirdparty/opencv-3.4.12/output/share/OpenCV")
set(cppzmq_DIR "/home/iirc/workspace/thirdparty/cppzmq-4.9.0/output/share/cmake/cppzmq")
set(portaudio_DIR "/home/iirc/workspace/thirdparty/portaudio_install/lib/cmake/portaudio")

set(code_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(code_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)




find_package(OpenCV REQUIRED)
find_package(cppzmq REQUIRED)
find_package(portaudio REQUIRED)


message("WARNING" "OpenCV_INCLUDE_DIRS:" ${OpenCV_INCLUDE_DIRS})
message("WARNING" "OpenCV_LIBS: " ${OpenCV_LIBS} )
message("WARNING" "ZeroMQ_INCLUDE_DIR:" ${cppzmq_INCLUDE_DIR})
message("WARNING" "ZeroMQ_LIBRARY:" cppzmq)

file(GLOB SOURCE_FILES "${code_SRC}/*")

message("the source file has:" ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS} ${cppzmq_INCLUDE_DIR} ${WIRINGPI_INCLUDE_DIRS} ${code_INCLUDE})


add_executable(multicamera_multhread multicamera_multithread.cpp ${SOURCE_FILES})
target_link_libraries(multicamera_multhread ${OpenCV_LIBS} cppzmq portaudio)


# add_executable(calibration calibration_data.cpp)
# target_link_libraries(calibration ${OpenCV_LIBS})

# add_executable(cylinder_warp cylinder_warping.cpp)
# target_link_libraries(cylinder_warp ${OpenCV_LIBS} cppzmq)

# add_executable(multicamera multicamera.cpp)
# target_link_libraries(multicamera ${OpenCV_LIBS} ${WIRINGPI_LIBRARIES})

# add_executable(multicameraUSB multicameraUSB.cpp)
# target_link_libraries(multicameraUSB ${OpenCV_LIBS})


