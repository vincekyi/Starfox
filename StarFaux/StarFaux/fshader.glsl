#version 150

in vec4 fColor;
in vec2 texCoord;

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
	if (1==0) {
		if (uShadingType < 3) {
			if (uEnableTexture == 1) Out_Color = fColor * texture2D(uTexture, texCoord);
			else Out_Color = fColor;
		} else if (uShadingType == 3) {
			vec3 N, V, L, H;

			N = normalize(fN);
			V = normalize(fV);
			L = normalize(fL);
			H = normalize(L + V);
    
			vec4 ambient = uAmbientProduct;
			vec4 diffuse = max(dot(L,N), 0.0) * uDiffuseProduct;
			if (uEnableTexture == 1)
				diffuse *= texture2D(uTexture, texCoord);
			vec4 specular = pow(max(dot(N,H), 0.0), uShininess) * uSpecularProduct;
			if (dot(L,N) < 0.0) {
				specular = vec4(0.0, 0.0, 0.0, 1.0);
			}
			Out_Color = ambient + diffuse + specular;
			Out_Color.a = 1.0;
		}
		vec4 fogColor = fogFactor * uFogColor;
		Out_Color = Out_Color  * fogFactor + fogColor * (1.0 - fogFactor); 

	} else {

		vec3 N = normalize(fN);
		vec3 V = normalize(fV);
		vec3 L = normalize(fL);
		vec3 H = normalize(L + V);
		vec3 PN = -N;
		PN = perturb_normal(-N, V, texCoord);
  
  		vec4 ambient = uAmbientProduct;
		vec4 diffuse = max(dot(PN, L), 0.0) * uDiffuseProduct;
		//ambient = vec4(0.1, 0.1, 0.1, 0.1);
		//diffuse = max(dot(PN, L), 0.0) * vec4(1.0, 1.0, 1.0, 1.0);
		//Out_Color = diffuse;


		vec4 specular = pow(max(dot(PN,H), 0.0), uShininess) * uSpecularProduct;
		if (dot(L, PN) < 0.0) {
			specular = vec4(0.0, 0.0, 0.0, 1.0);
		}
		Out_Color = ambient + diffuse + specular;
		Out_Color.a = 1.0;

		vec4 fogColor = fogFactor * uFogColor;
		Out_Color = Out_Color  * fogFactor + fogColor * (1.0 - fogFactor); 

		/*
		// Light emission properties
		// You probably want to put them as uniforms
		vec4 LightColor = vec4(1.0, 1.0, 1.0, 1.0);
		float LightPower = 0.5;
		
		//vec4 tex01_color = texture(tex0, uv).rgba;

		vec4 tex01_color = vec4(1.0, 1.0, 1.0, 1.0);
		vec4 final_color = vec4(0.2, 0.15, 0.15, 1.0) * tex01_color;
  
		float lambertTerm = dot(PN, L);

		if (lambertTerm > 0.0) {
			vec4 light_diffuse = LightColor;
			vec4 material_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
			vec4 light_specular = LightColor;
			vec4 material_specular = vec4(0.3, 0.3, 0.3, 1.0);
			float material_shininess = 15.0;
			vec4 material_ambient = 0.1 * material_diffuse;

			//final_color += light_diffuse * material_diffuse * lambertTerm * tex01_color;  
			final_color = material_ambient + material_diffuse * lambertTerm;
    
			vec3 R = reflect(-L, PN);
			float specular = pow( max(dot(N, H), 0.0), material_shininess);
			//final_color += light_specular * material_specular * specular;  
		}
		Out_Color.rgb = final_color.rgb;
		//Out_Color.rgb = PN.rgb;
		//Out_Color.rgb = N.rgb;
		Out_Color.a = 1.0;*/

	}
} 
