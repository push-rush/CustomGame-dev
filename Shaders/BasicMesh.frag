#version 330

// 采样纹理坐标
in vec2 fragTexCoord;

// 全局纹理
uniform sampler2D uTexture;

// 网格颜色
uniform vec3 uColor;

// 输出颜色
out vec4 outColor;

void main()
{
    // 从纹理中取样颜色
    // outColor = texture(uTexture, fragTexCoord);
    
    // 外部输入颜色
    outColor = vec4(uColor, 1.0);
}