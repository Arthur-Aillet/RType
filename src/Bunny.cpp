#include "Bunny.hpp"
#include "bunny_mesh.h"

Model Bunny::model() {
  auto vx_size = sizeof(bunny_mesh::modelVertices) / sizeof(*bunny_mesh::modelVertices);
  auto i_size = sizeof(bunny_mesh::modelIndices) / sizeof(*bunny_mesh::modelIndices);
  std::vector<glm::vec4> vec_vertices; // = std::vector(bunny_mesh::modelVertices,
                                       // bunny_mesh::modelVertices + vx_size);
  std::vector<glm::vec3>
      vec_normals; // = std::vector(bunny_mesh::modelVertices, bunny_mesh::modelVertices + vx_size);
  std::vector<uint32_t>
      vec_indices; // = std::vector(bunny_mesh::modelIndices, bunny_mesh::modelIndices + i_size);

  vec_vertices.reserve(vx_size);
  vec_normals.reserve(vx_size);
  vec_indices.reserve(i_size);

  for (int i = 0; i < vx_size; ++i) {
    vec_vertices.push_back(glm::vec4(bunny_mesh::modelVertices[i].position, 1.0f));
    vec_normals.push_back(bunny_mesh::modelVertices[i].normal);
  }

  for (int i = 0; i < i_size; ++i) {
    vec_indices.push_back(bunny_mesh::modelIndices[i]);
  }

  Model bunny;
  bunny.load(vec_vertices, vec_normals, vec_indices);
  return bunny;
}
