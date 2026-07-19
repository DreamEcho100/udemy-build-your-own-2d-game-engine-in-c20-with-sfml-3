#pragma once

#include <SFML/Audio.hpp>

class GameSounds {
private:
  sf::Music themeMusic_;

  sf::SoundBuffer bounceSoundBuffer_;
  sf::Sound bounceSound_;

public:
  GameSounds();

  void PlayBounceSound();
  void Restart();
};
