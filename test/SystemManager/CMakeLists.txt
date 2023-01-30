add_executable(dependencyManagement.test dependencyManagement.test.cpp)
target_link_libraries(dependencyManagement.test PUBLIC secs)
add_test(NAME dependency-management-test COMMAND dependencyManagement.test)

add_executable(entityManagement.test entityManagement.test.cpp)
target_link_libraries(entityManagement.test PUBLIC secs)
add_test(NAME entity-management-test COMMAND entityManagement.test)
