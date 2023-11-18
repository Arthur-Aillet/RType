#include <iostream>

#include "../Cevy/src/registry.hpp"
#include "../Cevy/src/game_engine/game_engine.hpp"
#include "../Cevy/src/game_engine/position.hpp"
#include "../Cevy/src/game_engine/velocity.hpp"

void logging_system(registry &r,
    sparse_array<position> const &positions,
    sparse_array<velocity> const &velocities) {
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
    app.add_component(movable, position {0, 0});
    app.add_component(movable, velocity {3, 1});
    app.add_system<position, velocity>(logging_system);
    std::cout << "Hello world!" << std::endl;
    app.run_systems();
    std::cout << "Hello world!" << std::endl;
    return 0;
}
