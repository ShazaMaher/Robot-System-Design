cmake_minimum_required(VERSION 3.5)
project(testpkg)

find_package(ur_rtde REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  )

add_message_files(
  FILES
  Status.msg)

generate_messages(
  DEPENDENCIES 
  std_msgs)

catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(test_node
        src/test_node.cpp
        src/SubsAndPubs.cpp
        src/SubsAndPubs.h
        src/rsdController.cpp
        src/rsdController.h
		src/global_def.h)
add_executable(io_node
	src/io_node.cpp
		src/global_def.h)

add_executable(test_node_packml
		src/test_node_packml.cpp
		src/global_def.h)

add_executable(em_node
		src/EmergencyStop.cpp
		src/SubsAndPubs.cpp
        src/SubsAndPubs.h
        src/rsdController.cpp
        src/rsdController.h)

QT5_WRAP_UI(UI_HDRS src/test_HMI/mainwindow.ui)
add_executable(test_HMI
		src/test_HMI/main.cpp
		src/test_HMI/mainwindow.cpp
		src/test_HMI/mainwindow.h
                ${XR_FORMS} 
                ${UI_HDRS}
		src/global_def.h)

add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
 set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

target_compile_features(em_node PUBLIC cxx_std_11)
target_compile_features(test_node PUBLIC cxx_std_11)
target_compile_features(io_node PUBLIC cxx_std_11)
target_compile_features(test_HMI PUBLIC cxx_std_11)
target_compile_features(test_node_packml PUBLIC cxx_std_11)


target_link_libraries(em_node ${catkin_LIBRARIES} ur_rtde::rtde)
target_link_libraries(test_node ${catkin_LIBRARIES} ur_rtde::rtde)
target_link_libraries(io_node ${catkin_LIBRARIES} ur_rtde::rtde)
target_link_libraries(test_HMI ${catkin_LIBRARIES} ${Qt5Widgets_LIBRARIES})
target_link_libraries(test_node_packml ${catkin_LIBRARIES})

