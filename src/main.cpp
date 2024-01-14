#include "App.hpp"
#include "Asset.hpp"
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"
#include "Position.hpp"
#include "Query.hpp"
#include "Rotation.hpp"
#include "Schedule.hpp"
#include "SpaceShipSync.hpp"
#include "ShipActions.hpp"
#include "Time.hpp"
#include "Vector.hpp"
#include "ecs.hpp"
#include "engine/Engine.hpp"
#include "input.hpp"
#include "network/NetworkPlugin.hpp"
#include "raylib.h"

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

  cmd.spawn(Position(), Rotation(0.0, 1.0, 0.0), handle_mesh, handle_difs, PlayerMarker());
}

void rotate_camera(Resource<Time> time, Query<cevy::engine::Camera, cevy::engine::Rotation> cams) {
  for (auto [cam, rot] : cams) {
    if (cevy::Keyboard::keyDown(KEY_A)) {
      rot.x -= 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_D)) {
      rot.x += 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_S)) {
      rot.y -= 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_W)) {
      rot.y += 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_Q)) {
      rot.z -= 30 * time.get().delta_seconds();
    }
    if (cevy::Keyboard::keyDown(KEY_E)) {
      rot.z += 30 * time.get().delta_seconds();
    }
  }
}

int main(int ac, char **argv) {
  struct SpaceShip {};
  App app;
  app.init_component<PlayerMarker>();
  app.insert_resource(AssetManager());
  app.add_plugins(Engine());
  app.add_system<Schedule::Startup>(create_player_ship);
  app.add_system<Schedule::Update>(rotate_camera);
  app.spawn(cevy::engine::Camera(), cevy::engine::Position(-10.0, 10.0, 10.0),
            cevy::engine::Rotation(0.0, 0.0, 0.0));


    // void start_server() {
    // NetworkBase server = NetworkBase(cevy::NetworkBase::NetworkMode::Server, "127.0.0.1", 12345, 54321, 1);
    // std::cout << "setting up acceptor;" << std::endl;
    // server.tcp_accept_new_connexion();
    // std::cout << "setting up udp read;" << std::endl;
    // // server.readUDP();
    // server.start_thread();
    // std::cout << "running" << std::endl;
    // server._nw_thread.join();
    // }

    // void start_client() {
    //       NetworkBase client = NetworkBase(cevy::NetworkBase::NetworkMode::Client, "127.0.0.1", 12345, 54321, 1);
    // client.tcp_client_connect();
    // client.start_thread();
    // client._nw_thread.join();
    // }


  // CevyNetwork(NetworkMode mode, const std::string &endpoint, size_t udp_port, size_t tcp_port,
  //             size_t client_offset) : NetworkBase(mode, endpoint, udp_port, tcp_port, client_offset){};

  // cevy::NetworkBase ntw("127.0.0.1", 54321);
  if (ac == 1) {
    NetworkPlugin<SpaceShipSync, ShipActions>(CevyNetwork(cevy::NetworkBase::NetworkMode::Server, "127.0.0.1", 12345, 54321, 0));
  } else {
    NetworkPlugin<SpaceShipSync, ShipActions>(CevyNetwork(cevy::NetworkBase::NetworkMode::Client, "127.0.0.1", 12345, 54321, 1));
  }
  return 0;

}