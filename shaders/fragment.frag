#version 330 core
out vec4 outputColour;

uniform vec4 inputColour;

void main()
{
    outputColour = inputColour;
}