#include <iostream>

#include "../Cevy/src/registry.hpp"
#include "../Cevy/src/game_engine/game_engine.hpp"
#include "../Cevy/src/game_engine/position.hpp"
#include "../Cevy/src/game_engine/velocity.hpp"

void logging_system(registry &r) {
    auto const &positions = r.get_components<position>();
    auto const &velocities = r.get_components<velocity>();
    for (size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
        auto const &pos = positions[i];
        auto const &vel = velocities[i];
        if (pos && vel) {
            std::cerr << i << " : Position = {" << pos.value().x << ", " << pos.value().y <<
                "}, Velocity = {" << vel.value().x << ", " << vel.value().y << "}" << std::endl;
        }
    }
}

int main() {
    registry app = registry();
    app.register_component<position>();
    app.register_component<velocity>();
    app.register_component<drawable>();
    app.register_component<controllable>();
    entity movable = app.spawn_entity();
    app.add_component(movable, position { 0, 0});
    app.add_component(movable, velocity { 3, 1});
    std::cout << "Hello world!" << std::endl;
    logging_system(app);
    std::cout << "Hello world!" << std::endl;
    return 0;
}
