#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3 fnormal;
uniform bool LuzEscenaOn;
uniform vec4 luzBool;

in vec3 fvertex;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);
const vec3 posFocus = vec3(5, 10, 5);
const vec3 colorFocus = vec3(0.4, 0.4, 0.4);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;
uniform mat4 TGbola;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
    //Calculo la posición del vertex en SCO y la envío al vertex shader
    vec4 vertexSCO = View * TG * vec4(fvertex, 1.0);
    //Calculo y envío la normal en SCO y normalizada
    mat3 NormalMatrix = inverse (transpose (mat3 (View * TG)));
    vec3 NormSCO = normalize(NormalMatrix*fnormal);

//Calculo del foco de escena
    vec4 posFocusFS = View * vec4(posFocus, 1);
	vec3 LSCO = normalize(posFocusFS.xyz - vertexSCO.xyz);
	vec3 NormSCOFS = normalize(NormSCO);

	 vec3 fcolor = Ambient();
     if (LuzEscenaOn) {
       fcolor += Difus(NormSCOFS, LSCO, colorFocus) + Especular(NormSCOFS,LSCO,vertexSCO.xyz,colorFocus);
    }

     vec3 posFocos[4];
     posFocos[0] = vec3(1.2, 0.0, 0.0);
     posFocos[1] = vec3(-1.2, 0.0, 0.0);
     posFocos[2] = vec3(0.0, 0.0, 1.2);
     posFocos[3] = vec3(0.0, 0.0, -1.2);

     vec3 colFocos[4];
     colFocos[0] = vec3(0.4, 0.0, 0.0);
     colFocos[1] = vec3(0, 0.4, 0.0);
     colFocos[2] = vec3(0, 0, 0.4);
     colFocos[3] = vec3(0.4, 0.4, 0);
     vec4 posF;

     for (int i = 0; i < 4; ++i) {
      if (luzBool[i] == 1) {
          posF =   View* TGbola * vec4(posFocos[i],1.0);
          vec3 L = normalize(posF.xyz - vertexSCO.xyz);


          fcolor += Difus(NormSCOFS, L, colFocos[i]) + Especular(NormSCOFS,L,vertexSCO.xyz,colFocos[i]);
      }

    }




	FragColor = vec4(fcolor,1);
}
