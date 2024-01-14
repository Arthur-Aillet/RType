/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Sync
*/

#pragma once
#include <cstdint>
#include <string>

#include "Asset.hpp"
#include "Diffuse.hpp"
#include "Factory.hpp"
#include "Handle.hpp"
#include "Indirect.hpp"
#include "Mesh.hpp"
#include "cevy.hpp"
#include "ecs.hpp"
#include "ecs/App.hpp"
#include "ecs/Schedule.hpp"
#include "engine/Position.hpp"
#include "engine/Rotation.hpp"
#include "engine/assets/AssetManager.hpp"
#include "network/Synchroniser.hpp"
#include "network/CevyNetwork.hpp"
#include <functional>

enum Objects {
  EShip = 1,
};

using Ship = cevy::Synchroniser::Spawnable<Objects::EShip,
  cevy::engine::Position(),
  cevy::engine::Rotation(),
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

    add_sync<PositionSync, cevy::engine::Position, cevy::engine::Rotation>(app);

    add_spawnable<Ship>(cevy::engine::Position(), cevy::engine::Rotation(),
                  make_indirect(getDiffuse), make_indirect(getMesh));
    // add_spawnable((uint8_t)Objects::Ship, Objects::Ship);
    // registerBundle(Objects::Ship, cevy::Position(), cevy::Rotation(),
    // Indirect<cevy::Handle<cevy::Model3D>>(get3dhandle));
  }

  protected:
  private:
};
