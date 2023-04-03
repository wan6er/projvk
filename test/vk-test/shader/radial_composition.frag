#version 450

layout (input_attachment_index = 0, binding = 0) uniform subpassInput samplerPosition;
layout (input_attachment_index = 1, binding = 1) uniform subpassInput samplerNormal;
layout (input_attachment_index = 2, binding = 2) uniform subpassInput samplerAlbedo;

// layout (location = 0) in vec2 inTexcoord;
// layout (location = 1) in vec4 inPosition;
// layout (location = 2) in vec4 inNormal;
// layout (location = 3) in vec4 inColor;

layout (location = 0) out vec4 outColor;

layout (binding = 3) uniform UBO 
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

float specular(vec3 pos_light, vec3 pos_camera, vec3 pos, vec3 nor) 
{
	vec3 light_dir = normalize(pos_light - pos) + normalize(pos_camera - pos);
	return pow(max(0.0, dot(normalize(nor), normalize(light_dir))), 4.0);
	// return 1.0;
}

float attenuation(float len, float radius) 
{
	// return len;
	len = pow(len / radius, 2.0);
	return 1.0 - max(min(1.0, len), 0.0);
}

vec3 get_color(vec3 fragPos, vec3 normal, vec4 albedo)
{
	float atten = attenuation(length(ubo.position.xyz - fragPos), ubo.radius);

	float ambient = 0.05;
	float diff = 0.3 * diffuse(ubo.position.xyz, fragPos, normal) * atten;
	float spec = 0.6 * specular(ubo.position.xyz, ubo.viewPos.xyz, fragPos, normal) * atten;
	return (albedo.rgb) * (ambient + diff) + spec;
	// return vec3(spec);
}

void main() 
{
	float fragDepth = subpassLoad(samplerPosition).a;
	if (fragDepth > 0) {
		vec3 fragPos = subpassLoad(samplerPosition).xyz;
		vec3 normal = subpassLoad(samplerNormal).xyz;
		vec4 albedo = subpassLoad(samplerAlbedo);

		if (albedo.r > 0.9 || albedo.g > 0.9 || albedo.b > 0.9) {
			outColor = albedo;
		} else {
			outColor = vec4(get_color(fragPos, normal, albedo), 1.0);
		}

	} else {
		outColor = vec4(0.0);
		discard;
	}
}