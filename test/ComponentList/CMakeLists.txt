add_executable(componentListTest componentListTest.cpp)
target_link_libraries(componentListTest PUBLIC secs)
add_test(NAME component-list-test COMMAND componentListTest)
set_tests_properties(
        component-list-test
        PROPERTIES LABELS
        "ComponentList;ComponentManager;SECS"
)