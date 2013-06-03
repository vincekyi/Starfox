#version 150

in  vec2  st;
uniform sampler2D texMap;
uniform vec4 vColor;
out vec4  fColor;

void main() 
{ 
    fColor = vColor*texture2D(texMap, st).bgra;
} 
