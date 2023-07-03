#version 300 es
in vec2 position;

uniform mat4 model;
uniform mat4 projection;

uniform vec2 offsets[9];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = projection * (model * vec4(position, 0.0f, 1.0f) + vec4(offset, 0.0f, 0.0f));
}