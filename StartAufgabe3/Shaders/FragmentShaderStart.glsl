#version 410  

struct LightParameters {
    float ambientFactor;
    float diffuseFactor;
    float specFactor;
    int shininess;
};

in vec3 colorForFragmentShader;  
in vec3 normalVectorForFragmentShader;
in vec3 fragPos;

uniform float ambientFactor;
uniform LightParameters params;

out vec4 colorOfMyChoice;

vec3 lightPos = vec3(0.0, 0.0, 1.0);        // Position der Lichtquelle -> fest codiert
vec3 cameraPose = vec3(0.0, 0.0, 1.0);      // Position der Kamera -> fest codiert


void main()  
{  
    vec3 lightDir = normalize(lightPos - fragPos);                          // Richtung zwischen Lichtquelle und Fragment
    vec3 viewDir = normalize(cameraPose - fragPos);                         // Richtung zwischen Kamera und Fragment
    vec3 reflectDir = reflect(-lightDir, normalVectorForFragmentShader);    // Reflektionsrichtung des Lichts berechnen

    float ambientFactor = params.ambientFactor;
    float diffuseFactor = (max(dot(normalVectorForFragmentShader, lightDir), 0.0)) * params.diffuseFactor;      // Diffusanteil der Beleuchtung berechnen
    float specFactor = (pow(max(dot(viewDir, reflectDir), 0.0), params.shininess) * params.specFactor);         // Spekularanteil der Beleuchtung berechnen

    colorOfMyChoice = vec4((ambientFactor + diffuseFactor + specFactor) * colorForFragmentShader, 1.0);  
};
 