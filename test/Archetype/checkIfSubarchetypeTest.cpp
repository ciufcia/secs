#include <secs.hpp>

int main() {
    secs::Archetype archetype1 {};
    archetype1.setComponent(0, true);
    archetype1.setComponent(1, true);
    archetype1.setComponent(2, true);

    secs::Archetype archetype2 {};
    archetype2.setComponent(1, true);
    archetype2.setComponent(2, true);

    if (secs::checkIfSubarchetype(archetype1, archetype2)) { return 1; }
    if (!secs::checkIfSubarchetype(archetype2, archetype1)) { return 1; }

    return 0;
}