#include <secs.hpp>

struct Comp
{
    int a = 0;
};

struct Comp2
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

    int getComp2Value() {
        if (getEntities().empty()) {
            return 1;
        }
        return getECS()->getComponent<Comp2>(getEntities()[0])->a;
    }
};

int main() {
    secs::SECS ecs {};

    Sys sys {};

    ecs.registerComponent<Comp>();
    ecs.registerComponent<Comp2>();

    ecs.activateSystem<Sys>(&sys);

    secs::Entity entity = ecs.createEntity();

    ecs.addComponent<Comp2>(entity);

    secs::Archetype archetype {};
    archetype.setComponent(ecs.getComponentId<Comp>(), true);

    ecs.setSystemDependencies<Sys>(archetype);

    if (sys.getCompValue() != 1) { return 1; }

    ecs.addComponent<Comp>(entity);

    if (sys.getCompValue() != 0) { return 1; }

    ecs.removeComponent<Comp>(entity);

    if (sys.getCompValue() != 1) { return 1; }

    ecs.setSystemDependency<Sys, Comp>(false);

    if (sys.getComp2Value() != 0) { return 1; }

    ecs.setSystemDependency<Sys, Comp>(true);

    if (sys.getCompValue() != 1) { return 1; }

    ecs.addComponent<Comp>(entity);

    if (sys.getCompValue() != 0) { return 1; }

    return 0;
}