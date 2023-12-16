#include <iostream>

#include "App.hpp"
#include "engine/Engine.hpp"

int main() {
    cevy::ecs::App app;
    app.add_plugins(cevy::Engine());
    app.run();
    return 0;
}
