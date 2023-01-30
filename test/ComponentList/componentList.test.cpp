#include <secs.hpp>

#include <cassert>

struct C
{
    bool val = false;
};

int main() {
    secs::ComponentList<C> cl {};

    cl.addComponent(0);
    cl.addComponent(1);

    cl.getComponent(0)->val = true;
    assert(cl.getComponent(0)->val);

    assert(!(cl.getComponent(1)->val));

    C c {};
    c.val = true;

    cl.addComponent(2, c);

    assert(cl.getComponent(2)->val);

    cl.deleteComponent(0);
    cl.deleteComponent(1);
    cl.deleteComponent(2);

    try {
        cl.getComponent(0);
        return 1;
    } catch (const secs::ComponentDoesntExist &e) {}

    cl.addComponent(0);

    try {
        cl.addComponent(0);
        return 1;
    } catch (const secs::DuplicateComponent &e) {}

    cl.entityDeleted(0);

    try {
        cl.getComponent(0);
        return 1;
    } catch (const secs::ComponentDoesntExist &e) {}

    return 0;
}
