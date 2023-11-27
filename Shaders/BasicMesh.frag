#version 330

// 采样纹理坐标
in vec2 fragTexCoord;

// 全局纹理
uniform sampler2D uTexture;

// 输出颜色
out vec4 outColor;

void main()
{
    // 从纹理中取样颜色
    outColor = texture(uTexture, fragTexCoord);
}