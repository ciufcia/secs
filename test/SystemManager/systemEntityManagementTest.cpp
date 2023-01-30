#include <secs.hpp>
#include <iostream>

#define failIfNotThrew(b) if (b) { b = false; } else { return 1; }

class System : public secs::System
{
public:

    std::size_t getEntityAmount() {
        return getEntities().size();
    }

    secs::SECS * getECSPointer() {
        return getECS();
    }
};

int main() {
    secs::SystemManager systemManager {};

    bool threw = false;

    secs::Entity entity {};

    std::cout << "a";

    try { systemManager.addEntityToSystem<System>(entity); }
    catch(secs::SystemNotActivated) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    System sys {};

    std::cout << "a";

    systemManager.activateSystem<System>(&sys);

    systemManager.addEntityToSystem<System>(entity);

    std::cout << "a";

    if (sys.getEntityAmount() != 1) { return 1; }

    systemManager.removeEntityFromSystem<System>(entity);

    if (sys.getEntityAmount() != 0) { return 1; }

    systemManager.addEntityToSystem<System>(entity);

    std::cout << "a";

    if (sys.getEntityAmount() != 1) { return 1; }

    systemManager.entityDeleted(entity);

    if (sys.getEntityAmount() != 0) { return 1; }

    std::cout << "a";

    try { sys.getECSPointer(); }
    catch(secs::ECSPointerIsNULL) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw);

    return 0;
}