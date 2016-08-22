cmake_minimum_required(VERSION 2.8.3)
project(movector)


find_package(catkin REQUIRED COMPONENTS roscpp std_msgs sensor_msgs cv_bridge)

find_package(OpenCV)

catkin_package(
   INCLUDE_DIRS include
)

include_directories( 
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)


install(TARGETS
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

add_subdirectory(src)
