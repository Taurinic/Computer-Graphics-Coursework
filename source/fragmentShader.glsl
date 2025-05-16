#version 330 core

// Inputs from vertex shader
in vec2 UV;
in vec3 FragPos; // World-space position of the fragment
in vec3 Normal;  // Normal at the fragment

// Outputs
out vec3 colour;

// Uniforms
uniform sampler2D textureMap;
uniform vec3 viewPos; // Camera position

// Point light struct
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLight;

void main()
{
    // Sample base color from texture
    vec3 baseColor = texture(textureMap, UV).rgb;

    // Lighting calculations
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float distance = length(pointLight.position - FragPos);

    // Ambient
    vec3 ambient = pointLight.ambient * baseColor;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * diff * baseColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // 32 = shininess
    vec3 specular = pointLight.specular * spec;

    // Attenuation
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // Final color
    colour = ambient + diffuse + specular;
}
