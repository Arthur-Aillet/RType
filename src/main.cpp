#include <iostream>

#include "App.hpp"
#include "AssetManager.hpp"
#include "Commands.hpp"
#include "Position.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"
#include "ecs.hpp"
#include "engine/Engine.hpp"

using namespace cevy;
using namespace ecs;

void create_space_ship(Resource<Asset<Model3D>> mod, Commands cmd) {
    auto handle = mod.get().load(
        Settings3D {
            .model = "assets/space-ship1.obj",
            .diffuse = "assets/space-ship1.png"
        }
    );

    cmd.spawn(Position(), handle);
}

int main() {
    App app;
    app.add_plugins(Engine());
    app.add_system<Schedule::Startup>(create_space_ship);
    app.run();
    return 0;
}
