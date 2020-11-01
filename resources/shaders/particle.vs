#version 440
layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 vel;
out vec4 f_pos;
out vec4 f_vel;
out float distToCamera;
uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(pos.xyz, 1.0);

    distToCamera = gl_Position.w;

    f_pos = pos;
    f_vel = vel;
}

