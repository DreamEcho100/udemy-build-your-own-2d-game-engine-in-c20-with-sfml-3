#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

struct EngineConfig {
  std::string windowTitle;
  sf::Vector2f windowSize;
  bool disableSfmlLogs;
  sf::Time maxDeltaTime;

  EngineConfig();
};

inline const EngineConfig gConfig;
