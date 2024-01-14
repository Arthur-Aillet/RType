#include "App.hpp"
#include "Asset.hpp"
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "ClearColor.hpp"
#include "Color.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"
#include "PhysicsProps.hpp"
#include "Query.hpp"
#include "Resource.hpp"
#include "Time.hpp"
#include "Timer.hpp"
#include "Transform.hpp"
#include "Vector.hpp"
#include "Velocity.hpp"
#include "ecs.hpp"
#include "Engine.hpp"
#include "Input.hpp"
#include "Keyboard.hpp"
#include "raylib.h"
#include "raylib.hpp"
#include <iostream>

using namespace cevy;
using namespace ecs;
using namespace engine;

struct PlayerStats {
  size_t i;
  Timer time_before_shoot;
  float move_speed;
};

struct BulletHandle {
  Handle<cevy::engine::Mesh> handle;
};

void spawn_entities(Resource<Asset<cevy::engine::Mesh>> mash_manager, Resource<Asset<Diffuse>> difs,
                    Commands cmd, World &w) {
  auto &meshs = mash_manager.get();
  auto handle_mesh = meshs.load("assets/player.gltf");
  auto bullet = meshs.load("assets/grenade.gltf");

  w.insert_resource(BulletHandle{bullet});
  // Spawn Player 0
  cmd.spawn(engine::Transform().rotateX(-90 * DEG2RAD), TransformVelocity(), handle_mesh,
            PlayerStats{0, Timer(1, cevy::engine::Timer::Once).set_elapsed(1), 7});
  // Spawn Camera Planet
  cmd.spawn(cevy::engine::Camera(),
            cevy::engine::Transform(Vector(-40, 0, 0)).setRotationY(90 * DEG2RAD));
  // Spawn Gas Planet
  cmd.spawn(meshs.load("assets/gas.gltf"),
            TransformVelocity(cevy::engine::Transform().setRotationY(0.05 * DEG2RAD)),
            PhysicsProps().setDecay(0), engine::Transform(300, -30, 150).scaleXYZ(80));
  // Spawn Star
  cmd.spawn(meshs.load("assets/star.gltf"), engine::Transform(400, 40, -100).scaleXYZ(20),
            engine::Color(255, 250, 215));
  // Spawn Frozen Planet
  cmd.spawn(meshs.load("assets/frozen.gltf"), engine::Transform(300, -12, -50).scaleXYZ(30),
            TransformVelocity(cevy::engine::Transform().setRotationY(0.025 * DEG2RAD)),
            PhysicsProps().setDecay(0));
  // Spawn Continental Planet
  cmd.spawn(meshs.load("assets/continental.gltf"),
            TransformVelocity(cevy::engine::Transform().setRotationY(-0.05 * DEG2RAD)),
            PhysicsProps().setDecay(0), engine::Transform(700, 100, -220).scaleXYZ(30));
  // Spawn Smac Planet
  cmd.spawn(meshs.load("assets/smac.gltf"),
            TransformVelocity(cevy::engine::Transform().setRotationY(0.035 * DEG2RAD)),
            PhysicsProps().setDecay(0), engine::Transform(50, -30, -30).scaleXYZ(12));
  // Spawn Enemy Planet
  cmd.spawn(meshs.load("assets/enemy.gltf"),
            engine::Transform(0, 10, 0).scaleXYZ(0.004).rotateY(180 * DEG2RAD));
}

void spawn_bullet(Resource<Asset<cevy::engine::Mesh>> meshs, Resource<BulletHandle> bullet_handle,
                  Resource<Time> time, Resource<Asset<Diffuse>> difs, Resource<cevy::engine::Input<Keyboard>> keys,Commands cmd,
                  Query<PlayerStats, cevy::engine::Transform> players) {
  for (auto [player_stats, tm] : players) {
    player_stats.time_before_shoot.tick(time.get().delta());
    if (player_stats.time_before_shoot.finished()) {
      if (keys.get().pressed(KEY_SPACE)) {
       cmd.spawn(
        bullet_handle.get().handle, TransformVelocity(cevy::engine::Transform().setPositionZ(1)),
        engine::Transform(tm.position).translateZ(1).rotateX(90 * DEG2RAD).scaleXYZ(0.002));
        player_stats.time_before_shoot.reset();
      }
    }
  }
}

void control_spaceship(
    Resource<Time> time,
    Query<PlayerStats, cevy::engine::Transform, cevy::engine::TransformVelocity> spaceship) {
  for (auto [space, tm, vel] : spaceship) {
    float delta = space.move_speed * time.get().delta_seconds();

    vel.position.y = 0;
    vel.position.z = 0;
    //if (cevy::Keyboard::keyDown(KEY_W) && tm.position.y < 15.5)
    //  vel.translateY(delta);
    //if (cevy::Keyboard::keyDown(KEY_S) && tm.position.y > -15.5)
    //  vel.translateY(-delta);
    //if (cevy::Keyboard::keyDown(KEY_D) && tm.position.z < 28.5)
    //  vel.translateZ(delta);
    //if (cevy::Keyboard::keyDown(KEY_A) && tm.position.z > -28.5)
    //  vel.translateZ(-delta);
  }
}

void set_background(Resource<ClearColor> col) {
  col.get() = ClearColor(cevy::engine::Color(0, 0, 0));
}

int main() {
  struct SpaceShip {};
  App app;
  app.init_component<PlayerStats>();
  app.insert_resource(AssetManager());
  app.add_plugins(Engine());
  app.add_systems<core_stage::Startup>(spawn_entities);
  app.add_systems<core_stage::Startup>(set_background);
  app.add_systems<core_stage::Update>(control_spaceship);
  app.add_systems(spawn_bullet);
  app.run();
  return 0;
}
