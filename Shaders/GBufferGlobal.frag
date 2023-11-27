#version 330
in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D uGDiffuse;
uniform sampler2D uNormal;
uniform sampler2D uGWorldPos;
uniform sampler2D uGSpecPower;

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
//uniform float uSpecPower;

// 全局定向光
uniform DirectionalLight uDirLight;

void main()
{
    vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
    vec3 gbufferNorm = texture(uNormal, fragTexCoord).xyz;
    vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;
    vec3 gbufferSpecPower = texture(uGSpecPower, fragTexCoord).xyz;

    // 计算Phong反射模型下的环境光分量和漫反射分量
    // 平面法线
    vec3 N = normalize(gbufferNorm);

    // 从平面到光源的向量
    vec3 L = normalize(-uDirLight.mDirection);

    // 从平面到相机的向量
    vec3 V = normalize(uCameraPos - gbufferWorldPos);

    // -L关于法线N的反射向量
    vec3 R = normalize(reflect(-L, N));

    // 计算Phong反射
    vec3 Phong = uAmbientLight;
    float NdotL = dot(N, L);
    if (NdotL > 0)
    {
        vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
        // G缓存区未存放物体表面的镜面反射率
        vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), gbufferSpecPower[0]);
        Phong += (Diffuse + Specular);
        //Phong += Diffuse;
    }
    Phong = clamp(Phong, 0.0, 1.0);

    outColor = vec4(gbufferDiffuse * Phong, 1.0);
}