#include "App.hpp"
#include "Asset.hpp"
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "ClearColor.hpp"
#include "Color.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"
#include "Line.hpp"
#include "Query.hpp"
#include "Target.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "Vector.hpp"
#include "Velocity.hpp"
#include "ecs.hpp"
#include "engine/Engine.hpp"
#include "input.hpp"
#include "raylib.h"
#include "raylib.hpp"
#include <raymath.h>

using namespace cevy;
using namespace ecs;
using namespace engine;

struct PlayerMarker {
  size_t i;
};

void spawn_entities(Resource<Asset<cevy::engine::Mesh>> meshs, Resource<Asset<Diffuse>> difs,
                    Commands cmd) {
  auto handle_mesh = meshs.get().load("assets/player.gltf");
  // handle_mesh.get().mesh.transform = MatrixRotateXYZ({0, M_PI, 0});

  cmd.spawn(engine::Transform().rotateX(-90 * DEG2RAD),
            TransformVelocity(cevy::engine::Transform(0, 0, 0)), handle_mesh, PlayerMarker());
  cmd.spawn(meshs.get().load("assets/gas.gltf"), engine::Transform(0, 2, 0)); // Gas
  cmd.spawn(meshs.get().load("assets/star.gltf"), engine::Transform(0, 4, 0),
            engine::Color(255, 250, 215)); // Broken - Used as Star
  cmd.spawn(meshs.get().load("assets/frozen.gltf"), engine::Transform(0, 12, 0));
  cmd.spawn(meshs.get().load("assets/continental.gltf"),
            engine::Transform(0, 6, 0));
  cmd.spawn(meshs.get().load("assets/smac.gltf"), engine::Transform(0, 8, 0)); // Smac
  cmd.spawn(meshs.get().load("assets/grenade.gltf"), engine::Transform(0, 16, 0));
  cmd.spawn(meshs.get().load("assets/enemy.gltf"), engine::Transform(0, 10, 0));
}

void control_spaceship(
    Resource<Time> time, Query<Line> lines,
    Query<cevy::engine::Camera, cevy::engine::Transform> cams,
    Query<PlayerMarker, cevy::engine::Transform, cevy::engine::TransformVelocity> spaceship) {
  for (auto [space, tm, vel] : spaceship) {
    float delta = 5 * time.get().delta_seconds();

    vel.position.y = 0;
    vel.position.z = 0;
    if (cevy::Keyboard::keyDown(KEY_W) && tm.position.y < 15.5)
      vel.translateY(delta);
    if (cevy::Keyboard::keyDown(KEY_S) && tm.position.y > -15.5)
      vel.translateY(-delta);
    if (cevy::Keyboard::keyDown(KEY_D) && tm.position.z < 28.5)
      vel.translateZ(delta);
    if (cevy::Keyboard::keyDown(KEY_A) && tm.position.z > -28.5)
      vel.translateZ(-delta);
  }
}

void set_background(Resource<ClearColor> col) {
  col.get() = ClearColor(cevy::engine::Color(0, 0, 0));
}

int main() {
  struct SpaceShip {};
  App app;
  app.init_component<PlayerMarker>();
  app.insert_resource(AssetManager());
  app.add_plugins(Engine());
  app.add_systems<core_stage::Startup>(spawn_entities);
  app.add_systems<core_stage::Startup>(set_background);
  app.add_systems<core_stage::Update>(control_spaceship);
  app.spawn(cevy::engine::Camera(),
            cevy::engine::Transform(Vector(-40, 0, 0)).setRotationY(90 * DEG2RAD));
  app.run();
  return 0;
}
