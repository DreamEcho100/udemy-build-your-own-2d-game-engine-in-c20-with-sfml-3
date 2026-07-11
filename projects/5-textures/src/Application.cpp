#include "Application.h"

#include <iostream>

Application::Application()
    : window_(sf::VideoMode({600, 800}), "SFML Window"), texture_("./assets/spaceship.png"),
      sprite_(texture_) {

  texture_.setSmooth(true);

  player_.setTexture(&texture_);
  player_.setSize({64, 64});
  player_.setFillColor(sf::Color::Blue);
  player_.setPosition({200, 200});

  enemy_.setTexture(&texture_);
  enemy_.setSize({64, 64});
  enemy_.setFillColor(sf::Color::Blue);
  enemy_.setPosition({200, 400});

  sprite_.setPosition({350, 250});
  sprite_.setScale({0.5, 0.5});
  sprite_.setColor(sf::Color::Green);
}

bool Application::IsRunning() const { return window_.isOpen(); }

void Application::ProcessEvents() {
  window_.handleEvents([this](const auto& event) { this->HandleEvent(event); });
}

void Application::Update() {}

void Application::Render() {
  window_.clear();
  window_.draw(player_);
  window_.draw(enemy_);
  window_.draw(sprite_);
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
  if (event.code == sf::Keyboard::Key::W) {
    std::cout << "\"W\" pressed\n";
  }
}

void Application::HandleEvent(const sf::Event::MouseWheelScrolled& event) {
  std::cout << "Scrolled by: " << event.delta << "\n";
}

void Application::HandleEvent(const sf::Event::MouseButtonPressed& event) {
  std::cout << "Mouse button pressed at";
  std::cout << event.position.x << ", " << event.position.y << "\n";
}

void Application::HandleEvent(const auto&) {}
