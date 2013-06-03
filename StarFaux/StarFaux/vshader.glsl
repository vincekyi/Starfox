#version 150

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
out float fogFactor;
out vec4 fColor;
out vec2 texCoord;

out vec3 fN; //normal at current position
out vec3 fV; //vector from point to viewer
out vec3 fL[5]; //vector from point to light

uniform mat4 uProj;
uniform mat4 uModelView;
uniform mat4 uModel;
uniform mat4 uView;
uniform float uShininess;

uniform int uNumLights;
uniform vec4 uAmbientProduct[15];
uniform vec4 uDiffuseProduct[15];
uniform vec4 uSpecularProduct[15];
uniform vec4 uLightPosition[15];
uniform float uAttenuation[15];

uniform vec4 uCameraPosition;
uniform int uShadingType;

uniform int uUseTexture;
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

	if (uShadingType == 0 ) { // No shading
		fN = fV = vec3(0.0, 0.0, 0.0);
		fL[0] = vec3(0.0, 0.0, 0.0);
		fColor = uAmbientProduct[0];
	}
	else {	// Flat Shading || Gouraud Shading || Phong Shading
		// Will's fN, fV calculation
		//fN = (uModel * vec4(vNormal.x, vNormal.y, vNormal.z, 0.0)).xyz;
		//fV = (uCameraPosition - uModel * vPosition).xyz;
		// Phong shading is taken care of in the fragment shader

		// Get the normal matrix - the 3x3 upper left matrix with some transformations
		// Should be same as gl_NormalMatrix which is deprecated
		//mat3 MV3x3 = mat3(transpose(inverse(uModelView)));
		mat3 MV3x3 = mat3(uModelView);

		vec4 vertexPosition_cameraspace = uModelView * vPosition;

		fN = (MV3x3 * vNormal);
		// Vector that goes from the vertex to the camera, in camera space.
		// In camera space, the camera is at the origin (0,0,0).
		//fV = (uCameraPosition - vertexPosition_cameraspace).xyz;
		fV = -vertexPosition_cameraspace.xyz;
		
		for (int i = 0; i < uNumLights; i++) {
			// Vector that goes from the vertex to the light, in camera space.
			vec3 LightPosition_cameraspace = (uView * uLightPosition[i]).xyz;
			fL[i] = LightPosition_cameraspace + fV;
			//fL[i] = uLightPosition.xyz + fV;
			//fL[i] = (uLightPosition[i] - uModel * vPosition).xyz;
		}

		if (uShadingType == 1 || uShadingType == 2) {	// Flat Shading || Gouraud Shading
			vec3 N, V, L, H;
			N = normalize(fN);
			V = normalize(fV);


			for (int i = 0; i < uNumLights; i++) {
				fL[i] = (uLightPosition[i] - uModel * vPosition).xyz;
				float distance = (pow(fL[i].x, 2) + pow(fL[i].y, 2) + pow(fL[i].z, 2)) * uAttenuation[i];
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
	} 

	if (uUseTexture == 1 || uUseTexture == 2) {
		texCoord = vTexCoords;
	}	
}
