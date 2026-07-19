#include "GameSounds.h"
#include "GameConfig.h"

GameSounds::GameSounds()
    : themeMusic_("assets/Theme.mp3"), bounceSoundBuffer_("assets/Bounce.mp3"),
      bounceSound_(bounceSoundBuffer_) {
  themeMusic_.setVolume(gConfig.themeMusicVolume);
  themeMusic_.setLooping(true);
  themeMusic_.play();

  bounceSound_.setVolume(gConfig.bounceSoundVolume);
}

void GameSounds::PlayBounceSound() { bounceSound_.play(); }

void GameSounds::Restart() {
  themeMusic_.setPlayingOffset(sf::Time::Zero);
  themeMusic_.play();
}
