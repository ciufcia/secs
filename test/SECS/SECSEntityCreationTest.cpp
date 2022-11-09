#include <secs.hpp>

int main() {
    secs::SECS ecs {};

    secs::Entity entity = ecs.createEntity();

    if (entity != 0) { return 1; }

    ecs.deleteEntity(entity);

    if (ecs.deleteEntity(entity)) { return 1; }

    entity = ecs.createEntity();

    if (entity != 0) { return 1; }

    return 0;
}