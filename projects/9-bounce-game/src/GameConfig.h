#pragma once

#include <SFML/Graphics.hpp>

struct GameConfig {
  sf::Vector2f windowSize;
  sf::Vector2f paddleSize;
  float paddleSpeed;
  float ballRadius;
  float ballSpeed;
  float ballSpawnCooldown;
  float themeMusicVolume;
  float bounceSoundVolume;
  int lives;

  GameConfig();
};

inline const GameConfig gConfig;
