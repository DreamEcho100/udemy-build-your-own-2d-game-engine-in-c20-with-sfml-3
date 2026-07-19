#include "Random.h"

Random::Random() : generator_(device_()) {}

int Random::GetInt(int min, int max) { return std::uniform_int_distribution(min, max)(generator_); }

float Random::GetFloat(float min, float max) {
  return std::uniform_real_distribution(min, max)(generator_);
}
