#include "App.hpp"
#include "Asset.hpp"
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"
#include "Position.hpp"
#include "Query.hpp"
#include "Rotation.hpp"
#include "Schedule.hpp"
#include "Time.hpp"
#include "Vector.hpp"
#include "ecs.hpp"
#include "engine/Engine.hpp"
#include "input.hpp"
#include "raylib.h"

using namespace cevy;
using namespace ecs;
using namespace engine;

struct PlayerMarker {
  size_t i;
};

void create_player_ship(Resource<Asset<cevy::engine::Mesh>> meshs, Resource<Asset<Diffuse>> difs,
                        Commands cmd) {
  auto handle_difs = difs.get().load("assets/space-ship1.png");
  auto handle_mesh = meshs.get().load("assets/space-ship1.obj");

  cmd.spawn(Position(), Rotation(0.0, 1.0, 0.0), handle_mesh, handle_difs, PlayerMarker());
}

void rotate_camera(Resource<Time> time, Query<cevy::engine::Camera, cevy::engine::Rotation> cams) {
  for (auto [cam, rot] : cams) {
    if (cevy::Keyboard::keyDown(KEY_A)) {
      rot.x -= 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_D)) {
      rot.x += 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_S)) {
      rot.y -= 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_W)) {
      rot.y += 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_Q)) {
      rot.z -= 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_E)) {
      rot.z += 30 * time.get().delta_seconds();
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
  app.add_system<Schedule::Update>(rotate_camera);
  app.spawn(cevy::engine::Camera(), cevy::engine::Position(-10.0, 10.0, 10.0),
            cevy::engine::Rotation(0.0, 0.0, 0.0));
  app.run();
  return 0;
}
