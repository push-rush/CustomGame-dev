#version 330

in vec3 fragColor;

out vec4 outColor;

void main()
{
    // outColor = vec4(0.0, 0.0, 1.0, 1.0);

    outColor = vec4(fragColor, 1.0);
}