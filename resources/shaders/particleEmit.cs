#version 440
layout (local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

struct Particle {
  vec4 position;
  vec4 velocity;
};

layout(std430, binding = 0) buffer ParticleBuffer {
  Particle particles[];
};

uniform float timestep;
uniform float speedMultiplier;
uniform uint particleBufferIndex;
uniform uint emitCount;
uniform uint MAX_PARTICLES;


float random (vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43756.873);
}

void main()
{
    uint index = gl_GlobalInvocationID.x;

    // can emit ?
    if(index < emitCound) {
      Particle newParticle;

      newParticle.position = random(vec2(index));


      // add to particle buffer
      uint newIndex = atomicAdd(particleBufferIndex, 1);
      particles[newIndex] = particle;

      // if we are at the end of the buffer, wrap back around to the start
      if(newIndex >= ubo.particleCount) {
        atomicStore(particleBufferIndex, uint(0), gl_ScopeDevice, gl_StorageSemanticsBuffer, gl_SemanticsRelaxed);
      }
    }
}
