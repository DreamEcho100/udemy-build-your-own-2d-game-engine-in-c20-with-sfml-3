#include <SFML/Graphics.hpp>

#include "EngineVisitor.h"

class Engine {
private:
  sf::RenderWindow window_;

public:
  Engine();

  bool IsRunning() const;
  void ProccessEvents();
  void Update();
  void Render();

private:
  friend EngineVisitor;

  void HandleEvent(const sf::Event::Closed&);
  void HandleEvent(const auto&);

  void EventWindowClose();
};
