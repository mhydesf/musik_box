enable_testing()
file(GLOB TEST_SOURCES "*.cpp")

foreach(testSrc ${TEST_SOURCES})
    get_filename_component(testName ${testSrc} NAME_WE)
    add_executable(${testName} ${testSrc})
    target_link_libraries(${testName} musik_box_lib)
    add_test(NAME ${testName} COMMAND ${testName})
endforeach(testSrc)
