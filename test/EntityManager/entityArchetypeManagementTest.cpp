#include <secs.hpp>

#define failIfNotThrew(b) if (b) { b = false; } else { return 1; }

int main() {
    secs::EntityManager entityManager {};

    bool threw = false;

    secs::Entity entity1 = entityManager.createEntity();

    entityManager.getEntityArchetype(entity1);
    entityManager.setEntityArchetype(entity1, secs::Archetype());

    try { entityManager.getEntityArchetype(1); }
    catch (secs::EntityDoesntExist) { threw = true; }
    catch (...) { return 1; }

    failIfNotThrew(threw)

    try { entityManager.setEntityArchetype(1, secs::Archetype()); }
    catch (secs::EntityDoesntExist) { threw = true; }
    catch (...) { return 1; }

    failIfNotThrew(threw)

    secs::Archetype archetype1 = entityManager.getEntityArchetype(entity1);
    archetype1.setComponent(0, true);
    entityManager.setEntityArchetype(entity1, archetype1);
    archetype1 = entityManager.getEntityArchetype(entity1);

    if (!archetype1.isComponentSet(0)) { return 1; }

    return 0;
}