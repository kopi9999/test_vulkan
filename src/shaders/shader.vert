#version 450
#extension GL_KHR_vulkan_glsl : enable

vec2 positions[3] = vec2[](
    vec2(0.0f, -0.5f),
    vec2(0.5f, 0.5f),
    vec2(-0.5f, 0.5f)
);

void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
