#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    float redRatio = 1.00 / 1.42;
    float greenRatio = 1.00 / 1.52;
    float blueRatio = 1.00 / 1.62;
    vec3 I = normalize(Position - cameraPos);
    vec3 redR = refract(I, normalize(Normal), redRatio);
    vec3 greenR = refract(I, normalize(Normal), greenRatio);
    vec3 blueR = refract(I, normalize(Normal), blueRatio);
    FragColor = vec4(texture(skybox, redR).r, texture(skybox, greenR).g, texture(skybox, blueR).b, 1.0);
}