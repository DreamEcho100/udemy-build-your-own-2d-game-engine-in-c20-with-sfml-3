#include <SFML/GpuPreference.hpp>

#include "Engine/Core/Engine.h"

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main() {
  Engine engine = Engine();

  while (engine.IsRunning()) {
    engine.ProccessEvents();
    engine.Update();
    engine.Render();
  }
}
