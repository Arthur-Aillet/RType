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

using namespace cevy;
using namespace ecs;
using namespace engine;

struct EnemySpawner {
  cevy::engine::Timer time_before_spawn;
  double spawn_increase_perc;
  Handle<cevy::engine::Mesh> handle;
};

struct PlayerStats {
  size_t i;
  cevy::engine::Timer time_before_shoot;
  float move_speed;
};

struct BulletHandle {
  cevy::engine::Handle<cevy::engine::Mesh> handle;
};

struct RtypeHandles {
  cevy::engine::Handle<cevy::engine::Mesh> bullet;
  cevy::engine::Handle<cevy::engine::Mesh> player;
  cevy::engine::Handle<cevy::engine::Mesh> enemy;
};

struct PlayerMarker {};
