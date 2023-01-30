#include <secs.hpp>

#include <cassert>

struct C
{
    bool val = false;
};

struct D
{
    bool val = false;
};

int main() {

    int i = 0;

    secs::ComponentManager cm {};

    try {
        cm.getComponent<C>(0);
        return 1;
    } catch (const secs::ComponentNotRecognised &e) {}

    cm.registerComponent<C>();
    cm.registerComponent<D>();

    assert(cm.getComponentId<C>() == 0);
    assert(cm.getComponentId<D>() == 1);

    cm.addComponent<C>(0);
    cm.addComponent<D>(0);

    auto *c = cm.getComponent<C>(0);
    c->val = true;

    assert(cm.getComponent<C>(0)->val);
    assert(!(cm.getComponent<D>(0)->val));

    cm.removeComponent<C>(0);
    cm.removeComponent<D>(0);

    try {
        cm.getComponent<C>(0);
        return 1;
    } catch (const secs::ComponentDoesntExist &e) {}

    cm.addComponent<C>(0);
    cm.entityDeleted(0);

    try {
        cm.getComponent<C>(0);
        return 1;
    } catch (const secs::ComponentDoesntExist &e) {}

    return 0;
}
