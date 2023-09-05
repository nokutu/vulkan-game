cmake_minimum_required(VERSION 3.20)
project(VulkanGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

IF (WIN32)
    add_compile_options(/EHsc)
ELSE ()
    add_compile_options(-fexceptions)
ENDIF ()

find_package(Vulkan REQUIRED)
# find_package(SFML REQUIRED network audio)
find_package(fmt REQUIRED)

IF (WIN32)
    add_subdirectory(C:/src/glfw-3.3.4 glfw)
ELSE ()
    find_package(glfw3 REQUIRED)
ENDIF ()

add_subdirectory(shaders)

add_executable(VulkanGame
        src/main.cpp
        src/debug.cpp src/window.cpp src/object.cpp
        src/renderer.cpp
        src/engine/graphics_pipeline.cpp
        src/engine/render_pass.cpp
        src/engine/swap_chain.cpp
        src/engine/queue_families.cpp
        src/engine/physical_device.cpp
        src/engine/logical_device.cpp
        src/engine/frame_buffers.cpp
        src/primitives/base_primitive.cpp
        src/primitives/base_primitive.hpp
)

target_include_directories(VulkanGame PUBLIC ${Vulkan_INCLUDE_DIRS} C:/src/eigen)
target_precompile_headers(VulkanGame PUBLIC headers.pch)

target_link_libraries(VulkanGame ${Vulkan_LIBRARIES} glfw shaders fmt::fmt)
# target_link_libraries(VulkanGame ${SFML_LIBRARIES})
