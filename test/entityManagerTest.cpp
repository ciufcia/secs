#include <secs.hpp>

int main() {
/*
    secs::EntityManager em {};

    secs::Entity entity1 = em.createEntity();
    secs::Entity entity2 = em.createEntity();
    secs::Entity entity3 = em.createEntity();

    bool wasExistant = em.deleteEntity(entity3);
    if (!wasExistant) {
        return 1;
    }

    entity3 = em.createEntity();
    if (entity3 != 2) {
        return 1;
    }

    auto archetypes = em.getEntityArchetypes();
    if (archetypes.size() != 3) {
        return 1;
    }

    secs::Archetype archetype1 = em.getEntityArchetype(entity1);
    secs::Archetype archetype2 = em.getEntityArchetype(entity2);

    archetype1.setComponent(0, true);

    em.setEntityArchetype(entity1, archetype1);

    archetype1 = em.getEntityArchetype(entity1);

    if (!archetype1.isComponentSet(0)) {
        return 1;
    }

    em.deleteEntity(entity1);
    em.deleteEntity(entity2);
    em.deleteEntity(entity3);

    if (em.getEntityArchetypes().size() != 0) {
        return 1;
    }
*/
    return 0;
}