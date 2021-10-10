![Game of Life](https://raw.githubusercontent.com/kurtjd/game-of-life/master/gameoflife.png)

Game of Life
============
A basic implementation of John Conway's Game of Life, written in C++. I made this just to practice using the SFML library and because I have a fascination with artificial life.

Controls
========
* Click a cell to activate it and click again to deactivate it.
* Press the enter key to run or pause the simulation.
* Use the up and down arrow keys to change the speed of the simulation.

Requirements
============
* SFML

Build Procedures
================
```g++ life.cpp -lsfml-graphics -lsfml-window -lsfml-system -o life```

Run
===
```./life```
