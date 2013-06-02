#version 150

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
out float fogFactor;
out vec4 fColor;
out vec2 texCoord[2];

out vec3 fN; //normal at current position
out vec3 fV; //vector from point to viewer
out vec3 fL[5]; //vector from point to light

uniform mat4 uProj;
uniform mat4 uModelView;
uniform mat4 uModel;
uniform float uShininess;

uniform int uNumLights;
uniform vec4 uAmbientProduct[5];
uniform vec4 uDiffuseProduct[5];
uniform vec4 uSpecularProduct[5];
uniform vec4 uLightPosition[5];

uniform vec4 uCameraPosition;
uniform int uShadingType;

uniform int uEnableTexture;
uniform mat3 uTextureTrans;

uniform float uFogMaxDist;
uniform float uFogMinDist;

float computeLinearFogFactor()
{
	float factor;
	factor = (uFogMaxDist - length(gl_Position.xyz)) / (uFogMaxDist - uFogMinDist );
	factor = clamp( factor, 0.0, 1.0 );
	return factor;
}

void main() 
{
    gl_Position = uProj * uModelView * vPosition;
	fogFactor = computeLinearFogFactor();
	fColor = vec4(0.0, 0.0, 0.0, 1.0);

	// Phong shading is taken care of in the fragment shader
	if (uShadingType == 0 ) { // No shading
		fN = fV = vec3(0.0, 0.0, 0.0);
		fL[0] = vec3(0.0, 0.0, 0.0);
		fColor = uAmbientProduct[0];
	}
	else if (uShadingType == 1 || uShadingType == 2) {	// Flat Shading || Gouraud Shading
		fN = (uModel * vec4(vNormal.x, vNormal.y, vNormal.z, 0.0)).xyz;
		fV = (uCameraPosition - uModel * vPosition).xyz;

		vec3 N, V, L, H;
		N = normalize(fN);
		V = normalize(fV);

		for (int i = 0; i < uNumLights; i++) {
			fL[i] = (uLightPosition[i] - uModel * vPosition).xyz;
			float distance = 0.0000005 * (pow(fL[i].x, 2) + pow(fL[i].y, 2) + pow(fL[i].z, 2));
			if (distance < 1.0) {
				distance = 1.0;
			}

			L = normalize(fL[i]);
			H = normalize(L + V);

			vec4 ambient = uAmbientProduct[i] / distance;
			vec4 diffuse = max(dot(L, N), 0.0) * uDiffuseProduct[i] / distance;
			vec4 specular = pow(max(dot(N, H), 0.0), uShininess) * uSpecularProduct[i] / distance;

			if (dot(L, N) < 0.0) {
				specular = vec4(0.0, 0.0, 0.0, 1.0);
			}

			fColor += ambient + diffuse + specular;
		}
		fColor.a = 1.0;
	}
	if (uEnableTexture == 1) {
		texCoord[0].xy = vTexCoords.xy;
	}	
}
