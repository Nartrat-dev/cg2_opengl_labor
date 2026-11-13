#version 410  
in vec3 colorForFragmentShader;  
uniform float ambientFactor;
out vec4 colorOfMyChoice;


struct LightParaemeters {
    float ambientFactor;
    float diffuseFactor;
    float specFactor;
    int shininess;
};

//uniform LightParameters params;


void main()  
{  
    colorOfMyChoice = vec4(ambientFactor*colorForFragmentShader,1.0);  
};
 