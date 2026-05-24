# Centipede Arcade Game Clone

This is my first ever university programming project, developed as a C++ clone of the classic **Centipede** arcade game. The project was built using **SFML** and includes player movement, shooting mechanics, mushroom obstacles, centipede movement, collision detection, scoring, sound effects, and background music.

## About the Project

The goal of this project was to recreate the core gameplay of the classic Centipede arcade game while learning the fundamentals of game development in C++. Since this was my first university project, it focuses on basic but important concepts such as arrays, functions, sprites, collision detection, keyboard input, game loops, and simple game state management.

The player controls a shooter at the bottom of the screen and must destroy the centipede while avoiding collisions. Mushrooms act as obstacles and affect the movement of the centipede. Shooting different game objects updates the score, and the game ends when the player collides with the centipede or a poisonous mushroom.

## Features

- Classic Centipede-inspired arcade gameplay
- Built with C++ and SFML
- Player movement using keyboard controls
- Bullet shooting mechanic
- Centipede made up of multiple moving body segments
- Centipede head and body collision detection
- Random mushroom generation
- Mushroom damage system
- Poisonous mushrooms created during gameplay
- Score system
- Background image
- Background music
- Sound effects for shooting and gameplay actions
- Extra centipede head generation during the game
- Game-over conditions based on collisions

## Technologies Used

- C++
- SFML Graphics
- SFML Audio
- SFML Window
- SFML System

## Project Structure

```text
i230604_Project_SectionD/
|
|-- Centipede_i230604.cpp
|-- Steps To Compile.txt
|
|-- Music/
|   |-- field_of_hopes.ogg
|
|-- Sound Effects/
|   |-- 1up.wav
|   |-- bonus.wav
|   |-- death.wav
|   |-- fire1.wav
|   |-- flea.wav
|   |-- kill.wav
|   |-- newBeat.wav
|   |-- scorpion.wav
|   |-- spider.wav
|
|-- Textures/
|   |-- background.png
|   |-- bullet.png
|   |-- c_body_left_walk.png
|   |-- c_head_left_walk.png
|   |-- death.png
|   |-- explosion.png
|   |-- flea.png
|   |-- fontcenti.ttf
|   |-- mushroom.png
|   |-- player.png
|   |-- right_head.png
|   |-- scorpion.png
|   |-- spider_and_score.png
|
|-- .vscode/
    |-- c_cpp_properties.json
```

## How to Run the Project

### 1. Install the Required Compiler

On Ubuntu or Linux-based systems, install the GNU C++ compiler:

```bash
sudo apt-get install g++
```

### 2. Install SFML

Install the SFML development libraries:

```bash
sudo apt-get install libsfml-dev
```

### 3. Compile the Project

Make sure you are inside the project folder where `Centipede_i230604.cpp` is located.

Compile the source file:

```bash
g++ -c Centipede_i230604.cpp
```

Link the object file with SFML:

```bash
g++ Centipede_i230604.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
```

### 4. Run the Game

```bash
./sfml-app
```

## Controls

| Key | Action |
|---|---|
| Up Arrow | Move player up |
| Down Arrow | Move player down |
| Left Arrow | Move player left |
| Right Arrow | Move player right |
| X | Shoot bullet |

## Gameplay Overview

The player starts near the bottom of the screen and can move within a limited player area. The centipede moves horizontally across the screen and drops down when it hits the edge of the screen or collides with mushrooms.

The player shoots bullets upward to destroy mushrooms and centipede segments. When a centipede segment is hit, the score increases and mushrooms may be generated at the hit position. Some mushrooms can become poisonous, creating additional danger for the player.

The game ends if the player collides with the centipede or a poisonous mushroom.

## Scoring System

| Action | Score |
|---|---|
| Destroy mushroom | +1 |
| Hit centipede body segment | +10 |
| Hit centipede head | +20 |

## Important Game Mechanics

### Player

The player is controlled using the arrow keys. Movement is restricted to the lower part of the screen, similar to the original Centipede arcade game.

### Bullet

The player can fire one bullet at a time using the `X` key. The bullet travels upward and disappears when it leaves the screen or hits an object.

### Centipede

The centipede is made up of multiple body segments and one head. It moves horizontally and changes direction when reaching the screen boundary or colliding with mushrooms.

### Mushrooms

Mushrooms are randomly placed across the play area. They can block and redirect the centipede. Mushrooms require multiple hits to be destroyed.

### Poisonous Mushrooms

Poisonous mushrooms are generated during gameplay and can cause the game to end if the player collides with them.

### Extra Centipede Heads

Additional centipede heads can be generated after a certain amount of time, increasing the difficulty of the game.

## What I Learned

This project helped me understand the basics of game development and C++ programming, including:

- Creating a game window using SFML
- Loading and displaying sprites
- Handling keyboard input
- Playing background music and sound effects
- Using arrays to store game object data
- Implementing a real-time game loop
- Detecting collisions between objects
- Updating player, bullet, mushroom, and centipede states
- Managing score and game-over conditions
- Structuring a medium-sized C++ program using functions

## Requirements

To run this project, you need:

- A C++ compiler such as `g++`
- SFML installed on your system
- Linux or any environment properly configured for SFML
- All texture, music, and sound effect folders kept in the same directory structure as the source file

## Notes

This was my first university programming project, so the code reflects my early learning stage. The project focuses more on implementing core gameplay logic and understanding programming fundamentals rather than advanced software architecture.

Future improvements could include:

- Main menu screen
- Game-over screen
- Lives system
- Multiple levels
- High score saving
- More enemy types
- Better animation handling
- Cleaner object-oriented structure
- Improved collision system

## Author

Developed by Faisal Safdar as a university programming project.

## License

This project is for educational purposes.
