#include <fstream>

#include "GameConfig.h"
#include "GameStats.h"

GameStats::GameStats()
    : font_("assets/Montserrat.ttf"), scoreText_(font_), highScoreText_(font_), livesText_(font_) {
  scoreText_.setFillColor(sf::Color::Cyan);
  scoreText_.setPosition({10, 10});
  SetScore(0);

  livesText_.setFillColor(sf::Color::Red);
  livesText_.setPosition({10, 110});
  SetLives(gConfig.lives);

  highScoreText_.setFillColor(sf::Color::Yellow);
  highScoreText_.setPosition({10, 60});

  if (std::ifstream file{"HighScore.txt"}) {
    file >> highScore_;
  } else {
    highScore_ = 0;
  }

  SetHighScore(highScore_);
}

void GameStats::Draw(sf::RenderWindow& window) const {
  window.draw(scoreText_);
  window.draw(highScoreText_);
  window.draw(livesText_);
}

void GameStats::SetScore(int score) {
  score_ = score;
  scoreText_.setString("Score: " + std::to_string(score_));
}
void GameStats::SetLives(int lives) {
  lives_ = lives;
  livesText_.setString("Lives: " + std::to_string(lives_));
}
void GameStats::SetHighScore(int highScore) {
  highScore_ = highScore;
  highScoreText_.setString("High: " + std::to_string(highScore_));
}

int GameStats::GetScore() const { return score_; }
int GameStats::GetLives() const { return lives_; }
int GameStats::GetHighScore() const { return highScore_; }

void GameStats::Reset() {
  if (score_ > highScore_) {
    std::ofstream file("HighScore.txt");
    if (file) {
      file << score_;
    }

    SetHighScore(score_);
  }

  SetLives(gConfig.lives);
  SetScore(0);
}
