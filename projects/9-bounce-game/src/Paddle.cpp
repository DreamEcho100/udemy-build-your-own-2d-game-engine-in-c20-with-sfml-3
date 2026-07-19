
#include "Paddle.h"

// #include <algorithm>

#include "GameConfig.h"

Paddle::Paddle() : shape_(gConfig.paddleSize), speed_(gConfig.paddleSpeed) {
  shape_.setFillColor(sf::Color::Green);
  shape_.setOrigin(shape_.getGeometricCenter());
  shape_.setPosition({gConfig.windowSize.x * 0.50f, gConfig.windowSize.y * 0.85f});
}

void Paddle::Move(float dt) {

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) &&
      shape_.getPosition().x - shape_.getSize().x * 0.5f > 0) {
    shape_.move({-speed_ * dt, 0});
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
      shape_.getPosition().x + shape_.getSize().x * 0.5f < gConfig.windowSize.x) {
    shape_.move({speed_ * dt, 0});
  }

  // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
  //   shape_.move({-speed_ * dt, 0});
  // }

  // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
  //   shape_.move({speed_ * dt, 0});
  // }

  // float halfWidth = shape_.getSize().x / 2;
  // float x = std::clamp(shape_.getPosition().x, halfWidth, gConfig.windowSize.x - halfWidth);
  // shape_.setPosition({x, shape_.getPosition().y});
}

void Paddle::Draw(sf::RenderWindow& window) const { window.draw(shape_); }

sf::FloatRect Paddle::GetGlobalBounds() const { return shape_.getGlobalBounds(); }

void Paddle::Restart() {
  shape_.setFillColor(sf::Color::Green);
  shape_.setOrigin(shape_.getGeometricCenter());
  shape_.setPosition({gConfig.windowSize.x * 0.50f, gConfig.windowSize.y * 0.85f});
}
