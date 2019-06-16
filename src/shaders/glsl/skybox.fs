#version 330 core
out vec4 FragColor;

in vec3 st;

uniform samplerCube cubeMap;
const float lowerLimit = -10.0;
const float upperLimit = 45.0;

void main(void) {
	vec3 fogColor  = vec3(0.5,0.6,0.7);
    float factor = (st.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor, 0.0, 1.0);
    FragColor = mix(vec4(fogColor, 1.0f), texture(cubeMap, st), factor);
}