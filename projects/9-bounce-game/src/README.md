# Requirements

Write a program that creates an **800 × 600** window. Inside it, create a rectangle sized **200 × 100**, positioned at **(100, 300)**, and center its origin using `getGeometricCenter()`.
Use an sf::Clock to calculate delta time each frame. Print that delta time in the terminal.

Apply the following transformations every frame using delta time:

- **Move speed**: 50 pixels per second to the right
- **Rotation speed**: 30 degrees per second clockwise
- **Scale speed**: shrink by 0.1 pixels per second on both axes

Run the program and make sure the rectangle keeps moving, rotating, and shrinking smoothly while the window remains fully responsive.

