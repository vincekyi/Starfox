#version 150

in vec4 fColor;
in vec2 texCoord;

in vec3 fN;
in vec3 fV;
in vec3 fL;

in float fogFactor;

uniform float uShininess;
uniform int uShadingType;
uniform sampler2D uTexture;

// 0: no texture
// 1: regular texture
// 2: bump mapping
uniform int uUseTexture;

uniform vec4 uAmbientProduct;
uniform vec4 uDiffuseProduct;
uniform vec4 uSpecularProduct;

uniform vec4 uFogColor;

out vec4 Out_Color;

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

vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
	// assume N, the interpolated vertex normal and 
	// V, the view vector (vertex to eye)
	vec3 map = texture(uTexture, texcoord ).xyz;
	map = map * 255./127. - 128./127.;
	mat3 TBN = cotangent_frame(N, -V, texcoord);
	return normalize(TBN * map);
}

void main() 
{ 
	if (uShadingType < 3) {
		if (uUseTexture == 1) {
			Out_Color = fColor * texture2D(uTexture, texCoord);
		} else {
			Out_Color = fColor;
		}
	} else if (uShadingType == 3) {
		vec3 N, V, L, H;

		N = normalize(fN);
		V = normalize(fV);
		L = normalize(fL);
		H = normalize(L + V);

		vec4 diffuse = uDiffuseProduct;

		// Use bump mapping
		if (uUseTexture == 2) {
			N = perturb_normal(N, V, texCoord);
		} else if (uUseTexture == 1) {
			diffuse *= texture2D(uTexture, texCoord);
		}
    
		float lambertTerm = max(dot(L, N), 0.0);
		diffuse *= lambertTerm;

		// Can't see bumps behind light, so fake it with ambience
		float bumpTerm = N.x + N.y + N.z; 
		vec4 ambient = bumpTerm * uAmbientProduct;

		vec4 specular = pow(max(dot(N,H), 0.0), uShininess) * uSpecularProduct;
		if (dot(L,N) < 0.0) {
			specular = vec4(0.0, 0.0, 0.0, 1.0);
		}
		Out_Color = ambient + diffuse + specular;
		Out_Color.a = 1.0;
	}
	vec4 fogColor = fogFactor * uFogColor;
	Out_Color = Out_Color  * fogFactor + fogColor * (1.0 - fogFactor); 
} 
