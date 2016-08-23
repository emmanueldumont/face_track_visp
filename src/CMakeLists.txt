cmake_minimum_required(VERSION 2.8.3)


add_library(bodyDetection bodyDetection.cpp)

add_executable(visp_detect vispDetection.cpp)
target_link_libraries(visp_detect bodyDetection ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} /usr/lib/x86_64-linux-gnu/libopencv_nonfree.so)
