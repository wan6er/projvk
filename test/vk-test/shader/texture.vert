#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 color;

layout (binding = 0) uniform UBO 
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
} ubo;

layout (binding = 1) uniform MODEL
{
	mat4 transform;
} model;

layout (location = 0) out vec2 outTexcoord;
layout (location = 1) out vec4 outPosition;
layout (location = 2) out vec4 outNormal;
layout (location = 3) out vec4 outColor;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
	mat4 model_trans = ubo.modelMatrix * model.transform;
	vec4 worldPos = model_trans * vec4(position.xyz, 1.0);
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * worldPos;
	
	outTexcoord = texcoord;
	outColor = vec4(color, 1.0);
	outPosition = vec4(worldPos.xyz, gl_Position.a);

	mat3 mNormal = transpose(inverse(mat3(model_trans)));
	outNormal = vec4(mNormal * normalize(normal), 1.0);
}
