#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float u_time;       // tiempo que lleva explotando
uniform float u_explode;    // 0 = off, 1 = on
uniform float u_strength;   // qué tanto se separa

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out float v_alpha;

// ruido baratito a partir de la posición
float hash31(vec3 p) {
    return fract(sin(dot(p, vec3(12.9898,78.233,45.164))) * 43758.5453);
}

void main()
{
    vec3 displaced = aPos;
    float alpha = 1.0;

    if (u_explode > 0.5) {
        float start = hash31(aPos * 10.0) * 1.5;   // cada vértice empieza en un momento distinto
        float t = max(u_time - start, 0.0);

        vec3 dir = normalize(aNormal + vec3(
            hash31(aPos * 3.1) - 0.5,
            hash31(aPos * 5.7) - 0.5,
            hash31(aPos * 8.3) - 0.5
        ));

        displaced += dir * t * u_strength;
        alpha = clamp(1.0 - t * 0.7, 0.0, 1.0);
    }

    vec4 worldPos = model * vec4(displaced, 1.0);
    FragPos = vec3(worldPos);
    Normal  = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    v_alpha = alpha;

    gl_Position = projection * view * worldPos;
}

