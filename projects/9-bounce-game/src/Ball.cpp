#include "Ball.h"
#include "GameConfig.h"
#include "Random.h"

Ball::Ball() : shape_(gConfig.ballRadius), speed_(gConfig.ballSpeed) {
  shape_.setFillColor(sf::Color::Cyan);
  shape_.setOrigin(shape_.getGeometricCenter());
  shape_.setPosition({gConfig.windowSize.x * 0.50f, gConfig.windowSize.y * 0.25f});

  direction_ = {1, sf::degrees(gRandom.GetFloat(0, 360))};
}

void Ball::Move(float dt) {
  sf::Vector2f lastPos = shape_.getPosition();

  shape_.move(direction_ * speed_ * dt);

  if (shape_.getPosition().x - shape_.getRadius() <= 0 ||
      shape_.getPosition().x + shape_.getRadius() >= gConfig.windowSize.x) {
    direction_.x *= -1;
    shape_.setPosition(lastPos);
  }

  if (shape_.getPosition().y - shape_.getRadius() <= 0) {
    direction_.y *= -1;
    shape_.setPosition(lastPos);
  }
}

void Ball::Draw(sf::RenderWindow& window) const { window.draw(shape_); }

void Ball::Bounce() { direction_.y *= -1; }

sf::FloatRect Ball::GetGlobalBounds() const { return shape_.getGlobalBounds(); }

sf::Vector2f Ball::GetDirection() const { return direction_; }

bool Ball::IsOutside() const {
  return shape_.getPosition().y + shape_.getRadius() >= gConfig.windowSize.y;
}
