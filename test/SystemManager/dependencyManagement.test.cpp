#include <secs.hpp>

#include <cassert>

class S1 : public secs::System
{

};

class S2 : public secs::System
{

};

int main() {
    secs::SystemManager sm {};

    S1 s1 {};
    S2 s2 {};

    sm.activateSystem<S1>(&s1);
    sm.activateSystem<S2>(&s2);

    assert(sm.getSystemDependencies<S1>() == secs::Archetype());
    assert(sm.getSystemDependencies(typeid(S2).name()) == secs::Archetype());

    secs::Archetype temp {};
    temp.setComponent(0, true);

    sm.setSystemDependencies<S1>(temp);

    assert(sm.getSystemDependencies<S1>() == temp);
    assert(!(sm.getSystemDependencies(typeid(S2).name()) == temp));

    try {
        sm.activateSystem<S1>(&s1);
        return 1;
    } catch (const secs::SystemAlreadyActivated &e) {}

    sm.deactivateSystem<S2>();

    try {
        sm.getSystemDependencies<S2>();
        return 1;
    } catch (const secs::SystemNotActivated &e) {};

    return 0;
}
