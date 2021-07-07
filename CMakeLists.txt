project(cyclonedds_ros2_jointstate_pubsub LANGUAGES C)
cmake_minimum_required(VERSION 3.5)

find_package(CycloneDDS REQUIRED)

add_library(typesupport src/Time.c src/Header.c src/JointState.c)
target_include_directories(typesupport PUBLIC include)

add_executable(publisher publisher.c)
target_link_libraries(publisher CycloneDDS::ddsc typesupport)

add_executable(subscriber subscriber.c)
target_link_libraries(subscriber CycloneDDS::ddsc typesupport)
