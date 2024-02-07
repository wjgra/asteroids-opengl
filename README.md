# Classic Arcade Game: Asteroids
## Overview
This is a recreation of the core mechanics of my favourite arcade game: [Atari's Asteroids](https://en.wikipedia.org/wiki/Asteroids_(video_game)), which was released in 1979. The goal is to control your spaceship to avoid colliding with the asteroids floating around the screen, whilst shooting said asteroids to destroy them. The play area is a topological torus, so you can fly off the edges of the screen to appear on the other side. Once you've cleared the asteroids, a new (larger) wave of asteroids appears. If you crash, that's it!

This was a small project that I used to familiarise myself with OpenGL, and a tribute to a game that I have fond memories of playing - it has no connection to Atari. If Atari takes any issue with this being on the Internet, I would gladly take it down or modify it upon request from the relevant person. However, the proliferation of similarly named projects that can be readily found via Google or GitHub search suggests this is unlikely to be of any concern to anyone.

I have also compiled this project to WASM using [Emscripten](https://emscripten.org/), enabling it to be played online [here](https://wjgrace.co.uk/projects/asteroids/asteroids.html).

![Asteroids start screen](https://www.wjgrace.co.uk/images/asteroids_thumbnail.gif)

*The start screen*

This was my first non-trivial OpenGL project and so does not have the best program structure. For instance, it would probably be better to batch draw calls for the different objects rather than doing them one at a time. Whilst performance isn't an issue here (due to the low vertex counts and lack of complicated shaders), this could become relevant in more elaborate programs with large numbers of objects. Either way, it was a fun learning experience.

## Dependencies and Compilation
This project uses SDL for window creation and input handling, and OpenGL for rendering. The native version uses [Glad](https://glad.dav1d.de/) for loading OpenGL API functions.

Sample GCC compilation command:
```
g++ src\*.cpp src\*.c -o asteroids.exe -W -Wall -Wextra -pedantic -I "C:\SDL-release-2.26.4\include" -lopengl32 -lglu32 -pthread "SDL2.dll" -O3 -DNDEBUG
```
Sample Emscripten compilation command:
```
emcc src/*.cpp src/*.c -o "asteroids.html" -W -Wall -Wextra -pedantic -sUSE_SDL=2 --shell-file template.html -I "C:\Users\wjgra\source\repos\emsdk\upstream\emscripten\cache\sysroot\include" --preload-file shaders_web --preload-file lucida_typewriter_font.tga --preload-file lucida_typewriter_font_2x.tga -sALLOW_MEMORY_GROWTH=1 -O3 -DNDEBUG -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2
```

## Controls
Accelerate with the up arrow key (your momentum decays with time, due to all the air in space, obviously...), rotate with the left and right arrow keys, and shoot using the spacebar.

Navigate menus by following the on-screen button prompts.
