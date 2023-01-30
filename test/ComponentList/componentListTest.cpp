#include <secs.hpp>

#define failIfNotThrew(b) if (b) { b = false; } else { return 1; }

struct Comp
{
    int a = 0;
};

int main() {
    secs::ComponentList<Comp> componentList {};
    secs::Entity entity = 0;

    bool threw = false;

    componentList.addComponent(entity);

    Comp *comp = componentList.getComponent(entity);
    comp->a = 10;

    if (componentList.getComponent(entity)->a != 10) { return 1; }

    try { componentList.getComponent(1); }
    catch (secs::ComponentDoesntExist) { threw = true; }
    catch (...) { return 1; }

    failIfNotThrew(threw)

    componentList.deleteComponent(entity);

    try { componentList.getComponent(entity); }
    catch (secs::ComponentDoesntExist) { threw = true; }
    catch (...) { return 1; }

    failIfNotThrew(threw)

    Comp c {};
    c.a = 5;

    componentList.addComponent(entity, c);

    if (componentList.getComponent(entity)->a != 5) { return 1; }

    componentList.entityDeleted(entity);

    try { componentList.getComponent(entity); }
    catch (secs::ComponentDoesntExist) { threw = true; }
    catch (...) { return 1; }

    failIfNotThrew(threw)

    return 0;
}