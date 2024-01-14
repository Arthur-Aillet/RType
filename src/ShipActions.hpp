/*
** AgarthaSoftware, 2024
** rtype
** File description:
** ShipActions
*/

#include "Resource.hpp"
#include "network/NetworkActions.hpp"
#include "network/NetworkCommands.hpp"
#include "Vector.hpp"
#include "network/network.hpp"
#include "network/CevyNetwork.hpp"

    struct Act {
    enum EAct {
        shoot,
        fly,
        flyDown,
    };
    };
    using Shoot = cevy::NetworkActions::Action<Act::shoot, Presume::idk>;
    using Fly = cevy::NetworkActions::Action<Act::fly, Presume::success, cevy::engine::Vector>;

class ShipActions : public cevy::NetworkActions {
public:
    ShipActions(cevy::CevyNetwork &net) : NetworkActions(net) {};

    using myEvent = Event<Act::shoot, int>;

    void build(cevy::ecs::App& app) override {
        std::cout << Shoot::value << std::endl;
        add_action<Shoot>(shootServerAction, shootAction, shootAction);
        add_action_with<Fly>(flyServerAction, flySuccessAction, flyFailureAction);
    }

    void some_system(cevy::ecs::Resource<cevy::NetworkCommands> netcmd) {
        const bool space_bar = true;
        if (space_bar) {
            netcmd.get().action_with<Fly>(cevy::engine::Vector(0, 0, 1));
        }
        if (space_bar) {
            netcmd.get().action<Shoot>();
        }
    }

    static EActionFailureMode flyServerAction(cevy::engine::Vector vec) {return cevy::CevyNetwork::ActionFailureMode::EActionFailureMode::Action_Success;};
    static bool flySuccessAction(cevy::engine::Vector vec) { return true;};
    static bool flyFailureAction(cevy::engine::Vector vec) { return true;};

    static EActionFailureMode shootServerAction(cevy::ecs::Query<> q) {return cevy::CevyNetwork::ActionFailureMode::EActionFailureMode::Action_Success;};
    // static bool shootAction(cevy::ecs::Query<> q) {};

    static EActionFailureMode shootAction(Resource<Asset<cevy::engine::Mesh>> meshs, Resource<BulletHandle> bullet_handle,
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
    }
  }
}

    static bool boomEvent(int a, cevy::ecs::Query<> q) {};
private:
};
