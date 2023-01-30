#include <secs.hpp>

#include <cassert>

int main() {
    secs::EntityManager em {};

    auto e1 = em.createEntity();
    auto e2 = em.createEntity();
    auto e3 = em.createEntity();

    assert(e1 == 0 && e2 == 1 && e3 == 2);

    em.deleteEntity(e1);

    e1 = em.createEntity();

    assert(e1 == 0);

    try {
        em.getEntityArchetype(10);
        return 1;
    } catch (const secs::EntityDoesntExist &e) {}

    return 0;
}
