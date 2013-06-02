#version 150

in vec4 fColor;
in vec2 texCoord[2];

in vec3 fN;
in vec3 fV;
in vec3 fL;

in float fogFactor;

uniform float uShininess;
uniform int uShadingType;
uniform int uEnableTexture;
uniform sampler2D uTexture;

uniform vec4 uAmbientProduct;
uniform vec4 uDiffuseProduct;
uniform vec4 uSpecularProduct;

uniform vec4 uFogColor;

void main() 
{ 
	if (uShadingType < 3) {
		if (uEnableTexture == 1) gl_FragColor = fColor * texture2D(uTexture, vec2(texCoord[0]));
        else gl_FragColor = fColor;
	} else if (uShadingType == 3) {
        vec3 N, V, L, H;

        N = normalize(fN);
        V = normalize(fV);
        L = normalize(fL);
        H = normalize(L + V);
    
        vec4 ambient = uAmbientProduct;
        vec4 diffuse = max(dot(L,N), 0.0) * uDiffuseProduct;
		if (uEnableTexture == 1)
			diffuse *= texture2D(uTexture, vec2(texCoord[0]));
        vec4 specular = pow(max(dot(N,H), 0.0), uShininess) * uSpecularProduct;
        if (dot(L,N) < 0.0) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }
        gl_FragColor = ambient + diffuse + specular;
        gl_FragColor.a = 1.0;
    }
	vec4 fogColor = fogFactor * uFogColor;
	gl_FragColor = gl_FragColor  * fogFactor + fogColor * (1.0 - fogFactor); 
} 
