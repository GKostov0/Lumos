#version 330

in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragmentPosition;

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

struct Material
{
    float shininess;
    float specularIntensity;
};

uniform sampler2D ourTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

void main()
{
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0f);

    if (diffuseFactor > 0.0f)
    {
        vec3 fragmentToEye = normalize(eyePosition - FragmentPosition);
        vec3 lightReflect = normalize(reflect(directionalLight.direction, normalize(Normal)));
        float specularFactor = dot(fragmentToEye, lightReflect);
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    color = texture(ourTexture, TexCoord) * (ambientColor + diffuseColor + specularColor);
}