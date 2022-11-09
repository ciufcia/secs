add_executable(componentManagerTest componentManagerTest.cpp)
target_link_libraries(componentManagerTest PUBLIC secs)
add_test(NAME component-manager-test COMMAND componentManagerTest)
set_tests_properties(
        component-manager-test
        PROPERTIES LABELS
        "ComponentManager;SECS"
)