file(GLOB TEST_SRC *.c)

foreach(test_file ${TEST_SRC})
    get_filename_component(test_name ${test_file} NAME_WE)

    add_executable(${test_name} ${test_file})
    
    target_link_libraries(${test_name} PRIVATE davinci_lib kutiles::kutiles)
    
    target_include_directories(${test_name} PRIVATE ../include)

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
