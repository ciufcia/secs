#include <secs.hpp>

#include <cassert>

int main() {
    secs::EntityManager em {};

    em.createEntity();
    em.createEntity();
    em.createEntity();

    std::vector<secs::Entity> temp {0, 1, 2};

    assert(em.getEntities() == temp);

    secs::Archetype a {};
    a.setComponent(0, true);

    em.setEntityArchetype(0, a);

    temp = {0};

    assert(em.getEntities(a) == temp);

    em.setEntityArchetype(2, a);

    temp = {0, 2};

    assert(em.getEntities(a) == temp);

    a.setComponent(1, true);

    em.setEntityArchetype(1, a);

    a.setComponent(1, false);

    temp = {0, 1, 2};

    assert(em.getEntities() == temp);

    return 0;
}
