/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Sync
*/

#pragma once
#include <cstdint>
#include <string>
#include <functional>

#include "main.hpp"
#include "Asset.hpp"
#include "Diffuse.hpp"
#include "Factory.hpp"
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
  EShip = 1,
  EEnemy = 1,
  EBullet = 3,
};

using Ship = cevy::Synchroniser::Spawnable<Objects::EShip,
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
    Synchroniser::build(app);
    Factory<Objects>::build(app);
  }

  void build_custom(cevy::ecs::App &app) override {
    std::function<cevy::engine::Handle<cevy::engine::Diffuse>()> getDiffuse =
        [&app]() -> cevy::engine::Handle<cevy::engine::Diffuse> {
      return app.resource<cevy::engine::Asset<cevy::engine::Diffuse>>().load(
          "assets/space-ship1.png");
    };

    std::function<cevy::engine::Handle<cevy::engine::Mesh>()> getMesh =
        [&app]() -> cevy::engine::Handle<cevy::engine::Mesh> {
      return app.resource<cevy::engine::Asset<cevy::engine::Mesh>>().load("assets/space-ship1.obj");
    };

      std::function<cevy::engine::Handle<cevy::engine::Mesh>()> getEnemyMesh =
        [&app]() -> cevy::engine::Handle<cevy::engine::Mesh> {
      return app.resource<EnemySpawner>().handle;
    };

    add_sync<PositionSync, engine::Transform, TransformVelocity>(app);

    add_spawnable<Ship>(engine::Transform().rotateX(-90 * DEG2RAD),
                  TransformVelocity(),
                  make_indirect(getDiffuse),
                  make_indirect(getMesh)),
                  engine::Color(0, 0, 255);

    add_spawnable<PlayerShip>(PlayerMarker(),
                  engine::Transform().rotateX(-90 * DEG2RAD),
                  TransformVelocity(),
                  make_indirect(getDiffuse),
                  make_indirect(getMesh)),
                  engine::Color(255, 0, 0);

    add_spawnable<Enemy>(make_indirect(make_function([](){return engine::Transform(0, rand() % 280 - 140, 34).scaleXYZ(0.004).rotateY(180 * DEG2RAD);})),
                  TransformVelocity(cevy::engine::Transform().translateZ(2. / 140.))
                  make_indirect(getDiffuse),
                  make_indirect(getEnemyMesh));

    // add_spawnable((uint8_t)Objects::Ship, Objects::Ship);
    // registerBundle(Objects::Ship, cevy::Position(), cevy::Rotation(),
    // Indirect<cevy::Handle<cevy::Model3D>>(get3dhandle));
  }

  protected:
  private:
};
