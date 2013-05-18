#version 150

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
out vec4 fColor;

out vec3 fN; //normal at current position
out vec3 fV; //vector from point to viewer
out vec3 fL; //vector from point to light

uniform mat4 uProj;
uniform mat4 uModelView;
uniform mat4 uModel;
uniform vec4 uColor;
uniform float uShininess;
uniform vec4 uSpecularColor;

uniform vec4 uCameraPosition;
uniform vec4 uLightPosition;
uniform int uShadingType;

uniform int uEnableTexture;
uniform mat3 uTextureTrans;

void main() 
{
    gl_Position = uProj * uModelView * vPosition;
	if (uShadingType == 0 ) {
		fN = fV = fL = vec3(0.0, 0.0, 0.0);
		fColor = uColor;
	}
	else {
		fN = (uModel * vec4(vNormal.x, vNormal.y, vNormal.z, 0.0)).xyz;
		fV = (uCameraPosition - uModel * vPosition).xyz;
		fL = (uLightPosition - uModel * vPosition).xyz;

		if (uShadingType == 3) {	// No Shading || Phong Shading
			fColor = uColor;
		} else if (uShadingType == 1 || uShadingType == 2) {	// Flat Shading || Gouraud Shading
			vec3 N, V, L, H;

			N = normalize(fN);
			V = normalize(fV);
			L = normalize(fL);
        
			H = normalize(L + V);

			vec4 ambient = 0.2 * uColor;
			vec4 diffuse = max(dot(L, N), 0.0) * 0.5 * uColor;
			vec4 specular = pow(max(dot(N, H), 0.0), uShininess) * 0.5 * uSpecularColor;

			if (dot(L, N) < 0.0) {
				specular = vec4(0.0, 0.0, 0.0, 1.0);
			}

			fColor = ambient + diffuse + specular;
			fColor.a = 1.0;
		}
	}
	if (uEnableTexture == 1) {
		gl_TexCoord[0].xy = (uTextureTrans * vec3(vTexCoords.x, vTexCoords.y, 1.0)).xy;
	}
}
