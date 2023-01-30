#include <secs.hpp>

#define failIfNotThrew(b) if (b) { b = false; } else { return 1; }

int main() {
    secs::EntityManager entityManager {};

    bool threw = false;

    secs::Entity entity1 = entityManager.createEntity();
    secs::Entity entity2 = entityManager.createEntity();

    if (!(entity1 == 0 && entity2 == 1)) { return 1; }

    entityManager.deleteEntity(entity1);

    entity1 = entityManager.createEntity();

    if (!(entity1 == 0 && entity2 == 1)) { return 1; }

    entityManager.deleteEntity(entity1);

    try { entityManager.getEntityArchetype(entity1); }
    catch(secs::EntityDoesntExist) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    return 0;
}