cmake_minimum_required(VERSION 3.20)
project(VulkanGame)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(/GX)

find_package(Vulkan REQUIRED)
# find_package(SFML REQUIRED network audio)
# find_package(glfw REQUIRED)


add_executable(VulkanGame main.cpp)

target_include_directories(VulkanGame PUBLIC ${Vulkan_INCLUDE_DIRS})

add_subdirectory(C:/src/glfw-3.3.4 glfw)

target_link_libraries(VulkanGame ${Vulkan_LIBRARIES})
target_link_libraries(VulkanGame glfw)
# target_link_libraries(VulkanGame ${SFML_LIBRARIES})
