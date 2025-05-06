cmake_minimum_required(VERSION 3.5)
project(magnet_sub_test)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# wiringPi ヘッダとライブラリを探す
find_path(WIRINGPI_INCLUDE_DIR wiringPi.h
  PATHS
    /usr/local/include
    /usr/include
)

find_library(WIRINGPI_LIBRARY wiringPi
  PATHS
    /usr/local/lib
    /usr/lib/aarch64-linux-gnu
    /usr/lib
)

if(NOT WIRINGPI_INCLUDE_DIR OR NOT WIRINGPI_LIBRARY)
  message(FATAL_ERROR "wiringPi が見つかりません (/usr/local/lib か /usr/lib/aarch64-linux-gnu に libwiringPi.so を置いてください)。")
endif()

include_directories(${WIRINGPI_INCLUDE_DIR})

add_executable(magnet_sub_test src/magnet_sub_test.cpp)
ament_target_dependencies(magnet_sub_test rclcpp std_msgs)
target_link_libraries(magnet_sub_test ${WIRINGPI_LIBRARY})

install(TARGETS magnet_sub_test
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
