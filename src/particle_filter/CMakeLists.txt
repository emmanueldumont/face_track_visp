cmake_minimum_required(VERSION 2.8.3)


add_library(detectTracking detectionTracking.cpp)

add_library(mainPF mainPF.cpp)
add_library(condens condens.cpp)
add_library(filter filter.cpp)
add_library(hist hist.cpp)
add_library(lbp lbp.cpp)
add_library(selector selector.cpp)

add_executable(particle_filter particleFilter.cpp)
target_link_libraries(particle_filter detectTracking mainPF filter condens hist lbp selector ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
