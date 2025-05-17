#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec3 fnormal;


out vec3 fvertex;


void main()
{
    fnormal = normal;
    fvertex = vertex;

    vec4 vertexSCO = View * TG * vec4(vertex, 1.0);

//Envio los parametros
    fmatamb = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;

    gl_Position = Proj * vertexSCO;
}
