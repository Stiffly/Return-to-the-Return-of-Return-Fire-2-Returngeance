#version 430

layout (binding=0) uniform sampler2D DiffuseTexture;
layout (binding=1) uniform sampler2D ShadowTexture;

in VertexData
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
	vec4 ShadowCoord;
} Input;

out vec4 frag_Diffuse;
out vec4 frag_Position;
out vec4 frag_Normal;

float Shadow(vec4 ShadowCoord)
{
	if( texture(ShadowTexture, Input.ShadowCoord.xy).z < ShadowCoord.z )
	{
		return 0.3;
	}
	else 
	{
		return 1.0;
	}
}

void main()
{
	// Diffuse Texture
	frag_Diffuse = texture(DiffuseTexture, Input.TextureCoord) * Shadow(Input.ShadowCoord);

	// G-buffer Position
	frag_Position = vec4(Input.Position.xyz, 1.0);

	// G-buffer Normal
	frag_Normal = vec4(Input.Normal, 0.0);
}