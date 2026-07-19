#include "Background.h"

Background::Background() : texture_("assets/Background.png"), sprite_(texture_) {}

void Background::Draw(sf::RenderWindow& window) const { window.draw(sprite_); }
