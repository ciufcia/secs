#include <secs.hpp>
#include <SFML/Graphics.hpp>
#include <random>

struct Renderable
{
    sf::Sprite sprite {};
};

struct Transform
{
    sf::Vector2f direction {};
};

class Renderer : public secs::System
{
public:

    Renderer(sf::RenderWindow *window) {
        mpWindow = window;
    }

    void update() {
        for (secs::Entity entity : mEntities) {
            Renderable *renderable = getECS()->getComponent<Renderable>(entity);
            mpWindow->draw(renderable->sprite);
        }
    }

private:

    sf::RenderWindow *mpWindow;
};

class Controller : public secs::System
{
public:

    void update(float deltaTime) {
        for (secs::Entity entity : mEntities) {
            Renderable *renderable = getECS()->getComponent<Renderable>(entity);
            Transform *transform = getECS()->getComponent<Transform>(entity);
            renderable->sprite.move(sf::Vector2f(transform->direction.x * 0.05 * deltaTime, transform->direction.y * 0.05 * deltaTime));
        }
    }
};

int main() {

    sf::RenderWindow window {sf::VideoMode(sf::Vector2u(920u, 920u)), "example"};
    window.setFramerateLimit(60);

    sf::Clock clock;

    secs::SECS ecs {};

    ecs.registerComponent<Renderable>();
    ecs.registerComponent<Transform>();

    Renderer renderer {&window};
    ecs.activateSystem<Renderer>(&renderer);
    ecs.setSystemDependency<Renderer, Renderable>();

    Controller controller {};
    ecs.activateSystem<Controller>(&controller);
    ecs.setSystemDependency<Controller, Renderable>();
    ecs.setSystemDependency<Controller, Transform>();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> positionDistribution(0, 920);
    std::uniform_int_distribution<> colorDistribution(0, 255);
    std::uniform_int_distribution<> directionDistribution(-3, 3);

    for (unsigned int i = 0u; i < 500u; i++) {
        secs::Entity entity = ecs.createEntity();

        Renderable renderable {};
        renderable.sprite.setPosition(sf::Vector2f(positionDistribution(gen), positionDistribution(gen)));
        renderable.sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(100, 100)));
        renderable.sprite.setTexture(sf::Texture());
        renderable.sprite.setColor(sf::Color(colorDistribution(gen), colorDistribution(gen), colorDistribution(gen)));
        ecs.addComponent<Renderable>(entity, renderable);

        Transform transform {};
        transform.direction.x = cos(directionDistribution(gen));
        transform.direction.y = cos(directionDistribution(gen));
        ecs.addComponent<Transform>(entity, transform);
    }

    while (window.isOpen()) {
        sf::Event event {};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case event.Closed:
                    window.close();
                    return 0;
            }
        }

        window.clear();
        controller.update(1.f / clock.restart().asSeconds());
        renderer.update();
        window.display();
    }

    return 0;
}