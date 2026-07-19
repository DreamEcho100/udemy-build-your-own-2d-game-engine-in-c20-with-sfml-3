#pragma once

#include <random>

class Random {
private:
  std::random_device device_;
  std::mt19937 generator_;

public:
  Random();

  int GetInt(int min, int max);
  float GetFloat(float min, float max);
};

inline Random gRandom;
