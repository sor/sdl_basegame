# sdl_basegame

A skeleton project to start your SDL2 journey

- Includes SDL + libraries:
  - Image - Loading of common image types
  - TTF - Loading and rendering of TrueType and OpenType fonts
  - Mixer - Loading and mixing of various audio formats
  - Net - Network communication 
- As well as those other helpful libraries
  - [{fmt}](https://github.com/fmtlib/fmt) - Implementation of C++20's std::format, available now 
  - [Dear ImGUI](https://github.com/ocornut/imgui) - Simple immediate mode user interface
  - [NativeFileDialog](https://github.com/mlabbe/nativefiledialog) - File dialogs that work under Linux, Win and Mac

---

# Classes

- **Game**: The base class for your game
    - Runs from start to the end of execution and calls HandleEvents(), Input(), Update(), and Render() on the current GameState
    - Has a prioritzied HandleEvent method that is processed before the GameState events
    - Has two ways of showing msec/frame (which is the same as 1000/FPS, so a higher number means its running slower)
- **GameState**: The base class for your different game states
    - Can dynamically load and unload its assets etc. on activation/deactivation with Init() and UnInit()

Either Game and GameState can initiate a state switch by calling SetNextState( number ) or game.SetNextState( number ), the change will happen after the frame has finished processing.

# Classdiagram

Classdiagram outlining base classes and their relations
![Classdiagram](doc/classdiagram.png)
