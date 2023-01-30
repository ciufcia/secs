#include <secs.hpp>

#include <cassert>

#include <unordered_map>

int main() {
    secs::EntityManager em {};

    auto e1 = em.createEntity();
    auto e2 = em.createEntity();

    std::unordered_map<secs::Entity, secs::Archetype> temp {{0, secs::Archetype()}, {1, secs::Archetype()}};

    assert(em.getEntityArchetypes() == temp);

    assert(em.getEntityArchetype(e1) == secs::Archetype() && em.getEntityArchetype(e2) == secs::Archetype());

    secs::Archetype a1 {};
    a1.setComponent(0, true);

    secs::Archetype a2 {};
    a2.setComponent(1, true);

    em.setEntityArchetype(e1, a1);
    em.setEntityArchetype(e2, a2);

    assert(em.getEntityArchetype(e1) == a1);
    assert(em.getEntityArchetype(e2) == a2);

    return 0;
}
