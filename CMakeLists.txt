cmake_minimum_required(VERSION 3.20)
project(VulkanGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

IF (WIN32)
    add_compile_options(/GX)
ELSE ()
    add_compile_options(-fexceptions)
ENDIF ()

find_package(Vulkan REQUIRED)
find_package (Eigen3 REQUIRED)
find_package (glm REQUIRED)
# find_package(SFML REQUIRED network audio)

IF (WIN32)
    add_subdirectory(C:/src/glfw-3.3.4 glfw)
ELSE ()
    find_package(glfw3 REQUIRED)
ENDIF ()

add_subdirectory(shaders)

add_executable(VulkanGame
        src/main.cpp
        src/debug.cpp src/window.cpp src/window.hpp)

target_include_directories(VulkanGame PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(VulkanGame ${Vulkan_LIBRARIES} glfw shaders glm)
# target_link_libraries(VulkanGame ${SFML_LIBRARIES})
