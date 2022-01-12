#version 440
out vec4 color;
in vec4 f_pos;
in vec4 f_vel;

in float distToCamera;
void main() {
    color = vec4(mix(normalize(f_vel).xyz, vec3(1, 1, 1), 0.4), f_pos.w);
}
