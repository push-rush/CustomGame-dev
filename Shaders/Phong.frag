#version 330

// 采样纹理坐标
in vec2 fragTexCoord;

// 法线（世界空间）
in vec3 fragNormal;

// 位置（世界空间）
in vec3 fragWorldPos;

// 全局纹理
uniform sampler2D uTexture;

// 输出颜色
out vec4 outColor;

// 创建针对定向光的结构体
struct DirectionalLight
{
    vec3 mDirection;    // 定向光方向
    vec3 mDiffuseColor; // 漫反射颜色
    vec3 mSpecColor;    // 镜面反射颜色
};

// 全局光照
// 相机位置
uniform vec3 uCameraPos;

// 环境光级别
uniform vec3 uAmbientLight;

// 镜面反射率 
uniform float uSpecPower;

// 全局定向光
uniform DirectionalLight uDirLight;

void main()
{
    // 平面法线
    vec3 N = normalize(fragNormal);

    // 从平面到光源的向量
    vec3 L = normalize(-uDirLight.mDirection);

    // 从平面到相机的向量
    vec3 V = normalize(uCameraPos - fragWorldPos);

    // -L关于法线N的反射向量
    vec3 R = normalize(reflect(-L, N));

    // 计算Phong反射
    vec3 Phong = uAmbientLight;
    float NdotL = dot(N, L);
    if (NdotL > 0)
    {
        vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
        vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
        Phong += (Diffuse + Specular);
    }

    // 最终颜色（纹理颜色*Phong光照 Alpha=1）
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}