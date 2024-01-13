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

void create_player_ship(Resource<Asset<cevy::engine::Mesh>> meshs, Resource<Asset<Diffuse>> difs,
                        Commands cmd) {
  auto handle_difs = difs.get().load("assets/space-ship1.png");
  auto handle_mesh = meshs.get().load("assets/space-ship1.obj");
  // handle_mesh.get().mesh.transform = MatrixRotateXYZ({0, M_PI, 0});

  cmd.spawn(cevy::engine::Transform(), TransformVelocity(cevy::engine::Transform(0, 0, 1)), handle_mesh, handle_difs, PlayerMarker());
}

void control_spaceship(Resource<Time> time, Query<Line> lines,
                       Query<cevy::engine::Camera, cevy::engine::Transform> cams,
                       Query<PlayerMarker, cevy::engine::Transform, cevy::engine::TransformVelocity> spaceship) {
  for (auto [space, tm, vel] : spaceship) {
    float delta = 5 * time.get().delta_seconds() * DEG2RAD;

    if (cevy::Keyboard::keyDown(KEY_W)) {
      vel.rotateX(delta);
    }
    if (cevy::Keyboard::keyDown(KEY_S)) {
      vel.rotateX(-delta);
    }
    if (cevy::Keyboard::keyDown(KEY_A)) {
      vel.rotateY(delta);
    }
    if (cevy::Keyboard::keyDown(KEY_D)) {
      vel.rotateY(-delta);
    }
    if (cevy::Keyboard::keyDown(KEY_E)) {
      vel.rotateZ(delta);
    }
    if (cevy::Keyboard::keyDown(KEY_Q)) {
      vel.rotateZ(-delta);
    }
    auto [cam, cam_tranform] = cams.single();


    vel.setPositionXYZ(tm.fwd() * 0.5);

    // Vector fwd = cevy::engine::Vector(0, 0, 1);
    // fwd.rotate(tm.rotation);
    // static float speed = 0;
    // speed += 0.0005;
    // tm.position += fwd * (delta + speed);

    cam_tranform.rotation = tm.rotation;
    cam_tranform.position = cevy::engine::Vector(0, 3, -10);
    cam_tranform.position.rotate(tm.rotation);
    cam_tranform.position += tm.position;
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
  app.add_systems<core_stage::Startup>(create_player_ship);
  app.add_systems<core_stage::Startup>(set_background);
  app.add_systems<core_stage::Update>(control_spaceship);
  app.spawn(cevy::engine::Camera(), cevy::engine::Transform(Vector(0, 5, -10)));
  app.run();
  return 0;
}
