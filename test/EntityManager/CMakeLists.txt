add_executable(entityCreation.test entityCreation.test.cpp)
target_link_libraries(entityCreation.test PUBLIC secs)
add_test(NAME entity-creation-test COMMAND entityCreation.test)

add_executable(archetypeManagement.test archetypeManagement.test.cpp)
target_link_libraries(archetypeManagement.test PUBLIC secs)
add_test(NAME archetype-management-test COMMAND archetypeManagement.test)

add_executable(getEntities.test getEntities.test.cpp)
target_link_libraries(getEntities.test PUBLIC secs)
add_test(NAME get-entities-test COMMAND getEntities.test)