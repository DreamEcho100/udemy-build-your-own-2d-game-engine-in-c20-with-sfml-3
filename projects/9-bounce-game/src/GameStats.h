#pragma once

#include <SFML/Graphics.hpp>

class GameStats {
private:
  sf::Font font_;

  sf::Text scoreText_;
  sf::Text livesText_;
  sf::Text highScoreText_;

  int score_;
  int lives_;
  int highScore_;

public:
  GameStats();

  void Draw(sf::RenderWindow& window) const;

  void SetScore(int score);
  void SetLives(int lives);
  void SetHighScore(int highScore);

  int GetScore() const;
  int GetLives() const;
  int GetHighScore() const;

  void Reset();
};
