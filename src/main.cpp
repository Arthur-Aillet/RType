#include "App.hpp"
#include "Camera.hpp"
#include "ClearColor.hpp"
#include "Color.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"
#include "Handle.hpp"
#include "Mesh.hpp"
#include "PhysicsProps.hpp"
#include "Query.hpp"
#include "Resource.hpp"
#include "Stage.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "Vector.hpp"
#include "Velocity.hpp"
#include "input.hpp"
#include "network/CevyNetwork.hpp"
#include "network/network.hpp"
#include "raylib.h"
#include "raylib.hpp"
#include <chrono>
#include <cstdlib>
#include <raymath.h>
#include <string.h>
#include "main.hpp"
#include "SpaceShipSync.hpp"
#include "ShipActions.hpp"
#include "network/NetworkPlugin.hpp"

using namespace cevy;
using namespace ecs;
using namespace engine;

void setup_logic(Resource<Asset<cevy::engine::Mesh>> mash_manager, Resource<Asset<Diffuse>> difs,
                    Commands cmd, World &w) {
  auto &meshs = mash_manager.get();
  auto player = meshs.load("assets/player.gltf");
  auto bullet = meshs.load("assets/grenade.gltf");
  auto enemy = meshs.load("assets/enemy.gltf");

  bullet->mesh.transform = MatrixScale(0.004, 0.004, 0.004);
  enemy->mesh.transform = MatrixScale(0.004, 0.004, 0.004);

  w.insert_resource(EnemySpawner{Timer(4, Timer::Once), 1, enemy});
  w.insert_resource(LifeResource{enemy});
  w.insert_resource(RtypeHandles{bullet, player, enemy, enemy});
  cmd.spawn(cevy::engine::Camera(),
          cevy::engine::Transform(Vector(-40, 0, 0)).setRotationY(90 * DEG2RAD));
  // Spawn Player 0
  // cmd.spawn(engine::Transform().rotateX(-90 * DEG2RAD), TransformVelocity(), handle_mesh,
  //           engine::Color(255, 0, 0),
  //           PlayerStats{0, Timer(1, Timer::Once).set_elapsed(2), 13});
  // // Spawn Camera Planet
}

void setup_world(Resource<Asset<cevy::engine::Mesh>> mash_manager, Resource<Asset<Diffuse>> difs,
                    Commands cmd, World &w) {
  auto &meshs = mash_manager.get();
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
}

void spawn_enemies(Resource<Time> time, Resource<EnemySpawner> spawner, Commands cmd, Resource<NetworkCommands> netcmd, Query<PlayerMarker> players)  {
  auto &clock = spawner.get().time_before_spawn;

  if (players.size() < 1)
    return;
  clock.tick(time.get().delta());

  if (clock.finished()) {
    // float y = rand() % 280 - 140;
    // cmd.spawn(spawner.get().handle,
    //   engine::Transform(0, y / 10, 34).scaleXYZ(0.004).rotateY(180 * DEG2RAD),
    //   TransformVelocity(cevy::engine::Transform().translateZ(-11 - y / 140)));
    netcmd.get().summon<Enemy>();
    // clock.setDuration(clock.duration().count() - clock.duration().count() * (spawner.get().spawn_increase_perc/100));
    clock.reset();
  }
}

void enemy_mvt(Resource<Time> time, Resource<NetworkCommands> netcmd, Query<EnemyMarker, cevy::engine::Transform, cevy::engine::TransformVelocity, SyncId> enemies) {
  for (auto [marker, tm, vel, id] : enemies) {
    if (tm.position.z < -29) {
      netcmd.get().dismiss(id);
    }
    if (tm.position.z > 28) {
      if (tm.position.y >= 0)
        vel.position.y = -5;
      if (tm.position.y < 0)
        vel.position.y = 5;
    }
    if (tm.position.y > 15)
      vel.position.y = -5;
    if (tm.position.y < -15)
      vel.position.y = 5;
  }
}

void collisions(Resource<NetworkCommands> netcmd,
  Query<EnemyMarker, cevy::engine::Transform, cevy::engine::TransformVelocity, SyncId> enemies,
  Query<BulletMarker, cevy::engine::Transform, BulletStats, SyncId> bullets,
  Query<PlayerStats, cevy::engine::Transform, cevy::engine::TransformVelocity, SyncId> spaceship) {

    for (auto [emarker, etm, evel, eid] : enemies) {
      for (auto [btmarker, bttm, btstats, bid] : bullets) {
        if (abs(bttm.position.x - etm.position.x) + abs(bttm.position.y - etm.position.y) + abs(bttm.position.z - etm.position.z) < 1) {
          std::cout << "enemy hit" << std::endl;
          netcmd.get().dismiss(eid);
          netcmd.get().dismiss(bid);
        }
      }
      for (auto [pstats, ptm, pvel, id] : spaceship) {
        if (abs(ptm.position.x - etm.position.x) + abs(ptm.position.y - etm.position.y) + abs(ptm.position.z - etm.position.z) < 1) {
          pstats.life -= 1;
          netcmd.get().action_with<ShipActions::SetLife>(pstats.life);
          if (pstats.life <= 0) {
            netcmd.get().dismiss(id);
          }
          netcmd.get().dismiss(eid);
        }
      }
  }
}

