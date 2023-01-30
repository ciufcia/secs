#include <secs.hpp>

#include <cassert>

int main() {
    secs::Archetype a1 {};

    assert(!a1.isComponentSet(0));

    a1.setComponent(0, true);

    assert(a1.isComponentSet(0));

    a1.setComponent(0, false);

    assert(!a1.isComponentSet(0));

    return 0;
}
