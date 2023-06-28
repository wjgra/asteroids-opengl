# asteroids-opengl

[DESCRIPTION]

N.B. 

This is my first OpenGL project, and my first C++ project with 10+ files.

If I were to start again, I wouldn't have Ship etc. inheriting from GameObject, but would use a renderer like I did for the text - in retrospect this is more flexible, and would make batching draw calls possible.

The simulation objects could also be members of GameState rather than (unique) pointers, but I wanted to play with smart pointers in this instance.
