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
    gl_Position = matrix * vec4(position, 1.0);      

    fragPos = (matrix * vec4(position, 1.0)).xyz;                        // Zugängliche Kopie für den Fragment-Shader
    colorForFragmentShader = color;
    normalVectorForFragmentShader = (matrix * vec4(normal, 1.0)).xyz;   // Normalenvektor transformieren und an den Fragment-Shader weitergeben
};