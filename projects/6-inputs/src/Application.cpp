#include "Application.h"

#include <iostream>

Application::Application() : window_(sf::VideoMode({600, 800}), "SFML Window") {}

bool Application::IsRunning() const { return window_.isOpen(); }

void Application::ProcessEvents() {
  window_.handleEvents([this](const auto& event) { this->HandleEvent(event); });
}

void Application::Update() {
  sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
  std::cout << mousePos.x << ' ' << mousePos.y << '\n';

  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    sf::CircleShape dot;
    dot.setRadius(5);
    dot.setFillColor(sf::Color::Red);
    dot.setPosition({mousePos.x - dot.getRadius(), mousePos.y - dot.getRadius()});

    dots.push_back(dot);
  }
}

void Application::Render() {
  window_.clear(sf::Color::Black);
  for (const auto& dot : dots) {
    window_.draw(dot);
  }
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
