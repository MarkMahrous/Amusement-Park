# Amusement Park Adventure - 3D Game

Welcome to the Amusement Park Adventure, a 3D game where you explore an exciting amusement park environment filled with various objects, tasks, and challenges!

## Overview

In this game, you take on the role of a character exploring an amusement park filled with attractions, rides, and collectible items. Your objective is to accomplish a specific goal within a set time frame by navigating through the park and collecting designated items.

## Features

### Game Environment

The amusement park environment consists of:
- Boundary walls surrounding the scene on three sides.
- A ground structure.
- Various amusement park objects including trees, lamp posts, cars, kiosks, a roller coaster, swings, a seesaw, a water fountain, gardens, benches, trash bins, and an information desk.
- A character representing the player.

### Modeling

- Player model includes a human-like representation with head, body, and limbs using a minimum of six primitives.
- Boundary walls consist of at least two primitives for each side.
- The ground is modeled using a minimum of one primitive.
- Other scene objects include two major objects with a minimum of five primitives each, along with three additional objects containing a minimum of three primitives each.
- The goal/collectible object(s) consist of at least three primitives.
- All models are designed to appear as realistic as possible and are colored accordingly.

### Collisions

- The player moves within the fenced boundaries and cannot go beyond the ground borders.
- Collision occurs when the player interacts with the collectible goal(s), causing them to disappear upon collection.

### Camera

- The camera offers free movement along the three axes.
- Three different views are available:
    1. Top View
    2. Side View
    3. Front View

### Animations and Controls

- Player movement is controlled using the keyboard, allowing movement in any direction.
- Player rotation aligns with the direction of motion, facilitated by the mouse/keyboard.
- The camera moves through the scene using mouse or keyboard controls.
- Scene object animations are triggered by specific key presses, including translation, rotation, or scaling.
- Goal object(s) animate in place throughout the game.
- Bounding wall colors change periodically.

### Game End

- The game concludes when the time runs out.
- A 'GAME WIN' screen appears if the player collects the goal(s) within the specified time.
- A 'GAME LOSE' screen appears if the goal is not collected/reached within the time limit.

## Implementation
Amusement Park Adventure - 3D Game is developed using C++ in Visual Studio and relies on the OpenGL library and the glut.h header for graphics rendering. Players can control the game through keyboard input to move in four directions and interact with the game environment.

## How to Run the Game
Follow these steps to run Amusement Park Adventure - 3D Game:
1. Set up your development environment with Visual Studio.
2. Ensure you have the necessary OpenGL and glut.h libraries installed.
3. Open the Game by running OpenGL3DTemplate.sln with Visual Studio.
4. Compile and run the game code, and you can start playing.
