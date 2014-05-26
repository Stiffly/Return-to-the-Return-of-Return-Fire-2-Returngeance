#version 430

layout(binding=0) uniform sampler2D texture0;

in VertexData {
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
} Input;
 
out vec4 fragmentColor;

void main() {
	// Texture
	vec4 texel = texture(texture0, Input.TextureCoord);

	fragmentColor = texel;
}