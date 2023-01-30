add_executable(entityToSystemMapping.test entityToSystemMapping.test.cpp)
target_link_libraries(entityToSystemMapping.test PUBLIC secs)
add_test(NAME entity-to-system-mapping-test COMMAND entityToSystemMapping.test)