#include "App.hpp"
#include "Asset.hpp"
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"
#include "Line.hpp"
#include "Position.hpp"
#include "Query.hpp"
#include "Rotation.hpp"
#include "Schedule.hpp"
#include "Target.hpp"
#include "Time.hpp"
#include "Vector.hpp"
#include "ecs.hpp"
#include "engine/Engine.hpp"
#include "input.hpp"
#include "raylib.hpp"

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

  cmd.spawn(Position(), Rotation(0.0, 0.0, 0.0), /*handle_mesh, handle_difs,*/ PlayerMarker());
}

void control_spaceship(Resource<Time> time,
                      Query<Line> lines,
                       Query<cevy::engine::Camera, Target, cevy::engine::Position> cams,
                       Query<PlayerMarker, Rotation, Position> spaceship) {
  for (auto [space, rot, pos] : spaceship) {
    float delta = 30 * time.get().delta_seconds();
    if (cevy::Keyboard::keyDown(KEY_A)) {
      rot.rotate({delta, 0, 0});
    }
    if (cevy::Keyboard::keyDown(KEY_D)) {
      rot.rotate({-delta, 0, 0});
    }
    if (cevy::Keyboard::keyDown(KEY_S)) {
      rot.rotate({0, delta, 0});
    }
    if (cevy::Keyboard::keyDown(KEY_W)) {
      rot.rotate({0, -delta, 0});
    }
    if (cevy::Keyboard::keyDown(KEY_Q)) {
      rot.rotate({0, 0, delta});
    }
    if (cevy::Keyboard::keyDown(KEY_E)) {
      rot.rotate({0, 0, -delta});
    }


    auto [one, two, three] = lines.multiple<3>();
    auto [fst] = one;
    auto [snd] = two;
    auto [trd] = three;

    Vector new_u = Position (5, 0, 0);
    Vector new_v = Position (0, 5, 0);
    Vector new_w = Position (0, 0, 5);
    new_u.rotate(rot * DEG2RAD);
    new_v.rotate(rot * DEG2RAD);
    new_w.rotate(rot * DEG2RAD);
    fst.end = new_u;
    snd.end = new_v;
    trd.end = new_w;

    //for (auto [cam, target, c_pos] : cams) {
    //  Vector camera_target = pos + Vector(0, 5, 5);
    //  camera_target.rotate(rot * DEG2RAD);
    //  target = camera_target;
    //  Vector camera_pos = pos + Vector(0, 10, -10);
    //  c_pos = camera_pos;
    //}
  }
}

int main() {
  struct SpaceShip {};
  App app;
  app.init_component<PlayerMarker>();
  app.insert_resource(AssetManager());
  app.add_plugins(Engine());
  app.add_system<Schedule::Startup>(create_player_ship);
  app.add_system<Schedule::Update>(control_spaceship);
  app.spawn(Line(Position (0, 0, 0), Position (5, 0, 0)), cevy::engine::Color(255, 200, 0));
  app.spawn(Line(Position (0, 0, 0), Position (0, 5, 0)), cevy::engine::Color(0, 255, 200));
  app.spawn(Line(Position (0, 0, 0), Position (0, 0, 5)), cevy::engine::Color(0, 200, 255));
  app.spawn(Line(Position (0, 0, -5), Position (0, 4, -5)), cevy::engine::Color(255, 255, 0));
  app.spawn(Line(Position (0, 0, +5), Position (0, 3, +5)), cevy::engine::Color(255, 0, 255));

  app.spawn(Line(Position (0, 0, 0), Position (4, 0, 0)), cevy::engine::Color(255, 0, 0, 50));
  app.spawn(Line(Position (0, 0, 0), Position (0, 4, 0)), cevy::engine::Color(0, 255, 0, 50));
  app.spawn(Line(Position (0, 0, 0), Position (0, 0, 4)), cevy::engine::Color(0, 0, 255, 50));

  app.spawn(cevy::engine::Camera(), cevy::engine::Position(10.0, 10.0, 10),
            cevy::engine::Rotation(0.0, 0.0, 0.0), cevy::engine::Target(0.0, 0.0, 0.0));
  app.run();
  return 0;
}