void bullet_mvt(Resource<Time> time, Resource<NetworkCommands> netcmd, Query<BulletMarker, cevy::engine::Transform, BulletStats, SyncId> bullets) {
  for (auto [marker, tm, stats, id] : bullets) {
    if (stats.lifetime >= stats.max_lifetime) {
      netcmd.get().dismiss(id);
    } else {
      stats.lifetime += 1;
    }
  }
}

void control_spaceship(
    Resource<Time> time,
    Query<PlayerStats, cevy::engine::Transform, cevy::engine::TransformVelocity, PlayerMarker> spaceship,
    Resource<NetworkCommands> netcmd) {
  if (!spaceship.size())
    return;
  auto [player_stats, tm, vel, marker] = spaceship.single();
  Vector v{};

  if (cevy::Keyboard::keyDown(KEY_W) && tm.position.y < 15.5)
    v.y += 1;
  if (cevy::Keyboard::keyDown(KEY_S) && tm.position.y > -15.5)
    v.y -= 1;
  if (cevy::Keyboard::keyDown(KEY_D) && tm.position.z < 28.5)
    v.z += 1;
  if (cevy::Keyboard::keyDown(KEY_A) && tm.position.z > -28.5)
    v.z -= 1;
  v = v.normalize() * player_stats.move_speed;
  v *= time.get().delta_seconds();
  if (v.eval() != 0)
    netcmd.get().action_with<ShipActions::Fly>(tm.xyz() + v);

    std::cout << "life:" << player_stats.life << std::endl;

  if (cevy::Keyboard::keyDown(KEY_SPACE)) {
    if (player_stats.next_shot <= time.get().now()) {
      player_stats.next_shot = time.get().now() + std::chrono::milliseconds(500);
      netcmd.get().action<ShipActions::Shoot>();
    }
  }
}

void lifebar(Resource<LifeResource> resource, Query<PlayerStats> player, Query<LifeMarker, cevy::engine::TransformVelocity> bar, Commands cmd) {
  static int life = 0;

  auto [stats] = player.single();
  if (stats.life > life)
    for (int i = stats.life; i != life; i--)
      for (auto [_, transform] : bar);
        //delete one
  if (stats.life < life)
    for (int i = stats.life; i != life; i++)
      cmd.spawn(resource.get().handle,
        LifeMarker(),
        engine::Transform(0, 5, 5 + 10 * (stats.life + i)).scaleXYZ(0.002)
      );
  if (stats.life != life)
    life = stats.life;
}

void set_background(Resource<ClearColor> col) {
  col.get() = ClearColor(cevy::engine::Color(0, 0, 0));
}

int server(int ac, char **av) {
  std::cout << "booting server" << std::endl;
  App app;
  app.init_component<PlayerStats>();
  app.init_component<PlayerMarker>();
  app.init_component<BulletStats>();
  app.init_component<BulletMarker>();
  app.init_component<EnemyMarker>();
  app.insert_resource(AssetManager());
  app.add_systems<core_stage::Startup>(setup_logic);
  app.add_plugins(Engine());
  app.emplace_plugin<NetworkPlugin<SpaceShipSync, ShipActions, ServerHandler>>(12345, 54321, 1);
  // app.add_systems<core_stage::Update>(control_spaceship);
  app.add_systems<core_stage::Update>(collisions);
  app.add_systems<core_stage::Update>(spawn_enemies);
  app.add_systems<core_stage::Update>(enemy_mvt);
  app.run();
  app.add_systems<core_stage::Update>(bullet_mvt);
  return 0;
}

int client(int ac, char **av) {
  std::cout << "booting client" << std::endl;
  App app;
  app.init_component<PlayerStats>();
  app.init_component<PlayerMarker>();
  app.init_component<BulletStats>();
  app.init_component<BulletMarker>();
  app.init_component<EnemyMarker>();
  app.init_component<LifeMarker>();
  app.insert_resource(AssetManager());
  app.add_plugins(Engine());
  app.add_systems<core_stage::Startup>(setup_logic);
  app.add_systems<core_stage::Startup>(setup_world);
  app.add_systems<core_stage::Startup>(set_background);
  app.add_systems<core_stage::Update>(enemy_mvt);
  app.add_systems<core_stage::Update>(control_spaceship);
  app.add_systems<core_stage::Update>(lifebar);
  app.emplace_plugin<NetworkPlugin<SpaceShipSync, ShipActions, ClientHandler>>(12345, 54321, 1);
  std::string host = ac > 1 ? av[1] : "127.0.0.1";
  app.resource<NetworkCommands>().connect(host);
  app.run();
  return 0;
}

int main(int ac, char **av) {
  if ((ac > 1) && (av[1] == std::string("--help") || av[1] == std::string("-h"))) {
    std::cout << "USAGE: ./" << av[0] << "[--server | <host=localhost>]" << std::endl;
    std::cout << "\t" << "start with --server to run as a server, or give host" << std::endl;
    return 0;
  }
  if ((ac > 1) && (av[1] == std::string("--server"))) {
    return server(ac, av);
  } else {
    return client(ac, av);
  }
}
