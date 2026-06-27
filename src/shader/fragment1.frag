#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
// in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
// uniform sampler2D texture0;
// uniform vec4 colDiffuse;

// uniform vec3 lightPosition;
uniform vec3 cameraPosition;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec3 ambientStrength = vec3(0.7f);
    vec3 lightColor = vec3(1.0f);
    vec3 toyColor = vec3(0.2f, 0.9f, 0.05f);
    vec3 fragNormal = normalize(fragNormal);
    // vec3 lightDir = vec3(lightPosition - fragPosition);


    // float diffuseValue = max(dot(fragNormal, lightDir), 0);
    // vec3 diffuse = diffuseValue*lightColor;

    float distanceToCamera = length(cameraPosition - fragPosition);
    float depthInfluence = clamp(1.0 / (1.0 + 0.05 * distanceToCamera), 0.2, 1.0);

    vec3 color = (ambientStrength /* + diffuse */)*toyColor*depthInfluence;

    finalColor = vec4(color, 1.0);
    // Gamma correction
    // finalColor = pow(finalColor, vec4(1.0/2.2));
}
