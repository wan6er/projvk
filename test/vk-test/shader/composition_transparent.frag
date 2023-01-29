#version 450

layout (input_attachment_index = 0, binding = 0) uniform subpassInput samplerPosition;
layout (input_attachment_index = 1, binding = 1) uniform subpassInput samplerNormal;
layout (input_attachment_index = 2, binding = 2) uniform subpassInput samplerAlbedo;

layout (input_attachment_index = 3, binding = 3) uniform subpassInput transparentPosition;
layout (input_attachment_index = 4, binding = 4) uniform subpassInput transparentNormal;
layout (input_attachment_index = 5, binding = 5) uniform subpassInput transparentAlbedo;

// layout (location = 0) in vec2 inTexcoord;
// layout (location = 1) in vec4 inPosition;
// layout (location = 2) in vec4 inNormal;
// layout (location = 3) in vec4 inColor;

layout (location = 0) out vec4 outColor;

layout (binding = 6) uniform UBO 
{
	vec4 viewPos;
	vec4 position;
	vec3 color;
	float radius;
} ubo;

float diffuse(vec3 pos_light, vec3 pos, vec3 nor) 
{
	vec3 view_dir = pos_light - pos;
	return max(0.0, dot(nor, normalize(view_dir)));
}

float attenuation(float len, float radius) 
{
	// return len;
	len = pow(len / radius, 2.0);
	return 1.0 - min(1.0, len);
}

vec3 get_color()
{
	vec3 fragPos = subpassLoad(samplerPosition).xyz;
	float fragDepth = subpassLoad(samplerPosition).a;
	vec3 normal = subpassLoad(samplerNormal).xyz;
	vec4 albedo = subpassLoad(samplerAlbedo);

	float ambient = 0.15;
	float diff = 0.4 * diffuse(ubo.position.xyz, fragPos, normal) * attenuation(length(ubo.position.xyz - fragPos), ubo.radius);
	return albedo.rgb * ubo.color * (ambient + diff);
}

vec3 get_transparent_color()
{
	vec3 fragPos = subpassLoad(transparentPosition).xyz;
	float fragDepth = subpassLoad(transparentPosition).a;
	vec3 normal = subpassLoad(transparentNormal).xyz;
	vec4 albedo = subpassLoad(transparentAlbedo);

	float ambient = 0.15;
	float diff = 0.4 * diffuse(ubo.position.xyz, fragPos, normal) * attenuation(length(ubo.position.xyz - fragPos), ubo.radius);
	return albedo.rgb * ubo.color * (ambient + diff);
}

void main() 
{
	float fragDepth = subpassLoad(transparentPosition).a;
	
	if (fragDepth > 0) {
		vec3 color = get_color();
		vec3 tColor = get_transparent_color();
		float lambda = 0.6;
		outColor = vec4((1.0 - lambda) * tColor + lambda * color, 1.0);
	} else {
		outColor = vec4(get_color(), 1.0);
	}
}