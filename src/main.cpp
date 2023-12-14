#include <iostream>

#include "../Cevy/src/cevy.hpp"
#include "../Cevy/src/ecs/ecs.hpp"
#include "../Cevy/src/ecs/App.hpp"
#include "../Cevy/src/ecs/Plugin.hpp"
#include "../Cevy/src/ecs/Schedule.hpp"
#include "../Cevy/src/game_engine/game_engine.hpp"
#include "../Cevy/src/game_engine/position.hpp"
#include "../Cevy/src/game_engine/velocity.hpp"

void logging_system(cevy::ecs::Query<position, velocity> query, Resource<int> num)
{
    std::cout << "Num: " << num << std::endl;
    auto i = 0;
    // for (const auto& e : query) {
    //     std::cerr << "element " << i++ << " in query" << std::endl;
    // }

    for (auto it = query.begin(); it != query.end(); ++it) {
        std::cerr << i++ << ": pos: (" << std::get<position&>(*it).x << "," << std::get<position&>(*it).y << ")" << std::endl;
    }
}

void incr(Resource<int> num) {
    num += 1;
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
    app.insert_resource(0);
    app.register_component<position>();
    app.register_component<velocity>();
    app.register_component<drawable>();
    app.register_component<controllable>();
    cevy::ecs::Entity movable = app.spawn_empty();
    app.add_component(movable, position {0, 0});
    app.add_component(movable, velocity {3, 1});
    app.add_super_system(apply_velocity);
    app.add_super_system(dampen);
    app.add_super_system(apply_gravity);
    app.add_super_system(jump);
    app.add_super_system(incr);
    app.add_super_system<cevy::ecs::Schedule::PostUpdate>(logging_system);
    app.run();
    return 0;
}
