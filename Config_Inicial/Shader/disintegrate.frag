#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float v_alpha;

uniform vec3 viewPos;
uniform vec3 u_color = vec3(0.9, 0.4, 0.1);

void main()
{
    // luz direccional simple para que no se vea plano
    vec3 lightDir = normalize(vec3(-0.2, -1.0, -0.3));
    float diff = max(dot(normalize(Normal), -lightDir), 0.0);
    vec3 col = (0.2 + 0.8 * diff) * u_color;

    if (v_alpha < 0.05)
        discard;

    FragColor = vec4(col, v_alpha);
}
