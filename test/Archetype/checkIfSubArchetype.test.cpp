#include <secs.hpp>

#include <cassert>

int main() {
    secs::Archetype a1 {};
    secs::Archetype a2 {};

    assert(secs::checkIfSubArchetype(a1, a2));
    assert(secs::checkIfSubArchetype(a2, a1));

    a1.setComponent(0, true);

    assert(!secs::checkIfSubArchetype(a1, a2));
    assert(secs::checkIfSubArchetype(a2, a1));

    a2.setComponent(0, true);

    assert(secs::checkIfSubArchetype(a1, a2));
    assert(secs::checkIfSubArchetype(a2, a1));

    a2.setComponent(1, true);

    assert(secs::checkIfSubArchetype(a1, a2));
    assert(!secs::checkIfSubArchetype(a2, a1));

    return 0;
}
