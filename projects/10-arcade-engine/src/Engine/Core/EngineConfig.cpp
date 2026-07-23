#include <cassert>
#include <fstream>
#include <nlohmann/json.hpp>

#include "EngineConfig.h"

EngineConfig::EngineConfig() {
  std::ifstream file("assets/Config.json");
  assert(file);

  nlohmann::json json = nlohmann::json::parse(file);
  windowTitle = json["windowTitle"];
  windowSize = {json["windowSize"][0], json["windowSize"][1]};
}
