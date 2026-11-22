#version 410

struct LightParameters {
    float ambientFactor;
    float diffuseFactor;
    float specFactor;
    float shininess;
};

in vec3 colorForFragmentShader;
in vec3 normalVectorForFragmentShader;
in vec3 fragPos;

uniform LightParameters params;

// Position der Lichtquelle
uniform vec3 lightPos;

// Position der Kamera
const vec3 cameraPos = vec3(0.0, 0.0, 1.0);

out vec4 colorOfMyChoice;

void main()
{
    // Normalenvektor normalisieren
    vec3 normalDir = normalize(normalVectorForFragmentShader);

    // Richtung zur Lichtquelle berechnen
    vec3 lightDir = normalize(lightPos - fragPos);

    // Richtung zur Kamera berechnen
    vec3 viewDir = normalize(cameraPos - fragPos);

    // Reflektierten Lichtstrahl berechnen
    vec3 reflectDir = reflect(-lightDir, normalDir);

    // Beleuchtung
    float ambient = params.ambientFactor;                                                               // Ambiente Lichtkomponente
    float diffuse = max(dot(normalDir, lightDir), 0.0) * params.diffuseFactor;                          // Diffuse Lichtkomponente
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), params.shininess) * params.specFactor;     // Spiegelnde Lichtkomponente
    
    float lighting = ambient + diffuse + specular;      // Gesamte Beleuchtung 

    // Farbe
    colorOfMyChoice = vec4(lighting * colorForFragmentShader, 1.0);
}
