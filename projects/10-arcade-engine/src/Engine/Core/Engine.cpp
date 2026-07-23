#include "Engine.h"
// #include "Engine/Core/Engine.h"
#include "EngineConfig.h"

Engine::Engine() : window_(sf::VideoMode(sf::Vector2u(gConfig.windowSize)), gConfig.windowTitle) {
  window_.setIcon(sf::Image("assets/Textures/Icon.png"));
  window_.setMinimumSize(window_.getSize() / 2u);
}

bool Engine::IsRunning() const { return window_.isOpen(); }

void Engine::ProccessEvents() {
  while (const auto event = window_.pollEvent()) {
    // event->visit([this](const auto& type) { EngineVisitor{*this}.HandleEvent(type); });
    event->visit(EngineVisitor{*this});
  }
}

void Engine::Update() {}

void Engine::Render() {
  window_.clear();
  window_.display();
}

void Engine::EventWindowClose() { window_.close(); }
