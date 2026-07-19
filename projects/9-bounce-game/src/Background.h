#pragma once

#include <SFML/Graphics.hpp>

class Background {
private:
  sf::Texture texture_;
  sf::Sprite sprite_;

public:
  Background();

  void Draw(sf::RenderWindow& window) const;
};
