set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

find_package(BIN2H REQUIRED)

file(GLOB files "${CMAKE_CURRENT_BINARY_DIR}/*.spv")

foreach (source ${files})
    get_filename_component(FILENAME ${source} NAME)
    string(REGEX MATCH "\\.(.*)\\..*" dummy ${FILENAME})
    string(TOUPPER ${CMAKE_MATCH_1} NAME)
    BIN2H(SOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME} HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_SHADER.hpp VARIABLE_NAME ${NAME}_SHADER)
endforeach ()
