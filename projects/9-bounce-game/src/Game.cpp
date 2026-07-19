#include "Game.h"
#include "GameConfig.h"

Game::Game() : window_(sf::VideoMode(sf::Vector2u(gConfig.windowSize)), "Bounce Game"), paddle_() {
  //
  window_.setFramerateLimit(60);
}

bool Game::IsRunning() const { return window_.isOpen(); }

void Game::ProcessEvents() {
  while (const auto event = window_.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window_.close();
    }
  }
}

void Game::Update() {
  float dt = clock_.restart().asSeconds();

  if (ballSpawnClock_.getElapsedTime().asSeconds() > gConfig.ballSpawnCooldown) {
    balls_.emplace_back();
    ballSpawnClock_.restart();
  }

  paddle_.Move(dt);

  for (Ball& ball : balls_) {
    ball.Move(dt);
  }

  HandleCollisions();

  if (stats_.GetLives() <= 0) {
    Restart();
  }
}

void Game::Render() {
  window_.clear(sf::Color::Black);

  background_.Draw(window_);

  paddle_.Draw(window_);

  for (const Ball& ball : balls_) {
    ball.Draw(window_);
  }

  stats_.Draw(window_);

  window_.display();
}

void Game::HandleCollisions() {
  for (Ball& ball : balls_) {
    bool isColliding =
        ball.GetGlobalBounds().findIntersection(paddle_.GetGlobalBounds()).has_value();
    if (isColliding && ball.GetDirection().y > 0) {
      ball.Bounce();
      sounds_.PlayBounceSound();

      stats_.SetScore(stats_.GetScore() + 1);
    }
  }

  int ballsMissed =
      static_cast<int>(std::erase_if(balls_, [](const Ball& ball) { return ball.IsOutside(); }));
  if (ballsMissed > 0) {
    stats_.SetLives(stats_.GetLives() - ballsMissed);
  }
}

void Game::Restart() {
  balls_.clear();
  ballSpawnClock_.restart();
  sounds_.Restart();
  paddle_.Restart();

  stats_.Reset();
}
