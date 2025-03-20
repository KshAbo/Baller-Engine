#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
// in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
// uniform sampler2D texture0;
// uniform vec4 colDiffuse;

uniform vec3 lightPosition;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec3 ambientStrength = vec3(0.1f);
    vec3 lightColor = vec3(1.0f);
    vec3 toyColor = vec3(1.0f, 0.5f, 0.31f);
    vec3 fragNormal = normalize(fragNormal);
    vec3 lightDir = vec3(lightPosition - fragPosition);


    float diffuseValue = max(dot(fragNormal, lightDir), 0);
    vec3 diffuse = diffuseValue*lightColor;


    vec3 color = (ambientStrength + diffuse)*toyColor;

    finalColor = vec4(color, 1.0);
    // Gamma correction
    // finalColor = pow(finalColor, vec4(1.0/2.2));
}
