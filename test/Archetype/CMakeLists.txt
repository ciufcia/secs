add_executable(setComponent.test setComponent.test.cpp)
target_link_libraries(setComponent.test PUBLIC secs)
add_test(NAME set-component-test COMMAND setComponent.test)

add_executable(archetypeComparison.test archetypeComparison.test.cpp)
target_link_libraries(archetypeComparison.test PUBLIC secs)
add_test(NAME archetype-comparison-test COMMAND archetypeComparison.test)

add_executable(checkIfSubArchetype.test checkIfSubArchetype.test.cpp)
target_link_libraries(checkIfSubArchetype.test PUBLIC secs)
add_test(NAME check-if-sub-archetype-test COMMAND checkIfSubArchetype.test)
