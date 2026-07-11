#include "Application.h"

#include <iostream>

Application::Application() : window_(sf::VideoMode({600, 800}), "SFML Window") {
  // window_.create(sf::VideoMode({600, 800}), "SFML Window");

  rect_.setSize({200, 100});
  rect_.setPosition({100, 150});
  rect_.setFillColor(sf::Color::Blue);
  rect_.setOutlineThickness(5);
  rect_.setOutlineColor(sf::Color::Green);

  circle_.setRadius(50);
  circle_.setPosition({400, 300});
  circle_.setFillColor({255, 165, 0});
  window_.setFramerateLimit(60);

  polygon_.setPointCount(4);
  polygon_.setPoint(0, {0, 0});
  polygon_.setPoint(1, {100, 0});
  polygon_.setPoint(2, {120, 100});
  polygon_.setPoint(3, {20, 80});
  polygon_.setPosition({150, 400});
  polygon_.setFillColor(sf::Color::Cyan);
  polygon_.setOutlineThickness(3);
  polygon_.setOutlineColor(sf::Color::Red);
}

bool Application::IsRunning() const { return window_.isOpen(); }

void Application::ProcessEvents() {
  window_.handleEvents([this](const auto& event) { this->HandleEvent(event); });
}

void Application::Update() {}

void Application::Render() {
  window_.clear();
  window_.draw(rect_);
  window_.draw(circle_);
  window_.draw(polygon_);
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
