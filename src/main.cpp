#include <iostream>

#include "App.hpp"
#include "AssetManager.hpp"
#include "Commands.hpp"
#include "Camera.hpp"
#include "Position.hpp"
#include "Rotation.hpp"
#include "Commands.hpp"
#include "input.hpp"
#include "EntityCommands.hpp"
#include "ecs.hpp"
#include "engine/Engine.hpp"

using namespace cevy;
using namespace ecs;

void create_space_ship(Resource<Asset<Model3D>> mod, Commands cmd) {
    auto handle = mod.get().load(
        Settings3D {
            .model = "assets/space-ship1.obj",
            .diffuse = "assets/space-ship1.png"
        }
    );

    cmd.spawn(Position(), Rotation(0.0, 1.0, 0.0), handle);
}


std::array<double, 3> cross(std::array<double, 3> first, std::array<double, 3> second) {
    std::array<double, 3> result = {
        second[1] * first[2] - second[2] * first[1],
        second[2] * first[0] - second[0] * first[2],
        second[0] * first[1] - second[1] * first[0]
    };
    return result;
}

void control_object(cevy::ecs::Query<cevy::Position, cevy::Rotation, cevy::Handle<cevy::Model3D>> objs) {
    std::array<double, 3> fowards = {0.0, 1.0, 0.0};
    std::array<double, 3> right = {1.0, 0.0, 0.0};
    std::array<double, 3> up = {0.0, 0.0, 1.0};
    double speed = 0.5;
    for (auto obj : objs) {
        fowards = {std::get<1>(obj).fowards().x, std::get<1>(obj).fowards().y, std::get<1>(obj).fowards().z};
        up = {std::get<1>(obj).up().x, std::get<1>(obj).up().y, std::get<1>(obj).up().z};
        right = cross(fowards, up);
        if (cevy::Keyboard::keyDown(KEY_SPACE)) {
            std::get<0>(obj) = {std::get<0>(obj).x + up[0] * speed, std::get<0>(obj).y + up[1] * speed, std::get<0>(obj).z + up[2] * speed};
        }
        if (cevy::Keyboard::keyDown(KEY_W)) {
            std::get<0>(obj) = {std::get<0>(obj).x + fowards[0] * speed, std::get<0>(obj).y + fowards[1] * speed, std::get<0>(obj).z + fowards[2] * speed};
        }
        if (cevy::Keyboard::keyDown(KEY_A)) {
            std::get<0>(obj) = {std::get<0>(obj).x + right[0] * speed, std::get<0>(obj).y + right[1] * speed, std::get<0>(obj).z + right[2] * speed};
        }
        if (cevy::Keyboard::keyDown(KEY_LEFT_SHIFT)) {
            std::get<0>(obj) = {std::get<0>(obj).x - up[0] * speed, std::get<0>(obj).y - up[1] * speed, std::get<0>(obj).z - up[2] * speed};
        }
        if (cevy::Keyboard::keyDown(KEY_S)) {
            std::get<0>(obj) = {std::get<0>(obj).x - fowards[0] * speed, std::get<0>(obj).y - fowards[1] * speed, std::get<0>(obj).z - fowards[2] * speed};
        }
        if (cevy::Keyboard::keyDown(KEY_D)) {
            std::get<0>(obj) = {std::get<0>(obj).x - right[0] * speed, std::get<0>(obj).y - right[1] * speed, std::get<0>(obj).z - right[2] * speed};
        }
        if (cevy::Keyboard::keyPressed(KEY_RIGHT)) {
            std::get<1>(obj).rotate();
        }
        // if (cevy::Keyboard::keyDown(KEY_DOWN)) {
        //     std::get<1>(obj).x += 1.0;
        // }
        // if (cevy::Keyboard::keyDown(KEY_LEFT)) {
        //     std::get<1>(obj).z += -1.0;
        // }
        // if (cevy::Keyboard::keyDown(KEY_UP)) {
        //     std::get<1>(obj).x += -1.0;
        // }
    }
}

void follow_object(cevy::ecs::Query<cevy::Camera, cevy::Position, cevy::Rotation> cams, cevy::ecs::Query<cevy::Position, cevy::Rotation, cevy::Handle<cevy::Model3D>> objs) {
    Vector fowards = {0.0, 1.0, 0.0};
    double distance = 30;
    for (auto obj : objs) {
        // fowards = std::get<1>(obj).fowards();
        for (auto cam : cams) {
            fowards = std::get<2>(cam);
            // std::get<2>(cam) = std::get<1>(obj);
            std::get<0>(cam).camera.target = {std::get<0>(obj).x, std::get<0>(obj).y, std::get<0>(obj).z};
            std::get<1>(cam) = Position (std::get<0>(obj).x - fowards.x * distance, std::get<0>(obj).y - fowards.y * distance, std::get<0>(obj).z - fowards.z * distance);
        }
    }
}

int main() {
    App app;
    app.add_plugins(Engine());
    app.add_system<Schedule::Startup>(create_space_ship);
    app.add_system<Schedule::Update>(control_object);
    app.add_system<Schedule::Update>(follow_object);
    app.run();
    return 0;
}
