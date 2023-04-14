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
    const vec2 origin = vec2(0.5);
    const float stride = (1.0 / SAMPLES);
    outColor = texture(inColor, inPos);

    for (int i = 0; i < SAMPLES; ++i) {
        float proportion = (SAMPLES + i) * 0.5;
        vec4 prevColor = texture(inColor, (inPos - origin) * stride * proportion + origin);
        outColor += prevColor * stride * 0.3;
    }

}