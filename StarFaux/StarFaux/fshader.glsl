#version 150

in vec4 fColor;
in int st;

in vec3 fN;
in vec3 fV;
in vec3 fL;

uniform float uShininess;
uniform int uShadingType;
uniform vec4 uSpecularColor;
uniform int uEnableTexture;
uniform sampler2D uTexture;

void main() 
{ 
	if (uShadingType == 0 || uShadingType == 2) {
		if (uEnableTexture == 1) gl_FragColor = fColor * texture2D(uTexture, vec2(gl_TexCoord[0]));
        else gl_FragColor = fColor;
	} else if (uShadingType == 1) {
		if (uEnableTexture == 1) gl_FragColor = fColor * texture2D(uTexture, vec2(gl_TexCoord[0]));
        else gl_FragColor = fColor;
	} else if (uShadingType == 3) {
        vec3 N, V, L, H;

        N = normalize(fN);
        V = normalize(fV);
        L = normalize(fL);
        H = normalize(L + V);
    
        vec4 ambient = 0.2 * fColor;
        vec4 diffuse = max(dot(L,N), 0.0) * 0.5 * fColor;
		if (uEnableTexture == 1)
			diffuse *= texture2D(uTexture, vec2(gl_TexCoord[0]));
        vec4 specular = pow(max(dot(N,H), 0.0), uShininess) * 0.5 * uSpecularColor;
        if(dot(L,N) < 0.0){
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }
        gl_FragColor = ambient + diffuse + specular;
        gl_FragColor.a = 1.0;
    }
} 