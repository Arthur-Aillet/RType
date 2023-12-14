#include <iostream>

#include "../Cevy/src/cevy.hpp"
#include "../Cevy/src/ecs/ecs.hpp"
#include "../Cevy/src/ecs/App.hpp"
#include "../Cevy/src/ecs/Plugin.hpp"
#include "../Cevy/src/ecs/Schedule.hpp"
#include "../Cevy/src/game_engine/game_engine.hpp"
#include "../Cevy/src/game_engine/position.hpp"
#include "../Cevy/src/game_engine/velocity.hpp"

void logging_system(cevy::ecs::World &w,
    SparseVector<position> const &positions,
    SparseVector<velocity> const &velocities) {
    for (size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
        auto const &pos = positions[i];
        auto const &vel = velocities[i];
        if (pos && vel) {
            std::cerr << i << " : Position = {" << pos.value().x << ", " << pos.value().y <<
                "}, Velocity = {" << vel.value().x << ", " << vel.value().y << "}" << std::endl;
        }
    }
}

void system2(cevy::ecs::Query<position, velocity> query)
{
    auto i = 0;
    // for (const auto& e : query) {
    //     std::cerr << "element " << i++ << " in query" << std::endl;
    // }

    for (auto it = query.begin(); it != query.end(); ++it) {
        std::cerr << i++ << ": pos: (" << std::get<position&>(*it).x << "," << std::get<position&>(*it).y << ")" << std::endl;
    }
}

void dampen(cevy::ecs::Query<velocity> query)
{
    for (auto e : query) {
        auto& x = std::get<velocity&>(e).x;
        auto& y = std::get<velocity&>(e).y;
        x -= x * x * 0.1 / 60;
        y -= y * y * 0.1 / 60;
    }
}

void apply_velocity(cevy::ecs::Query<velocity, position> query)
{
    for (auto e : query) {
        std::get<position&>(e).x += std::get<velocity&>(e).x / 60.0;
        std::get<position&>(e).y += std::get<velocity&>(e).y / 60.0;
    }
}

void apply_gravity(cevy::ecs::Query<velocity> query)
{
    for (auto e : query) {
        std::get<velocity&>(e).y -= 9.81 / 60.0;
    }
}

void jump(cevy::ecs::Query<velocity, position> query)
{
    for (auto e : query) {
        if (std::get<position&>(e).y <= 0)
            std::get<velocity&>(e).y = 18.0;
    }
}

void update(cevy::ecs::World &w) {
    std::cerr << "Update" << std::endl;
}

void post_update(cevy::ecs::World &w) {
    std::cerr << "Post Update" << std::endl;
}

void pre_update(cevy::ecs::World &w) {
    std::cerr << "Pre Update" << std::endl;
}

void startup(cevy::ecs::World &w) {
    std::cerr << "Startup" << std::endl;
}

void pre_startup(cevy::ecs::World &w) {
    std::cerr << "pre Startup" << std::endl;
}

class GoodByePlugin : public cevy::ecs::Plugin {
    public: void build(cevy::ecs::App& app) override
    {
        app.register_component<float>();
    }
};


class HelloPlugin : public cevy::ecs::Plugin {
    public: void build(cevy::ecs::App& app) override
    {
        app.register_component<int>();
    }
};

int main() {
    cevy::ecs::App app;
    app.add_plugins<HelloPlugin>();
    app.register_component<position>();
    app.register_component<velocity>();
    app.register_component<drawable>();
    app.register_component<controllable>();
    cevy::ecs::Entity movable = app.spawn_empty();
    app.add_component(movable, position {0, 0});
    app.add_component(movable, velocity {3, 1});

    // app.add_system<cevy::ecs::Schedule::Update>(update);
    // app.add_system<cevy::ecs::Schedule::PreUpdate>(pre_update);
    // app.add_system<cevy::ecs::Schedule::PostUpdate>(post_update);
    // app.add_system<cevy::ecs::Schedule::Startup>(startup);
    // app.add_system<cevy::ecs::Schedule::PreStartup>(pre_startup);
    // app.add_system<position, velocity>(logging_system);
    app.add_super_system(system2);
    app.add_super_system(apply_velocity);
    app.add_super_system(dampen);
    app.add_super_system(apply_gravity);
    app.add_super_system(jump);
    std::cout << "Hello world!" << std::endl;
    app.run();
    std::cout << "Hello world!" << std::endl;
    return 0;
}
