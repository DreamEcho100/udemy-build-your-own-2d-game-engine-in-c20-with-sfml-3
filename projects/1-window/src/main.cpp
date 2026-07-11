// Write a program that asks the user in the terminal whether they want fullscreen or windowed mode.
// If they choose windowed, create a 1024 by 768 window titled "Windowed Mode". If they choose
// fullscreen, create a fullscreen window using the desktop resolution and title it "Fullscreen
// Mode". In both cases, keep the window responsive, handle the Closed event, and use clear and
// display inside the game loop.

#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
  sf::RenderWindow window;

  char choice;
  std::cout << "Fullscreen? (y/n)";
  std::cin >> choice;

  if (choice == 'y' || choice == 'Y') {
    window.create(sf::VideoMode::getDesktopMode(), "Fullscreen Mode", sf::State::Fullscreen);
  } else {
    window.create(sf::VideoMode({1024, 768}), "Windowed Mode");
  }

  while (window.isOpen()) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear();
    window.display();
  }
}
