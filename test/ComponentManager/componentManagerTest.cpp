#include <secs.hpp>

#define failIfNotThrew(b) if (b) { b = false; } else { return 1; }

struct Comp
{
    int a = 0;
};

int main() {
    secs::ComponentManager componentManager {};
    secs::Entity entity = 0;

    bool threw = false;

    try { componentManager.addComponent<Comp>(entity); }
    catch(secs::ComponentNotRecognised) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    try { componentManager.addComponent<Comp>(entity, Comp()); }
    catch(secs::ComponentNotRecognised) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    componentManager.registerComponent<Comp>();

    componentManager.addComponent<Comp>(entity);

    Comp *comp = componentManager.getComponent<Comp>(entity);

    comp->a = 10;

    if (componentManager.getComponent<Comp>(entity)->a != 10) { return 1; }

    componentManager.removeComponent<Comp>(entity);

    try { componentManager.getComponent<Comp>(entity); }
    catch(secs::ComponentDoesntExist) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    Comp c {};
    c.a = 5;

    componentManager.addComponent<Comp>(entity, c);

    if (componentManager.getComponent<Comp>(entity)->a != 5) { return 1; }

    componentManager.entityDeleted(entity);

    try { componentManager.getComponent<Comp>(entity); }
    catch(secs::ComponentDoesntExist) { threw = true; }
    catch(...) { return 1; }

    failIfNotThrew(threw)

    return 0;
}