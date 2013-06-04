#version 150

in vec4 fColor;

in vec2 texCoord;

in vec3 fN;
in vec3 fV;
in vec3 fL[15];

in float fogFactor;

uniform float uShininess;
uniform int uShadingType;
uniform sampler2D uTexture;

uniform int uNumLights;
uniform vec4 uAmbientProduct[15];
uniform vec4 uDiffuseProduct[15];
uniform vec4 uSpecularProduct[15];
uniform float uAttenuation[15];

// 0: no texture
// 1: regular texture
// 2: bump mapping
uniform int uUseTexture;

uniform vec4 uFogColor;
uniform int fogFlag;

out vec4 outColor;

// For thrusters
uniform vec4 uThrustColor;
uniform int uIsThruster;

// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec2 atexcoord )
{
	// assume N, the interpolated vertex normal and 
	// V, the view vector (vertex to eye)
	vec3 map = texture(uTexture, atexcoord ).xyz;
	map = map * 255./127. - 128./127.;
	mat3 TBN = cotangent_frame(N, -V, atexcoord);
	return normalize(TBN * map);
}

void main() 
{
	outColor = vec4(0.0, 0.0, 0.0, 1.0);

	if (uShadingType < 3) {
		if (uUseTexture == 1) {
			if (uIsThruster == 1) {
				outColor = uThrustColor * texture2D(uTexture, texCoord).bgra;
			} else {
				outColor = fColor * texture2D(uTexture, texCoord);
			}
		} else {
			outColor = fColor;
		}
	} else if (uShadingType == 3) {
		vec3 N, V, L, H;

        N = normalize(fN);
        V = normalize(fV);
		for (int i = 0; i < uNumLights; i++) {
			L = normalize(fL[i]);
			H = normalize(L + V);
			float distance = (pow(fL[i].x, 2) + pow(fL[i].y, 2) + pow(fL[i].z, 2)) * uAttenuation[i];
			if (distance < 1.0) {
				distance = 1.0;
			}

			vec4 ambient = uAmbientProduct[i] / distance;
			vec4 diffuse = uDiffuseProduct[i] / distance;
			//vec4 diffuse = max(dot(L,N), 0.0) * uDiffuseProduct[i] / distance;

			// Use bump mapping
			if (uUseTexture == 2) {
				N = perturb_normal(N, V, texCoord);
				// Can't see bumps behind light, so fake it with ambience
				float bumpTerm = N.x + N.y + N.z; 
				ambient *= bumpTerm;
			// Use regular texture
			} else if (uUseTexture == 1) {
				diffuse *= texture2D(uTexture, texCoord);
			}
			float lambertTerm = max(dot(L, N), 0.0);
			diffuse *= lambertTerm;

			vec4 specular = pow(max(dot(N,H), 0.0), uShininess) * uSpecularProduct[i] / distance;
			if (dot(L,N) < 0.0) {
				specular = vec4(0.0, 0.0, 0.0, 1.0);
			}
			outColor += ambient + diffuse + specular;
		}
        outColor.a = 1.0;
    }

	vec4 fogColor = fogFactor * uFogColor;
	if(fogFlag == 1){
		outColor = outColor  * fogFactor + fogColor * (1.0 - fogFactor); 
	}
} 
