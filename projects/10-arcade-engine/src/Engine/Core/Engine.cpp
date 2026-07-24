#include "Engine.h"
// #include "Engine/Core/Engine.h"
#include "Engine/Utils/Log.h"
#include "EngineConfig.h"

Engine::Engine() : window_(sf::VideoMode(sf::Vector2u(gConfig.windowSize)), gConfig.windowTitle) {
  window_.setIcon(sf::Image("assets/Textures/Icon.png"));
  window_.setMinimumSize(window_.getSize() / 2u);

  if (gConfig.disableSfmlLogs) {
    sf::err().rdbuf(nullptr);
  }

  context_.save.Set<int>("score", 42);
  int score = context_.save.Get<int>("score");
  LOG_INFO("Score: {}", score);

  LOG_INFO("Window created");
}

bool Engine::IsRunning() const { return window_.isOpen(); }

void Engine::ProccessEvents() {
  while (const auto event = window_.pollEvent()) {
    // event->visit([this](const auto& type) { EngineVisitor{*this}.HandleEvent(type); });
    event->visit(EngineVisitor{*this});
  }
}

void Engine::Update() {
  context_.time.Update();
  //
}

void Engine::Render() {
  window_.clear();
  window_.display();
}

void Engine::EventWindowClose() {
  window_.close();
  LOG_INFO("Window closed after {:0.2f} seconds", context_.time.GetElapsedTime());
}
void Engine::EventWindowResized(const sf::Event::Resized& event) {
  LOG_INFO("Window resized to: {}x{}", event.size.x, event.size.y);
}
