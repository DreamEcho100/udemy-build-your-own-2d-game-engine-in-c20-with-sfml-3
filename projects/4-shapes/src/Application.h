#pragma once

#include <SFML/Graphics.hpp>

class Application {
private:
  sf::RenderWindow window_;

  sf::RectangleShape rect_;
  sf::CircleShape circle_;
  sf::ConvexShape polygon_;

public:
  Application();

  bool IsRunning() const;

  void ProcessEvents();
  void Update();
  void Render();

private:
  void HandleEvent(const sf::Event::Closed&);
  void HandleEvent(const sf::Event::Resized& event);
  void HandleEvent(const sf::Event::FocusLost&);
  void HandleEvent(const sf::Event::FocusGained&);
  void HandleEvent(const sf::Event::KeyPressed& event);
  void HandleEvent(const sf::Event::MouseWheelScrolled& event);
  void HandleEvent(const sf::Event::MouseButtonPressed& event);
  void HandleEvent(const auto&);
};
