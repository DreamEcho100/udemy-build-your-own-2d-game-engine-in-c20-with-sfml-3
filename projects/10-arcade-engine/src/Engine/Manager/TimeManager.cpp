#include <algorithm>

#include "Engine/Core/EngineConfig.h"
#include "TimeManager.h"

void TimeManager::Update() {
  const sf::Time currTime = clock_.getElapsedTime();
  deltaTime_ = std::min(currTime - prevTime_, gConfig.maxDeltaTime);
  prevTime_ = currTime;
}

float TimeManager::GetDeltaTime() const { return deltaTime_.asSeconds(); }
float TimeManager::GetElapsedTime() const { return clock_.getElapsedTime().asSeconds(); }
