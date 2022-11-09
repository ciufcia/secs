add_executable(SECSEntityCreationTest SECSEntityCreationTest.cpp)
target_link_libraries(SECSEntityCreationTest PUBLIC secs)
add_test(NAME secs-entity-creation-test COMMAND SECSEntityCreationTest)
add_executable(SECSComponentManagementTest SECSComponentManagementTest.cpp)
target_link_libraries(SECSComponentManagementTest PUBLIC secs)
add_test(NAME secs-component-management-test COMMAND SECSComponentManagementTest)
add_executable(SECSSystemManagementTest SECSSystemManagementTest.cpp)
target_link_libraries(SECSSystemManagementTest PUBLIC secs)
add_test(NAME secs-system-management-test COMMAND SECSSystemManagementTest)
add_executable(SECSSystemDependenciesTest SECSSystemDependenciesTest.cpp)
target_link_libraries(SECSSystemDependenciesTest PUBLIC secs)
add_test(NAME secs-system-dependencies-test COMMAND SECSSystemDependenciesTest)
set_tests_properties(
        secs-entity-creation-test
        secs-component-management-test
        secs-system-management-test
        secs-system-dependencies-test
        PROPERTIES LABELS
        "SECS"
)