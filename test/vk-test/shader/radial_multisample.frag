#version 450

#define SAMPLES 32

layout (binding = 0) uniform sampler2D inColor;
// layout (input_attachment_index = 0, binding = 0) uniform subpassInput inColor;

layout (binding = 1) uniform RadialUBO
{
    float len;
    float placeholder;
    vec2 origin;
} ubo;

layout (location = 0) in vec2 inPos;

layout (location = 0) out vec4 outColor;

void main()
{
    // vec4 prevColor = subpassLoad(inColor);
    const vec2 origin = vec2(0.5);
    // vec2 stride = 1.0 / textureSize(inColor, 0) * 10;
    outColor = texture(inColor, inPos);
    // outColor = vec4(0.0);

    // if (length(inPos - origin) < 0.3)
    for (int i = 0; i < SAMPLES; ++i) {
        vec4 prevColor = texture(inColor, (inPos - origin) * 0.025 * i + origin);
        outColor += prevColor * (1.0 / SAMPLES) * 0.3;
    }

    // if (outColor.a == 0.0) {
    //     discard;
    // }
}