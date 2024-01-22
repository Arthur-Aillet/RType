/*
** AgarthaSoftware, 2024
** rtype
** File description:
** ShipActions
*/

#include "Diffuse.hpp"
#include "Mesh.hpp"
#include "Resource.hpp"
#include "Transform.hpp"
#include "Time.hpp"
#include "Velocity.hpp"
#include "cevy.hpp"
#include "main.hpp"
#include "network/NetworkActions.hpp"
#include "network/NetworkCommands.hpp"
#include "Vector.hpp"
#include "network/network.hpp"
#include "network/CevyNetwork.hpp"
#include "input.hpp"
#include "SpaceShipSync.hpp"

using namespace cevy;
using namespace ecs;
using namespace engine;

class ShipActions : public cevy::NetworkActions {
public:
    ShipActions(cevy::CevyNetwork &net, Synchroniser& sync) : NetworkActions(net, sync) {};


    struct Act {
    enum EAct {
        shoot,
        fly,
        flyDown,
    };
    };

    using myEvent = Event<Act::shoot, int>;

    using Shoot = cevy::NetworkActions::Action<Act::shoot, Presume::idk>;
    using Fly = cevy::NetworkActions::Action<Act::fly, Presume::success, cevy::engine::Vector>;

    void build(cevy::ecs::App& app) override {
        NetworkActions::build(app);
        // return;
        auto spawn_ship = make_function<bool, CevyNetwork::ConnectionDescriptor>(
            [&app](CevyNetwork::ConnectionDescriptor cd){
                app.resource<NetworkCommands>().summon<PlayerShip, Ship>(cd);
                return true;
            });
        on_client_join(spawn_ship);
        std::cout << Shoot::value << std::endl;
        // add_event<ClientJoin>();
        add_action<Shoot>(make_function(shootServerAction), make_function(shootAction), make_function(shootFailAction));
        add_action_with<Fly>(make_function(flyServerAction), make_function(flySuccessAction), make_function(flyFailureAction));
    }


    static EActionFailureMode flyServerAction(cevy::engine::Vector vec) {return cevy::CevyNetwork::ActionFailureMode::EActionFailureMode::Action_Success;};
    static bool flySuccessAction(cevy::engine::Vector vec) { return true;};
    static bool flyFailureAction(EActionFailureMode, cevy::engine::Vector vec) { return true;};

    static EActionFailureMode shootServerAction(Resource<Asset<cevy::engine::Mesh>> meshs, Resource<BulletHandle> bullet_handle,
                  Resource<Time> time, Resource<Asset<Diffuse>> difs, Commands cmd,
                  Query<PlayerStats, cevy::engine::Transform> players) {
  for (auto [player_stats, tm] : players) {
    player_stats.time_before_shoot.tick(time.get().delta());
    if (player_stats.time_before_shoot.finished()) {
      if (cevy::Keyboard::keyDown(KEY_SPACE)) {
       cmd.spawn(
        bullet_handle.get().handle, TransformVelocity(cevy::engine::Transform().setPositionZ(30)),
        engine::Transform(tm.position).translateZ(1).rotateX(90 * DEG2RAD).scaleXYZ(0.004));
        player_stats.time_before_shoot.reset();
      }
    }};
        return cevy::CevyNetwork::ActionFailureMode::Action_Success;
    }
    // static bool shootAction(cevy::ecs::Query<> q) {};

    static bool shootAction() {
        return true;
    };

    static bool shootFailAction(EActionFailureMode) {
        return true;
    };

private:
};
