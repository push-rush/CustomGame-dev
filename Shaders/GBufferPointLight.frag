#version 330

in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;
uniform sampler2D uGSpecPower;

// 点光源信息
struct PointLight
{
    vec3 mWorldPos;
    vec3 mDiffuseColor;
    vec3 mSpecColor;
    float mInnerRadius;
    float mOuterRadius;
};

uniform PointLight uPointLight;
uniform vec2 uScreenDimensions;
uniform vec3 uCameraPos;

void main()
{
    vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;

    vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
    vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
    vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;
    vec3 gbufferSpecPower = texture(uGSpecPower, gbufferCoord).xyz;

    // 计算Phong反射模型下的环境光分量和漫反射分量
    // 平面法线
    vec3 N = normalize(gbufferNorm);

    // 从平面到光源的向量
    vec3 L = normalize(uPointLight.mWorldPos - gbufferWorldPos);

    // 从平面到相机的向量
    vec3 V = normalize(uCameraPos - gbufferWorldPos);

    // -L关于法线N的反射向量
    vec3 R = normalize(reflect(-L, N));

    // 计算Phong反射
    vec3 Phong = vec3(0.0, 0.0, 0.0);
    float NdotL = dot(N, L);
    if (NdotL > 0)
    {
        float dist = distance(uPointLight.mWorldPos, gbufferWorldPos);

        float intensity = smoothstep(uPointLight.mInnerRadius, uPointLight.mOuterRadius, dist);
        vec3 DiffuseColor = mix(uPointLight.mDiffuseColor, vec3(0.0, 0.0, 0.0), intensity);
        vec3 Specular = uPointLight.mSpecColor * pow(max(0.0, dot(R, V)), gbufferSpecPower[0]);
        Specular = mix(Specular, vec3(0.0, 0.0, 0.0), intensity);
        Phong = DiffuseColor * NdotL + Specular;
    }
    
    outColor = vec4(gbufferDiffuse * Phong, 1.0);
}