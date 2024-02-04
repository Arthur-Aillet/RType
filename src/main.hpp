/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** main
*/

#pragma once

#include "Time.hpp"
#include "Timer.hpp"
#include "engine/Engine.hpp"
#include "ecs.hpp"
#include "Asset.hpp"
#include "AssetManager.hpp"
#include "network/Synchroniser.hpp"

using namespace cevy;
using namespace ecs;
using namespace engine;

using SyncId = Synchroniser::SyncId;

struct EnemySpawner {
  cevy::engine::Timer time_before_spawn;
  double spawn_increase_perc;
  Handle<cevy::engine::Mesh> handle;
};

struct LifeResource {
    Handle<cevy::engine::Mesh> handle;
};

struct PlayerStats {
  size_t i;
  cevy::ecs::Time::time_point next_shot;
  float move_speed;
  int life;
};

struct BulletStats {
  double max_lifetime;
  double lifetime;
  double amage;
};

struct BulletHandle {
  cevy::engine::Handle<cevy::engine::Mesh> handle;
};

struct RtypeHandles {
  cevy::engine::Handle<cevy::engine::Mesh> bullet;
  cevy::engine::Handle<cevy::engine::Mesh> player;
  cevy::engine::Handle<cevy::engine::Mesh> enemy;
  cevy::engine::Handle<cevy::engine::Mesh> life;
};

struct PlayerMarker {};

struct LifeMarker {};

struct BulletMarker {};

struct EnemyMarker {};
