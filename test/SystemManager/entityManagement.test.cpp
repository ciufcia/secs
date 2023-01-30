#include <secs.hpp>

#include <cassert>

#include <vector>

class S : public secs::System
{
    public:

        const std::vector<secs::Entity> &getSystemEntities() {
            return getEntities();
        }
};

int main() {
    secs::SystemManager sm {};

    S s {};

    sm.activateSystem<S>(&s);

    sm.addEntityToSystem<S>(0);
    sm.addEntityToSystem<S>(1);

    const std::vector<secs::Entity> temp1 {0, 1};

    assert(s.getSystemEntities() == temp1);

    sm.removeEntityFromSystem<S>(0);

    const std::vector<secs::Entity> temp2 {1};

    assert(s.getSystemEntities() == temp2);

    sm.entityDeleted(1);

    const std::vector<secs::Entity> temp3 {};

    assert(s.getSystemEntities() == temp3);

    return 0;
}
