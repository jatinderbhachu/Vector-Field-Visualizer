#version 440

struct Particle {
  vec4 position;
  vec4 initialPosition;
  vec4 velocity;
};

layout(std430, binding = 0) buffer ParticleBuffer {
  Particle particles[];
};

out vec4 f_pos;
out vec4 f_vel;

uniform vec2 particleScale;
uniform mat4 cameraView;
uniform mat4 cameraProjection;

vec3 CreateQuad(in uint vertexID) {
    uint b = 1 << vertexID;
    return vec3((0x3 & b) != 0, (0x9 & b) != 0, 0);
}

void main() {
  Particle particle = particles[gl_InstanceID];

  vec3 rect = CreateQuad(gl_VertexID) - 0.5;

  rect.x *= particleScale.x;
  rect.y *= particleScale.y;

  vec4 newPos = cameraView * vec4(particle.position.xyz, 1.0f);
  newPos.xyz += rect;
  gl_Position = cameraProjection * newPos;

  f_pos = particle.position;
  f_vel = particle.velocity;
}

