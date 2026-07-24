#include "EngineVisitor.h"
#include "Engine.h"

void EngineVisitor::operator()(const sf::Event::Closed&) { engine.EventWindowClose(); }
void EngineVisitor::operator()(const sf::Event::Resized& event) {
  engine.EventWindowResized(event);
}
