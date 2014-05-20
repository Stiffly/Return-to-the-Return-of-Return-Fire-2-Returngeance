#version 430

layout (binding=0) uniform sampler2D PositionTexture;
layout (binding=1) uniform sampler2D NormalsTexture;
layout (binding=2) uniform sampler2D SpecularTexture;

uniform vec2 ViewportSize;
uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 la;
uniform vec3 ls;
uniform vec3 ld;
uniform vec3 lp;
uniform float specularExponent;
uniform vec3 CameraPosition;
uniform float ConstantAttenuation;
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;
uniform float LightRadius;

const vec3 ks = vec3(1.0, 1.0, 1.0);
const vec3 kd = vec3(1.0, 1.0, 1.0);
const vec3 ka = vec3(1.0, 1.0, 1.0);
const float kshine = 1.0;


in VertexData
{
	vec3 Position;
	vec2 TextureCoord;
} Input;

out vec4 FragColor;

vec4 phong(vec3 position, vec3 normal, vec3 specular)
{
	// Diffuse
	vec3 lightPos = vec3(V * vec4(lp, 1.0));
	vec3 distanceToLight = lightPos - position;
	vec3 directionToLight = normalize(distanceToLight);
	float dotProd = dot(directionToLight, normal);
	dotProd = max(dotProd, 0.0);
	vec3 Id = kd * ld * dotProd;

	// Specular
	//vec3 reflection = reflect(-directionToLight, normal);
	vec3 surfaceToViewer = normalize(-position);
	vec3 halfWay = normalize(surfaceToViewer + directionToLight);
	float dotSpecular = max(dot(halfWay, normal), 0.0);
	float specularFactor = pow(dotSpecular, specularExponent * 2.0);
	vec3 Is = specular.r * ls * specularFactor;

	//Attenuation
	float dist = distance(lightPos, position);
	//float attenuation = -log(min(1.0, dist / LightRadius));

	//float attenuation = 1.0 / (ConstantAttenuation + (LinearAttenuation * dist) + (QuadraticAttenuation * dist * dist));

	float attenuation = pow(max(0.0f, 1.0 - (dist / LightRadius)), 2);

	//float attenuation = 1.0 / (1.0 - 0.0001 * pow(dist, 2));

	//float attenuation = clamp(0.0, 1.0, 1.0 / (0.001 + (0.001 * dist) + (0.001 * dist * dist)));

	//float attenuation = 1.0 / dot(directionToLight, directionToLight);

	//float att_s = 5;
	//float attenuation = pow(dist, 2) / pow(5.0, 2);
	//attenuation = 1.0 / (1.0 + attenuation * att_s);
	//att_s = 1.0 / (1.0 + att_s);
	//attenuation = attenuation / (1.0 - att_s);

	//float radius = 5.0;
	//float alpha = dist / radius;
	//float dampingFactor = 1.0 - pow(alpha, 3);

	return vec4((Id + Is) * attenuation, 1.0);
}

void main()
{
	vec2 TextureCoord = gl_FragCoord.xy / ViewportSize;
	vec4 PositionTexel = texture(PositionTexture, TextureCoord);
	vec4 NormalTexel = texture(NormalsTexture, TextureCoord);
	vec4 SpecularTexel = texture(SpecularTexture, TextureCoord);

	FragColor = phong(vec3(PositionTexel), vec3(NormalTexel), vec3(SpecularTexel))+vec4(0.1f);
	//FragColor = NormalTexel;
}