#version 130

uniform sampler2D diffuseTex; // The color information
uniform sampler2D posTex;     // World position
uniform sampler2D normalTex;  // Normals
uniform sampler2D blendTex;   // A bitmap with colors to blend with.
uniform vec3 camera;          // The coordinate of the camera
in vec2 position;             // The world position
layout (location = 0) out vec4 fragColor;

void main(void)
{
   // Load data, stored in textures, from the first stage rendering.
   vec4 diffuse = texture2D(diffuseTex, position.xy);
   vec4 blend = texture2D(blendTex, position.xy);
   vec4 worldPos = texture2D(posTex, position.xy);
   vec4 normal = texture2D(normalTex, position.xy);
   // Use information about lamp coordinate (not shown here), the pixel
   // coordinate (worldpos.xyz), the normal of this pixel (normal.xyz)
   // to compute a lighting effect.
   // Use this lighting effect to update 'diffuse'
   vec4 preBlend = diffuse * lamp + specularGlare;
   // manual blending, using premultiplied alpha.
   fragColor = blend + preBlend*(1-blend.a);
// Some debug features. Enable any of them to get a visual representation
// of an internal buffer.
// fragColor = (normal+1)/2;
// fragColor = diffuse;
// fragColor = blend;
// fragColor = worldPos; // Scaling may be needed to range [0,1]
// fragColor = lamp*vec4(1,1,1,1);
}