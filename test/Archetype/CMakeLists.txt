add_executable(archetypeTest archetypeTest.cpp)
target_link_libraries(archetypeTest PUBLIC secs)
add_test(NAME archetype-test COMMAND archetypeTest)
add_executable(checkIfSubarchetypeTest checkIfSubarchetypeTest.cpp)
target_link_libraries(checkIfSubarchetypeTest PUBLIC secs)
add_test(NAME check-if-subarchetype-test COMMAND checkIfSubarchetypeTest)
set_tests_properties(
        archetype-test
        check-if-subarchetype-test
        PROPERTIES LABELS
        "Archetype;EntityManager;SystemManager;SECS"
)