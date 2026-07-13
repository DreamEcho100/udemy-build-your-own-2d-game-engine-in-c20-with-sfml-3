#include "Application.h"

#include <iostream>

Application::Application() : window_(sf::VideoMode({800, 600}), "SFML Window?") {
  window_.setFramerateLimit(60);

  player_.setSize({200, 100});
  player_.setOrigin(player_.getGeometricCenter());
  player_.setPosition({100, 300});
  player_.setFillColor(sf::Color::Cyan);
}

bool Application::IsRunning() const { return window_.isOpen(); }

void Application::ProcessEvents() {
  window_.handleEvents([this](const auto& event) { this->HandleEvent(event); });
}

void Application::Update() {
  float dt = clock_.restart().asSeconds();

  if (player_.getPosition().x -
      ((player_.getSize().x * player_.getScale().x) < window_.getSize().x) * 0.5) {
    player_.move({playerMoveSpeed_ * dt, 0});
    float factor = 1 - playerScaleSpeed_ * dt;
    player_.scale({factor, factor});
    player_.rotate(sf::degrees(playerRotationSpeed_ * dt));
  }
}

void Application::Render() {
  window_.clear(sf::Color::Black);
  window_.draw(player_);
  window_.display();
}

void Application::HandleEvent(const sf::Event::Closed&) { window_.close(); }

void Application::HandleEvent(const sf::Event::Resized& event) {
  std::cout << "Size:" << event.size.x << "x" << event.size.y << "\n";
}

void Application::HandleEvent(const sf::Event::FocusLost&) { std::cout << "Window lost focus!\n"; }
void Application::HandleEvent(const sf::Event::FocusGained&) {
  std::cout << "Window gained focus!\n";
}

void Application::HandleEvent(const sf::Event::KeyPressed& event) {
  if (event.code == sf::Keyboard::Key::Z) {
    std::cout << "Logical Z key pressed\n";
  }
  if (event.scancode == sf::Keyboard::Scan::Z) {
    std::cout << "Physical Z key pressed\n";
  }
}

void Application::HandleEvent(const sf::Event::MouseWheelScrolled& event) {
  std::cout << "Scrolled by: " << event.delta << "\n";
}

void Application::HandleEvent(const sf::Event::MouseButtonPressed& event) {}

void Application::HandleEvent(const auto&) {}
