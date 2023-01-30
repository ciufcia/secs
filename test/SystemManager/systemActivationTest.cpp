#include <secs.hpp>

#define failIfNotThrew(b) if (b) { b = false; } else { return 1; }

class System : public secs::System
{

};

int main() {
    secs::SystemManager systemManager {};

    bool threw = false;

    try { systemManager.getSystemDependencies<System>(); }
    catch(secs::SystemNotActivated) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    System sys {};

    systemManager.activateSystem<System>(&sys);

    try { systemManager.activateSystem<System>(&sys); }
    catch(secs::SystemAlreadyActivated) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    systemManager.deactivateSystem<System>();

    try { systemManager.getSystemDependencies<System>(); }
    catch(secs::SystemNotActivated) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    return 0;
}