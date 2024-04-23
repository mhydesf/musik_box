add_library(${MB_LIB} INTERFACE)
target_include_directories(${MB_LIB} INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp
    ${CMAKE_CURRENT_SOURCE_DIR}/tool
)
