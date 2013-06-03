#version 150

in  vec4 vPosition;
in  vec2 vTexCoords;
out vec2 st;

//uniform mat4 scale;
//uniform mat4 trans;

uniform mat4 ModelView;
uniform mat4 Projection;



void main() 
{
	st = vTexCoords;

	//mat4 mv = scale*trans*ModelView;
	gl_Position = ModelView*Projection*vPosition;
} 