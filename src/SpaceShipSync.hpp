/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Sync
*/

#pragma once
#include <cmath>
#include <cstdint>
#include <string>
#include <functional>

#include "cevy.hpp"
#include "main.hpp"
#include "Asset.hpp"
#include "Diffuse.hpp"
#include "Factory.hpp"
#include "Transform.hpp"
#include "Color.hpp"
#include "Velocity.hpp"
#include "Handle.hpp"
#include "Indirect.hpp"
#include "Mesh.hpp"
#include "cevy.hpp"
#include "ecs.hpp"
#include "ecs/App.hpp"
#include "ecs/Scheduler.hpp"
#include "engine/assets/AssetManager.hpp"
#include "network/Synchroniser.hpp"
#include "network/CevyNetwork.hpp"

enum Objects {
  EPlayerShip = 0,
  EShip = 1,
  EEnemy = 2,
  EBullet = 3,
};

using Ship = cevy::Synchroniser::Spawnable<Objects::EPlayerShip,
  engine::Transform,
  TransformVelocity,
  cevy::engine::Handle<cevy::engine::Diffuse>,
  cevy::engine::Handle<cevy::engine::Mesh>>;

using PlayerShip = cevy::Synchroniser::Spawnable<Objects::EShip,
  PlayerMarker,
  engine::Transform,
  TransformVelocity,
  cevy::engine::Handle<cevy::engine::Diffuse>,
  cevy::engine::Handle<cevy::engine::Mesh>>;

using Enemy = cevy::Synchroniser::Spawnable<Objects::EEnemy,
  engine::Transform,
  TransformVelocity,
  cevy::engine::Handle<cevy::engine::Diffuse>,
  cevy::engine::Handle<cevy::engine::Mesh>>;

class SpaceShipSync : public cevy::Synchroniser, public cevy::ecs::Factory<Objects> {
  public:
  struct PositionSync {};
  SpaceShipSync(cevy::CevyNetwork& net) : cevy::Synchroniser(net) {};
  // SpaceShipSync(cevy::Synchroniser &&rhs) : cevy::Synchroniser(rhs) {};
  ~SpaceShipSync(){};

  SpaceShipSync(SpaceShipSync &&rhs) : Synchroniser(std::move(rhs)), Factory(rhs){};

  void build(cevy::ecs::App &app) override {
    std::cout << "(INFO)ShipSync::build" << std::endl;
    Synchroniser::build(app);
    Factory<Objects>::build(app);
  }

  void build_custom(cevy::ecs::App &app) override {
    std::cout << "(INFO)ShipSync::build_custom" << std::endl;
    add_sync<PositionSync, engine::Transform, TransformVelocity>(app);

    auto player_ship_spawner = [&app](EntityCommands e){
      e.insert(PlayerMarker(),
        engine::Transform().rotateX(-90 * DEG2RAD),
        TransformVelocity(),
        engine::Color(220, 220, 220));

      // e.insert(
      //   app.resource<cevy::engine::Asset<cevy::engine::Diffuse>>().load("assets/space-ship1.png")
      //   );
      e.insert(
        app.resource<RtypeHandles>().player
      );

    };

    auto ship_spawner = [&app](EntityCommands e){
      e.insert(
        engine::Transform().rotateX(-90 * DEG2RAD),
        TransformVelocity(),
        engine::Color(255, 160, 30));
      // e.insert(
      //   app.resource<cevy::engine::Asset<cevy::engine::Diffuse>>().load("assets/space-ship1.png")
      //   );
      e.insert(
        app.resource<RtypeHandles>().player
      );
    };

  auto enemy_spawner = [&app](EntityCommands e){
    auto spawner = app.resource<EnemySpawner>();
    float y = rand() % 280 - 140;

    e.insert(
      spawner.handle,
      // app.resource<cevy::engine::Asset<cevy::engine::Mesh>>().load("assets/enemy.gltf"),
      engine::Transform(0, y / 10, 34).scaleXYZ(0.004).rotateY(M_PI),
      TransformVelocity(cevy::engine::Transform().translateZ(-11. - y / 140.))
    );
  };



    add_spawnable_command<Ship>(ship_spawner);
    add_spawnable_command<PlayerShip>(player_ship_spawner);
    add_spawnable_command<Enemy>(enemy_spawner);
  }

  protected:
  private:
};
