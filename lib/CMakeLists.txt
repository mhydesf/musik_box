set(MB_LIB ${PROJECT_NAME}_lib)

add_library(${MB_LIB} INTERFACE)
target_include_directories(${MB_LIB} INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
)
