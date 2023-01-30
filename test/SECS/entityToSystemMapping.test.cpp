#include <secs.hpp>

#include <cassert>

#include <vector>

struct C
{

};

class S : public secs::System
{
    public:

        const std::vector<secs::Entity> &getSystemEntities() {
            return getEntities();
        }
};

int main() {
    secs::SECS ecs {};

    ecs.createEntity();
    ecs.createEntity();
    ecs.createEntity();

    S s {};

    ecs.activateSystem<S>(&s);

    const std::vector<secs::Entity> temp1 = {0, 1, 2};

    assert(s.getSystemEntities() == temp1);

    ecs.registerComponent<C>();

    ecs.setSystemDependency<S, C>(true);

    const std::vector<secs::Entity> temp2 = {};

    assert(s.getSystemEntities() == temp2);

    ecs.addComponent<C>(1);

    const std::vector<secs::Entity> temp3 = {1};

    assert(s.getSystemEntities() == temp3);

    ecs.deleteEntity(1);

    const std::vector<secs::Entity> temp4 = {};

    assert(s.getSystemEntities() == temp4);

    return 0;
}
