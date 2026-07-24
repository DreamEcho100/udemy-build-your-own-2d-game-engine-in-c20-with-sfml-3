#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class TimeManager {
private:
  sf::Clock clock_;
  sf::Time deltaTime_;
  sf::Time prevTime_;

public:
  float GetDeltaTime() const;
  float GetElapsedTime() const;

private:
  friend class Engine;

  void Update();
};
