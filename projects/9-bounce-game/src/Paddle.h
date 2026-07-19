#pragma once

#include <SFML/Graphics.hpp>

class Paddle {
private:
  sf::RectangleShape shape_;
  float speed_;

public:
  Paddle();

  void Move(float dt);
  void Draw(sf::RenderWindow& window) const;
  sf::FloatRect GetGlobalBounds() const;
  void Restart();
};
