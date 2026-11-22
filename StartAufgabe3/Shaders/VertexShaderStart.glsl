#version 410

uniform mat4 matrix;

in vec3 color;
in vec3 position;
in vec3 normal;

out vec3 colorForFragmentShader;
out vec3 normalVectorForFragmentShader;
out vec3 fragPos;

void main()
{
    // hier die Position des Vertexes transformieren
    vec4 transformedPosition = matrix * vec4(position, 1.0);

    gl_Position = transformedPosition;

    // Gebe anschließend Position an den FragmentShader weiter
    fragPos = transformedPosition.xyz;

    // Jetzt wird der normalenvektor transformiert
    normalVectorForFragmentShader =
        (matrix * vec4(normal, 0.0)).xyz;       // w = 0, da es sich um eine Richtung handelt

    // Farbe für den FragmentShader weitergeben
    colorForFragmentShader = color;
}
