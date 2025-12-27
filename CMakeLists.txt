cmake_minimum_required(VERSION 3.21)
project(BEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw)
add_subdirectory(external/glm)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

#target_include_directories(BEngine PRIVATE ${SRC_DIR})
include_directories(src)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)


#file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

add_executable(BEngine
    ${SOURCE_FILES}
    "Tests/tests.h")


target_link_libraries(BEngine PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
)
