#version 450

layout (location = 0) in vec2 inTexcoord;
layout (location = 1) in vec4 inPosition;
layout (location = 2) in vec4 inNormal;
layout (location = 3) in vec4 inColor;

layout (binding = 2) uniform sampler2D samplerTexture;

layout (location = 0) out vec4 outFragColor;

void main() 
{
  outFragColor = texture(samplerTexture, inTexcoord);
}