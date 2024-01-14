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

class ShipActions : cevy::NetworkActions {
public:
    ShipActions(cevy::CevyNetwork &net) : NetworkActions(net) {};

    struct Act {
    enum EAct {
        shoot,
        fly,
        flyDown,
    };
    };
    using Shoot = Action<Act::shoot, Presume::idk>;
    using Fly = Action<Act::fly, Presume::success, cevy::engine::Vector>;

    using myEvent = Event<Act::shoot, int>;

    void build(cevy::ecs::App& app) override {
        add_event_with<myEvent>((boomEvent));
        add_action<Shoot>(shootServerAction, shootAction, shootAction);
        add_action_with<Fly>(flyServerAction, flySuccessAction, flyFailureAction);
    }

    void some_system(cevy::ecs::Resource<cevy::NetworkCommands> netcmd) {
        const bool space_bar = true; //dummy
        if (space_bar) {
            netcmd.get().action_with<Fly>(cevy::engine::Vector(0, 0, 1));
        }
        if (space_bar) {
            netcmd.get().action<Shoot>();
        }
    }

    static EActionFailureMode flyServerAction(cevy::engine::Vector vec, cevy::ecs::Query<> q) {return cevy::CevyNetwork::ActionFailureMode::EActionFailureMode::Action_Success;};
    static void flySuccessAction(cevy::engine::Vector vec, cevy::ecs::Query<> q) {};
    static void flyFailureAction(cevy::engine::Vector vec, cevy::ecs::Query<> q) {};

    static EActionFailureMode shootServerAction(cevy::ecs::Query<> q) {return cevy::CevyNetwork::ActionFailureMode::EActionFailureMode::Action_Success;};
    static void shootAction(cevy::ecs::Query<> q) {};

    static void boomEvent(int a, cevy::ecs::Query<> q) {};
private:
};