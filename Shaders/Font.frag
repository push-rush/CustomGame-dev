#version 330

// 采样纹理坐标
in vec2 fragTexCoord;

// 全局纹理
uniform sampler2D uTexture;
uniform vec3 uColor;

// 输出颜色
out vec4 outColor;

void main()
{
    // 从纹理中取样颜色
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTexture, fragTexCoord).r);
    outColor = vec4(uColor, 1.0) * sampled;
}