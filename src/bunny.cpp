#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/fwd.hpp>
#include "Velocity.hpp"
#include "Asset.hpp"
#include "AssetManager.hpp"
#include "Color.hpp"
#include "PbrMaterial.hpp"
#include "commands/EntityCommands.hpp"
#include "Engine.hpp"
#include "commands/Commands.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "App.hpp"
#include "Stage.hpp"
#include "pipeline.hpp"
#include "Bunny.hpp"
#include "Query.hpp"

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
                   Commands cmd) {
  auto& meshs = mesh_manager.get();
  auto &mats = material_manager.get();
  auto bunny_handle = meshs.load(Bunny::model());
  auto mat_h = mats.load(PbrMaterial(glm::vec3(0.1, .1, .1), glm::vec3(1), 12));

  // auto bunny = cmd.spawn(Transform(), bunny_handle, mat_h, Color(0, 0, 1));
  cmd.spawn(cevy::engine::Camera(),
            cevy::engine::Transform(glm::vec3(0, -2, -10)));


  // std::cout << "added bunny...:" << bunny.id() << std::endl;

  auto rotator = cmd.spawn(bunny_handle, mat_h, Color(0, 0, 1), Transform(glm::vec3(0, 0, 0), glm::quat({0, 0, 0}), glm::vec3(1, 1, 1)), TransformVelocity(Transform(glm::vec3(0, 0, 0), glm::quat({0, DEG2RAD * 90, 0}), glm::vec3(1, 1, 1))));
  std::cout << "added rotator...:" << rotator.id() << std::endl;

  const int ringCount = 5;
  const float ringRadius = 5;
  for (int i = 0; i < ringCount; i++) {
    glm::vec3 rgb = 20.f * hsv2rgb({float(i) / ringCount, 0.9, 1.0f});
    Transform tm = Transform(glm::vec3(ringRadius * std::cos(glm::two_pi<float>() * float(i) / ringCount), 3.0f,
                      ringRadius * std::sin(glm::two_pi<float>() * float(i) / ringCount)), glm::quat({0, 0, 0}), glm::vec3(.5, .5, .5));
    PointLight light = {rgb, 1.0f};
    auto entity = cmd.spawn(Parent{rotator.id()}, tm, light, bunny_handle, mat_h, TransformVelocity(glm::quat(glm::vec3(0, 0, -0.01))));
  }

  return 0;
}

int main() {
  App app;
  app.init_resource<AssetManager>();
  app.add_plugins(Engine());
  app.add_systems<cevy::ecs::core_stage::PostStartup>(initial_setup);
  app.run();
}
