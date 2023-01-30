#include <secs.hpp>

#include <cassert>

int main() {
    secs::Archetype a1 {};
    secs::Archetype a2 {};

    assert(a1 == a1);
    assert(a2 == a2);

    assert(a1 == a2);
    assert(!(a1 != a2));
    assert(a2 == a1);
    assert(!(a2 != a1));

    a1.setComponent(0, true);

    assert(a1 != a2);
    assert(!(a1 == a2));
    assert(a2 != a1);
    assert(!(a2 == a1));

    a2.setComponent(0, true);

    assert(a1 == a2);
    assert(!(a1 != a2));
    assert(a2 == a1);
    assert(!(a2 != a1));

    a2.setComponent(0, false);

    assert(a1 != a2);
    assert(!(a1 == a2));
    assert(a2 != a1);
    assert(!(a2 == a1));

    a1.setComponent(1, true);
    a1.setComponent(2, true);
    a1.setComponent(3, true);

    assert(a1 != a2);
    assert(!(a1 == a2));
    assert(a2 != a1);
    assert(!(a2 == a1));

    a1.setComponent(2, false);

    assert(a2 != a1);
    assert(!(a2 == a1));
    assert(a2 != a1);
    assert(!(a2 == a1));

    a2.setComponent(0, true);
    a2.setComponent(1, true);
    a2.setComponent(3, true);

    assert(a1 == a2);
    assert(!(a1 != a2));
    assert(a2 == a1);
    assert(!(a2 != a1));

    return 0;
}
