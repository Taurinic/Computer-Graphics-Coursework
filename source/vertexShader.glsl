#version 330 core

// Inputs
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal; // Normal input

// Outputs to fragment shader
out vec2 UV;
out vec3 FragPos; // World-space position
out vec3 Normal;  // Transformed normal

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Calculate vertex world position
    vec4 worldPos = model * vec4(position, 1.0);
    FragPos = worldPos.xyz;

    // Transform the normal
    Normal = mat3(transpose(inverse(model))) * normal;

    // Output texture UV
    UV = uv;

    // Output final clip space position
    gl_Position = projection * view * worldPos;
}
