#version 330
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
uniform mat4 uMatrixPalette[96];

// 声明变量对应的槽号
layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inNormal;
layout(location=2) in uvec4 inSkinBones;
layout(location=3) in vec4 inSkinWeights;
layout(location=4) in vec2 inTexCoord;

out vec2 fragTexCoord;

// 法线（世界空间）
out vec3 fragNormal;

// 位置（世界空间）
out vec3 fragWorldPos;

void main()
{
    // 位置坐标转换为齐次坐标
    vec4 pos = vec4(inPosition, 1.0);

    vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
    skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
    skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
    skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

    // 从对象空间转到世界空间
    skinnedPos = skinnedPos * uWorldTransform;

    // 保存世界坐标
    fragWorldPos = skinnedPos.xyz;

    // 转到剪辑空间
    gl_Position = skinnedPos * uViewProj;

    vec4 skinnedNormal = vec4(inNormal, 0.0f);
    skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x
                  + (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y
                  + (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z
                  + (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;
    // 将法线转换到世界空间
    fragNormal = (skinnedNormal * uWorldTransform).xyz;

    // 将纹理坐标传给片段着色器
    fragTexCoord = inTexCoord;
}

