add_executable(systemActivationTest systemActivationTest.cpp)
target_link_libraries(systemActivationTest PUBLIC secs)
add_test(NAME system-activation-test COMMAND systemActivationTest)
add_executable(systemDependenciesManagementTest systemDependenciesManagementTest.cpp)
target_link_libraries(systemDependenciesManagementTest PUBLIC secs)
add_test(NAME system-dependencies-management-test COMMAND systemDependenciesManagementTest)
add_executable(systemEntityManagementTest systemEntityManagementTest.cpp)
target_link_libraries(systemEntityManagementTest PUBLIC secs)
add_test(NAME system-entity-management-test COMMAND systemEntityManagementTest)
set_tests_properties(
        system-activation-test
        system-dependencies-management-test
        system-entity-management-test
        PROPERTIES LABELS
        "SystemManager;SECS"
)