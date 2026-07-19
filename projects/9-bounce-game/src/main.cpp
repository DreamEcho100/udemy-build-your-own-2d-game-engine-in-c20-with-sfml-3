#include "Game.h"

int main() {
  Game app;

  while (app.IsRunning()) {
    app.ProcessEvents();
    app.Update();
    app.Render();
  }
}
