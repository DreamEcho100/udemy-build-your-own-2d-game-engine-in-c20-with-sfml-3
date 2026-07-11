// Write a program that uses window.waitEvent() instead of pollEvent(). Print a message for every
// event you receive (for example: “Resized”, “KeyPressed”, “MouseWheelScrolled”, etc.). The program
// should wait for interaction instead of running a continuous loop, and it must still allow the
// window to close properly when requested.

// Open the system task manager or use a terminal command to check the CPU usage of your program
// while it runs with waitEvent(). Then switch back to pollEvent() and observe the difference.
// Explain why one approach may use more CPU than the other.

// - When using pollEvent(), the program continues running the game loop every frame, even if no
// events are happening. This means the CPU is constantly working, which results in higher CPU
// usage.

// - When using waitEvent(), the program pauses and waits for an event before continuing. While
// waiting, the CPU does almost no work, so the usage is much lower.

// In short:

// pollEvent() keeps the application fully active and responsive for real-time rendering and
// animation, but uses more CPU.

// waitEvent() saves CPU by sleeping while nothing happens, but it stops rendering and updating,
// making it unsuitable for real-time gameplay.

#include <SFML/Graphics.hpp>
#include <iostream>

void HandleEvent(const sf::Event::Resized& event) {
  std::cout << "Size:" << event.size.x << "x" << event.size.y << "\n";
}

void HandleEvent(const sf::Event::FocusLost&) { std::cout << "Window lost focus!\n"; }
void HandleEvent(const sf::Event::FocusGained&) { std::cout << "Window gained focus!\n"; }

void HandleEvent(const sf::Event::KeyPressed& event) {
  if (event.code == sf::Keyboard::Key::W) {
    std::cout << "\"W\" pressed\n";
  }
}

void HandleEvent(const sf::Event::MouseWheelScrolled& event) {
  std::cout << "Scrolled by: " << event.delta << "\n";
}

void HandleEvent(const sf::Event::MouseButtonPressed& event) {
  std::cout << "Mouse button pressed at";
  std::cout << event.position.x << ", " << event.position.y << "\n";
}

void HandleEvent(const auto&) {}

int main() {
  sf::RenderWindow window(sf::VideoMode({600, 800}), "SFML Window");

  while (window.isOpen()) {
    while (const std::optional event = window.waitEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }

      event->visit([](const auto& type) { HandleEvent(&type); });
    }

    window.clear();
    window.display();
  }
}
