#include "Application.h"

#include <iostream>

Application::Application() : window_(sf::VideoMode({800, 600}), "SFML Window?") {
  window_.setFramerateLimit(60);

  player_.setRadius(60);
  player_.setOrigin(player_.getGeometricCenter());
  player_.setPosition(
      {static_cast<float>(window_.getSize().x) / 2, static_cast<float>(window_.getSize().y) / 2});
  player_.setFillColor(sf::Color::Magenta);
}

bool Application::IsRunning() const { return window_.isOpen(); }

void Application::ProcessEvents() {
  window_.handleEvents([this](const auto& event) { this->HandleEvent(event); });
}

void Application::Update() {
  if (isGrowing_) {
    player_.scale({1.01f, 1.01f});
  } else {
    player_.scale({0.99f, 0.99f});
  }

  frameCount_++;

  if (frameCount_ % 100 == 0) {
    isGrowing_ = !isGrowing_;
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
