//
// Created by jacob on 1/27/2023.
//

#include <secs.hpp>

#include <vector>
#include <random>
#include <iostream>
#include <cmath>

// a simple component, that holds data about two-dimensional position and direction
struct Transform
{
    float posX = 0.f;
    float posY = 0.f;

    float dirX = 1.f;
    float dirY = 1.f;

    void randomizeDirection() {
        std::default_random_engine gen;
        std::uniform_real_distribution<float> distribution {-1.f, 1.f};

        float x = ((float(rand()) / float(RAND_MAX)) * (1.f - (-1.f))) + (-1.f);
        float y = ((float(rand()) / float(RAND_MAX)) * (1.f - (-1.f))) + (-1.f);

        float l = std::sqrtf(x * x + y * y);

        dirX = x / l;
        dirY = y / l;
    }
};

// a system, that moves all of it's entities 
class Mover : public secs::System
{
    public:

        // returns false, if one of the entities got 100 units away from the center, otherwise returns true
        bool update() {
            // iterate through all of the entities, that this system has access to
            for (auto entity : getEntities()) {
                // get entity's Transform component through the ECS
                auto *transform = getECS()->getComponent<Transform>(entity);

                // update the position
                transform->posX += transform->dirX * speed;
                transform->posY += transform->dirY * speed;

                // log entity's position
                std::cout << "Position of entity `" << entity << "`: (" << transform->posX << ", " << transform->posY << ")\n";

                float distance = std::sqrtf(transform->posX * transform->posX + transform->posY * transform->posY);

                if (distance >= 100.f)
                    return false;
            }

            return true;
        }

    private:

        float speed = 1.f;
};

int main() {
    // create the ecs
    secs::SECS ecs {};

    // create 100 entities
    for (int i = 0; i < 100; i++)
        ecs.createEntity();

    // get all the entities
    const std::vector<secs::Entity> entities = ecs.getEntities();

    ecs.registerComponent<Transform>();

    // iterate through the entities and give them the Transform component
    for (auto entity : entities) {
        Transform transform {};
        transform.randomizeDirection();
        ecs.addComponent<Transform>(entity, transform);
    }

    Mover mover {};

    // activate the Mover system
    ecs.activateSystem<Mover>(&mover);

    // make Mover require Transform to work
    ecs.setSystemDependency<Mover, Transform>(true);

    // move all the entities, until one of them gets 100 units away from the center
    while (mover.update()) {}

    return 0;
}