#version 330
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 声明变量对应的槽号
layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexCoord;

out vec2 fragTexCoord;

// 法线（世界空间）
out vec3 fragNormal;

// 位置（世界空间）
out vec3 fragWorldPos;

void main()
{
    // 位置坐标转换为齐次坐标
    vec4 pos = vec4(inPosition, 1.0);

    // 从对象空间转到世界空间
    pos = pos * uWorldTransform;

    // 保存世界坐标
    fragWorldPos = pos.xyz;

    // 转到剪辑空间
    gl_Position = pos * uViewProj;

    // 将法线转换到世界空间
    fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

    // 将纹理坐标传给片段着色器
    fragTexCoord = inTexCoord;
}

