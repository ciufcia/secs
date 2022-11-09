#include <secs.hpp>

#define failIfNotThrew(b) if (b) { b = false; } else { return 1; }

class System : public secs::System
{

};

int main() {
    secs::SystemManager systemManager {};

    bool threw = false;

    System sys {};

    try { systemManager.setSystemDependencies<System>(secs::Archetype()); }
    catch(secs::SystemNotActivated) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    systemManager.activateSystem<System>(&sys);

    secs::Archetype archetype1 {};

    archetype1.setComponent(0, true);
    archetype1.setComponent(4, true);
    archetype1.setComponent(6, true);

    systemManager.setSystemDependencies<System>(archetype1);

    if (systemManager.getSystemDependencies<System>() != archetype1) { return 1; }
    if (systemManager.getSystemDependencies(typeid(System).name()) != archetype1) { return 1; }

    return 0;
}