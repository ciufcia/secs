add_executable(componentManager.test componentManager.test.cpp)
target_link_libraries(componentManager.test PUBLIC secs)
add_test(NAME component-manager-test COMMAND componentManager.test)
