#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

out vec4 FragColor;

uniform mat4 TGfocus1;
uniform mat4 TGfocus2;
uniform mat4 View;

uniform vec3 posFocus;
uniform vec3 posFocusC1;
uniform vec3 posFocusC2;

uniform vec3 colorFocusFS;
uniform vec3 colorFocusC;


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

  //Pasar camara d'escena a SCO
  vec3 fposFocusC1 = (View * TGfocus1 * vec4(posFocusC1, 1.0)).xyz;
  vec3 fposFocusC2 = (View * TGfocus2 * vec4(posFocusC2, 1.0)).xyz;

  // Normalitzar la L
  vec3 LC = normalize(posFocus - fvertex);
  vec3 LC1 = normalize(fposFocusC1 - fvertex);
  vec3 LC2 = normalize(fposFocusC2 - fvertex);

  // Tornar a normalitzar la Normal
  vec3 normalSCOFS = normalize(fnormal);     

  vec3 fcolor = Ambient() + 
             Difus(normalSCOFS, LC, colorFocusFS) +
             Especular(normalSCOFS, LC, fvertex, colorFocusFS) + 
             Difus(normalSCOFS, LC1, colorFocusC) +
             Especular(normalSCOFS, LC1, fvertex, colorFocusC) +
             Difus(normalSCOFS, LC2, colorFocusC) +
             Especular(normalSCOFS, LC2, fvertex, colorFocusC);

	FragColor = vec4(fcolor, 1);	
}
