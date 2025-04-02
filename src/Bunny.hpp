#pragma once
#include "Model.hpp"

using Model = cevy::engine::Model;

class Bunny {
  public:
  Model static model();

  protected:
  Bunny() = default;
  ~Bunny() = default;

  protected:
  private:
};
