#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexcoord;

layout (binding = 0) uniform UBO 
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
} ubo;

layout (location = 0) out vec3 outColor;

out gl_PerVertex 
{
    vec4 gl_Position;   
};


void main() 
{
	outColor = inColor;
	
	// vec2 outUV = vec2(gl_VertexIndex & 1, (gl_VertexIndex >> 1) & 1);
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * vec4(inPos.xyz, 1.0);
}
