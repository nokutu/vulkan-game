find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

#==============================================================================
# COMPILE SHADERS
#

set(SHADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.frag)

foreach (source IN LISTS SHADERS)
    get_filename_component(FILENAME ${source} NAME)
    add_custom_command(
            COMMAND
            ${glslc_executable}
            -o ${FILENAME}.spv
            ${source}
            OUTPUT ${FILENAME}.spv
            COMMENT "Compiling ${FILENAME}"
    )

    string(REGEX MATCH "\\.(.*)" dummy ${FILENAME})
    string(TOUPPER ${CMAKE_MATCH_1} NAME)

    list(APPEND SHADERS ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.spv)
endforeach ()

add_custom_target(compiled_shaders ALL DEPENDS ${SHADERS})
add_custom_target(bin2hpp COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/load_binaries.cmake)

add_library(shaders STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/shader_manager.cpp)
target_include_directories(shaders PUBLIC include)
target_include_directories(shaders PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(shaders bin2hpp)
add_dependencies(bin2hpp compiled_shaders)


