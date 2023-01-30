add_executable(componentList.test componentList.test.cpp)
target_link_libraries(componentList.test PUBLIC secs)
add_test(NAME component-list-test COMMAND componentList.test)
