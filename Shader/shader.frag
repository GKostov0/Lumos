#version 330

in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;

out vec4 color;

struct DirectionalLight
{
    // Ambient
    vec3 color;
    float ambientIntensity;

    // Diffuse
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D ourTexture;
uniform DirectionalLight directionalLight;

void main()
{
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    color = texture(ourTexture, TexCoord) * (ambientColor + diffuseColor);
}