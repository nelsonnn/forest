#version 120
// Blur (low-pass)
//   1 2 1
// 1 2 3 2 1
// 2 3 1 3 2 / 37
// 1 2 3 2 1
//   1 2 1

uniform float DX;
uniform float DY;
uniform sampler2D img;

vec4 sample(float dx,float dy)
{
  return texture2D(img,gl_TexCoord[0].st+vec2(dx,dy));
}

void main()
{
  float one = 2.0/49.0;
  float two = 2.5/49.0;
  float thr = 3.0/49.0;
  gl_FragColor =         one*sample(-DX,2.0*DY)+ two*sample(0.0,2.0*DY)+ one*sample(+DX,2.0*DY)
+ one*sample(-2.0*DX,+DY)+ two*sample(-DX,+DY) + thr*sample(0.0,+DY) + two*sample(+DX,+DY) + one*sample(2.0*DX,+DY)
+ two*sample(-2.0*DX,0.0)+ thr*sample(-DX,0.0) + one*sample(0.0,0.0) + thr*sample(+DX,0.0) + two*sample(2.0*DX,0.0)
+ one*sample(-2.0*DX,-DY)+ two*sample(-DX,-DY) + thr*sample(0.0,-DY) + two*sample(+DX,-DY) + one*sample(2.0*DX,-DY)
                       + one*sample(-DX,-2.0*DY)+two*sample(0.0,-2.0*DY)+one*sample(+DX,-2.0*DY);
  //gl_FragColor = min(thiscolor, vec4(0.10,0.10,0.10,1.0));
}
