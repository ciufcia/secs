#include <secs.hpp>

int main() {
    secs::Archetype archetype1{};
    archetype1.setComponent(0, true);
    archetype1.setComponent(1, true);
    archetype1.setComponent(2, true);
    if (!archetype1.isComponentSet(0)) { return 1;}
    if (archetype1.isComponentSet(8)) { return 1; }
    archetype1.setComponent(1, false);
    if (archetype1.isComponentSet(1)) { return 1; }
    return 0;
}
