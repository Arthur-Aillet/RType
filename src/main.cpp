#include "App.hpp"
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"
#include "Position.hpp"
#include "Rotation.hpp"
#include "ecs.hpp"
#include "engine/Engine.hpp"
#include "input.hpp"

using namespace cevy;
using namespace ecs;
using namespace engine;

struct PlayerMarker {
  size_t i;
};

void create_player_ship(Resource<Asset<Model3D>> mod, Commands cmd) {
  auto handle = mod.get().load(
      Settings3D{.model = "assets/space-ship1.obj", .diffuse = "assets/space-ship1.png"});

  cmd.spawn(Position(), Rotation(0.0, 1.0, 0.0), handle, PlayerMarker());
}

std::array<float, 3> cross(std::array<float, 3> first, std::array<float, 3> second) {
  std::array<float, 3> result = {second[1] * first[2] - second[2] * first[1],
                                 second[2] * first[0] - second[0] * first[2],
                                 second[0] * first[1] - second[1] * first[0]};
  return result;
}

void control_object(Query<Position, Rotation, Handle<Model3D>, PlayerMarker> objs) {
  std::array<float, 3> fowards = {0.0, 1.0, 0.0};
  std::array<float, 3> right = {1.0, 0.0, 0.0};
  std::array<float, 3> up = {0.0, 0.0, 1.0};
  float speed = 0.5;
  for (auto obj : objs) {
    fowards = {std::get<1>(obj).fowards().x, std::get<1>(obj).fowards().y,
               std::get<1>(obj).fowards().z};
    up = {std::get<1>(obj).up().x, std::get<1>(obj).up().y, std::get<1>(obj).up().z};
    right = cross(fowards, up);
    if (Keyboard::keyDown(KEY_SPACE)) {
      std::get<0>(obj) = {std::get<0>(obj).x + up[0] * speed, std::get<0>(obj).y + up[1] * speed,
                          std::get<0>(obj).z + up[2] * speed};
    }
    if (Keyboard::keyDown(KEY_W)) {
      std::get<0>(obj) = {std::get<0>(obj).x + fowards[0] * speed,
                          std::get<0>(obj).y + fowards[1] * speed,
                          std::get<0>(obj).z + fowards[2] * speed};
    }
    if (Keyboard::keyDown(KEY_A)) {
      std::get<0>(obj) = {std::get<0>(obj).x + right[0] * speed,
                          std::get<0>(obj).y + right[1] * speed,
                          std::get<0>(obj).z + right[2] * speed};
    }
    if (Keyboard::keyDown(KEY_LEFT_SHIFT)) {
      std::get<0>(obj) = {std::get<0>(obj).x - up[0] * speed, std::get<0>(obj).y - up[1] * speed,
                          std::get<0>(obj).z - up[2] * speed};
    }
    if (Keyboard::keyDown(KEY_S)) {
      std::get<0>(obj) = {std::get<0>(obj).x - fowards[0] * speed,
                          std::get<0>(obj).y - fowards[1] * speed,
                          std::get<0>(obj).z - fowards[2] * speed};
    }
    if (Keyboard::keyDown(KEY_D)) {
      std::get<0>(obj) = {std::get<0>(obj).x - right[0] * speed,
                          std::get<0>(obj).y - right[1] * speed,
                          std::get<0>(obj).z - right[2] * speed};
    }
    if (Keyboard::keyPressed(KEY_RIGHT)) {
    }

    if (Keyboard::keyPressed(KEY_LEFT)) {
    }
  }
}

void follow_object(Query<cevy::engine::Camera, Position, Rotation> cams,
                   Query<Position, Rotation, Handle<Model3D>, PlayerMarker> objs) {
  Vector fowards = {0.0, 1.0, 0.0};
  float distance = 30;
  for (auto obj : objs) {
    // fowards = std::get<1>(obj).fowards();
    for (auto cam : cams) {
      fowards = std::get<2>(cam);
      // std::get<2>(cam) = std::get<1>(obj);
      std::get<0>(cam).camera.target = {std::get<0>(obj).x, std::get<0>(obj).y, std::get<0>(obj).z};
      std::get<1>(cam) = Position(std::get<0>(obj).x - fowards.x * distance,
                                  std::get<0>(obj).y - fowards.y * distance,
                                  std::get<0>(obj).z - fowards.z * distance);
    }
  }
}

int main() {
  struct SpaceShip {};
  App app;
  app.init_component<PlayerMarker>();
  app.insert_resource(AssetManager());
  app.add_plugins(Engine());
  app.add_system<Schedule::Startup>(create_player_ship);
  app.add_system<Schedule::Update>(control_object);
  app.add_system<Schedule::Update>(follow_object);
  app.run();
  return 0;
}
