#version 450

layout (location = 0) out vec2 outUV;

out gl_PerVertex
{
	vec4 gl_Position;
};

vec2 screen_point[] = { 
	{0.0, 0.0},
	{0.0, 1.0},
	{1.0, 0.0},
	{0.0, 1.0},
	{1.0, 1.0},
	{1.0, 0.0},
};

void main() 
{
	// outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	// gl_Position = vec4(outUV * 2.0f - 1.0f, 0.0f, 1.0f);
	// outUV = vec2((gl_VertexIndex >> 1) & 1, gl_VertexIndex & 1);
	gl_Position = vec4(screen_point[gl_VertexIndex] * 2.0 - 1.0, 0.0f, 1.0f);
}