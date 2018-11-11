#version 120
//  Per Pixel Lighting shader

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;

void main()
{
  //  Vertex location in modelview coordinates
  vec4 P = gl_ModelViewMatrix * gl_Vertex;
  //  Light position
  Light  = gl_LightSource[0].position.xyz - P.xyz;
  //  Normal
  Normal = gl_NormalMatrix * gl_Normal;
  //  Eye position
  View  = -P.xyz;
  //  Set vertex position
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
