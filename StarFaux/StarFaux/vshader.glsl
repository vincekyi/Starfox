#version 150

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
out float fogFactor;
out vec4 fColor;
out vec2 texCoord;

out vec3 fN; //normal at current position
out vec3 fV; //vector from point to viewer
out vec3 fL; //vector from point to light

uniform mat4 uProj;
uniform mat4 uModelView;
uniform mat4 uModel;
uniform float uShininess;
uniform vec4 uAmbientProduct;
uniform vec4 uDiffuseProduct;
uniform vec4 uSpecularProduct;

uniform vec4 uCameraPosition;
uniform vec4 uLightPosition;
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

	if (uShadingType == 0 ) { // No shading
		fN = fV = fL = vec3(0.0, 0.0, 0.0);
		fColor = uAmbientProduct;
	}
	else {
		// Phong shading is taken care of in the fragment shader
		mat3 MV3x3 = mat3(transpose(inverse(uModelView)));
		fN = (MV3x3 * vNormal);
		vec4 vertexPosition_cameraspace = uModelView * vPosition;
		fV = (uCameraPosition - vertexPosition_cameraspace).xyz;
		fL = uLightPosition.xyz + fV;
		texCoord = vTexCoords;

		if (uShadingType == 1 || uShadingType == 2) {	// Flat Shading || Gouraud Shading
			vec3 N, V, L, H;

			N = normalize(fN);
			V = normalize(fV);
			L = normalize(fL);
        
			H = normalize(L + V);

			vec4 ambient = uAmbientProduct;
			vec4 diffuse = max(dot(L, N), 0.0) * uDiffuseProduct;
			vec4 specular = pow(max(dot(N, H), 0.0), uShininess) * uSpecularProduct;

			if (dot(L, N) < 0.0) {
				specular = vec4(0.0, 0.0, 0.0, 1.0);
			}

			fColor = ambient + diffuse + specular;
			fColor.a = 1.0;
		}
	}
	if (uEnableTexture == 1) {
		texCoord = vTexCoords;
	}	
}
