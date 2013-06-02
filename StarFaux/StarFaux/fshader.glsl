#version 150

in vec4 fColor;
in int st;
in vec2 texCoord[2];

in vec3 fN;
in vec3 fV;
in vec3 fL[5];

in float fogFactor;

uniform float uShininess;
uniform int uShadingType;
uniform int uEnableTexture;
uniform sampler2D uTexture;

uniform int uNumLights;
uniform vec4 uAmbientProduct[5];
uniform vec4 uDiffuseProduct[5];
uniform vec4 uSpecularProduct[5];

uniform vec4 uFogColor;

void main() 
{
	//gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

	if (uShadingType < 3) {
		if (uEnableTexture == 1) gl_FragColor = fColor * texture2D(uTexture, vec2(texCoord[0]));
        else gl_FragColor = fColor;
	} else if (uShadingType == 3) {
        vec3 N, V, L, H;

        N = normalize(fN);
        V = normalize(fV);
		for (int i = 0; i < uNumLights; i++) {
			L = normalize(fL[i]);
			H = normalize(L + V);
			float distance = 0.0000005 * (pow(fL[i].x, 2) + pow(fL[i].y, 2) + pow(fL[i].z, 2));
			if (distance < 1.0) {
				distance = 1.0;
			}

			vec4 ambient = uAmbientProduct[i] / distance;
			vec4 diffuse = max(dot(L,N), 0.0) * uDiffuseProduct[i] / distance;
			if (uEnableTexture == 1)
				diffuse *= texture2D(uTexture, vec2(texCoord[0]));
			vec4 specular = pow(max(dot(N,H), 0.0), uShininess) * uSpecularProduct[i] / distance;
			if (dot(L,N) < 0.0) {
				specular = vec4(0.0, 0.0, 0.0, 1.0);
			}
			gl_FragColor += ambient + diffuse + specular;
		}
        gl_FragColor.a = 1.0;
    }
	vec4 fogColor = fogFactor * uFogColor;
	gl_FragColor = gl_FragColor  * fogFactor + fogColor * (1.0 - fogFactor); 
} 
