#version 330 core

// Inputs
in vec2 UV;

// Outputs
out vec3 colour;

// Uniforms
uniform sampler2D textureMap;

void main()
{
    colour = vec3(texture(textureMap, UV));
}
