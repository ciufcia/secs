add_executable(entityCreationTest entityCreationTest.cpp)
target_link_libraries(entityCreationTest PUBLIC secs)
add_test(NAME entity-creation-test COMMAND entityCreationTest)
add_executable(entityArchetypeManagementTest entityArchetypeManagementTest.cpp)
target_link_libraries(entityArchetypeManagementTest PUBLIC secs)
add_test(NAME entity-archetype-management-test COMMAND entityArchetypeManagementTest)
set_tests_properties(
        entity-creation-test
        entity-archetype-management-test
        PROPERTIES LABELS
        "EntityManager;SECS"
)