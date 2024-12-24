#include <tuple>
#include <vector>
#define GLM_FORCE_SWIZZLE


#include "Asset.hpp"
#include "AssetManager.hpp"
#include "Color.hpp"
#include "PbrMaterial.hpp"
#include "commands/EntityCommands.hpp"
#include "engine/Engine.hpp"
#include "commands/Commands.hpp"
#include "engine/Transform.hpp"
#include "engine/Camera.hpp"
#include "ecs/App.hpp"
#include "pipeline.hpp"
#include "Bunny.hpp"

using namespace cevy;
using namespace ecs;
using namespace engine;

float DEG2RAD = glm::pi<float>() / 180;

static glm::vec3 hsv2rgb(glm::vec3 c) {
  glm::vec4 K = glm::vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  glm::vec3 p = abs(fract(c.xxx() + K.xyz()) * 6.0f - K.www());
  return c.z * mix(K.xxx(), clamp(p - K.xxx(), 0.0f, 1.0f), c.y);
}


int initial_setup(Resource<Asset<cevy::engine::Model>> mesh_manager, Resource<Asset<cevy::engine::PbrMaterial>> material_manager,
                   Commands cmd, World &w) {
  auto& meshs = mesh_manager.get();
  auto &mats = material_manager.get();
  auto bunny_handle = meshs.load(Bunny::model());
  auto mat_h = mats.load(PbrMaterial(glm::vec3(0., 1., 1.), glm::vec3(1), 12));

  cmd.spawn(Transform(), bunny_handle, mat_h, Color(0, 0, 1));
  cmd.spawn(cevy::engine::Camera(),
            cevy::engine::Transform(glm::vec3(0, -2, -10)));
  // cmd.spawn(cevy::engine::Camera(),
  //           cevy::engine::Transform());

  const int ringCount = 5;
  const float ringRadius = 20;
  for (int i = 0; i < ringCount; i++) {
    glm::vec3 rgb = 200.f * hsv2rgb({float(i) / ringCount, 0.9, 1.0f});
    Transform tm = Transform(glm::vec3(ringRadius * std::cos(glm::two_pi<float>() * float(i) / ringCount), 15.0f,
                      ringRadius * std::sin(glm::two_pi<float>() * float(i) / ringCount)));
    PointLight light = {rgb, 1.0f};
    cmd.spawn(tm, light);
  }

  return 0;
}

int main() {
  App app;
  app.init_resource<AssetManager>();
  app.add_plugins(Engine());
  app.add_systems<core_stage::Startup>(initial_setup);
  app.run();
}
