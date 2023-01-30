#include <secs.hpp>

#define failIfNotThrew(b) if (b) { b = false; } else { return 1; }

struct Comp
{
    int a = 0;
};

class Sys : public secs::System
{
public:

    int getCompValue() {
        if (getEntities().empty()) {
            return 1;
        }
        return getECS()->getComponent<Comp>(getEntities()[0])->a;
    }

    secs::SECS * getECSPointer() {
        return getECS();
    }
};

int main() {
    secs::SECS ecs {};

    bool threw = false;

    ecs.registerComponent<Comp>();

    Sys sys {};

    try { sys.getECSPointer(); }
    catch(secs::ECSPointerIsNULL) { threw = true; }
    catch (...) { return 1; }

    failIfNotThrew(threw)

    ecs.activateSystem<Sys>(&sys);
    ecs.setSystemDependency<Sys, Comp>(true);

    try { sys.getECSPointer(); }
    catch(...) { return 1; }

    secs::Entity entity = ecs.createEntity();

    Comp comp {};
    comp.a = 10;

    ecs.addComponent<Comp>(entity, comp);

    if (sys.getCompValue() != 10) { return 1; }

    ecs.removeComponent<Comp>(entity);

    if (sys.getCompValue() != 1) { return 1; }

    ecs.addComponent<Comp>(entity);

    if (sys.getCompValue() != 0) { return 1; }

    ecs.deleteEntity(entity);

    if (sys.getCompValue() != 1) { return 1; }

    entity = ecs.createEntity();

    ecs.deactivateSystem<Sys>();

    if (sys.getCompValue() != 1) { return 1; }

    try { sys.getECSPointer(); }
    catch(secs::ECSPointerIsNULL) { threw = true; }
    catch (...) { return 1; }

    failIfNotThrew(threw)

    return 0;
}