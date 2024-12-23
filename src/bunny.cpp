#include "Asset.hpp"
#include "AssetManager.hpp"
#include "engine/Engine.hpp"
#include "commands/Commands.hpp"
#include "commands/EntityCommands.hpp"
#include "engine/Transform.hpp"
#include "engine/Camera.hpp"
#include "ecs/App.hpp"
#include "Bunny.hpp"

using namespace cevy;
using namespace ecs;
using namespace engine;

float DEG2RAD = glm::pi<float>() / 180;

int initial_setup(Resource<Asset<cevy::engine::Model>> mesh_manager, Resource<Asset<Diffuse>> difs,
                   Commands cmd, World &w) {
  auto& meshs = mesh_manager.get();
  auto bunny_handle = meshs.load(Bunny::model());
  cmd.spawn(Transform(), bunny_handle);
  cmd.spawn(cevy::engine::Camera(),
            cevy::engine::Transform(glm::vec3(0, -2, -10)));
  // cmd.spawn(cevy::engine::Camera(),
  //           cevy::engine::Transform());

  return 0;
}

int main() {
  App app;
  app.init_resource<AssetManager>();
  app.add_plugins(Engine());
  app.add_systems<core_stage::Startup>(initial_setup);
  app.run();
}
