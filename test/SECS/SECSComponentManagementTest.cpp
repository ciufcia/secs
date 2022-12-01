#include <secs.hpp>

#define failIfNotThrew(b) if (b) { b = false; } else { return 1; }

struct Comp
{
    int a = 0;
};

struct Comp2
{

};

int main() {
    secs::SECS ecs {};

    bool threw = false;

    secs::Entity entity = ecs.createEntity();

    try { ecs.addComponent<Comp>(entity); }
    catch(secs::ComponentNotRecognised) { threw = true; }
    catch(...) { return 1; }

    ecs.registerComponent<Comp>();

    if (ecs.checkIfHasComponent<Comp>(entity)) { return 1; }

    ecs.addComponent<Comp>(entity);

    if (!ecs.checkIfHasComponent<Comp>(entity)) { return 1; }

    Comp *comp = ecs.getComponent<Comp>(entity);

    comp->a = 10;

    if (ecs.getComponent<Comp>(entity)->a != 10) { return 1; }

    ecs.removeComponent<Comp>(entity);

    if (ecs.checkIfHasComponent<Comp>(entity)) { return 1; }

    Comp c {};
    c.a = 5;

    ecs.addComponent<Comp>(entity, c);

    if (ecs.getComponent<Comp>(entity)->a != 5) { return 1; }

    ecs.removeComponent<Comp>(entity);

    try { ecs.getComponent<Comp>(entity); }
    catch(secs::ComponentDoesntExist) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    if (ecs.getComponentId<Comp>() != ecs.getComponentId<Comp>()) { return 1; }
    if (ecs.getComponentId<Comp>() == ecs.getComponentId<Comp2>()) { return 1; }

    return 0;
}