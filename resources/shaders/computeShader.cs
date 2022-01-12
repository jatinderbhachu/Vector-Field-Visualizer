#version 440
layout (local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

struct Particle {
  vec4 position;
  vec4 initialPosition;
  vec4 velocity;
};

layout(std430, binding = 0) buffer ParticleBuffer {
  Particle particles[];
};

uniform float timestep;
uniform float speedMultiplier;

void main()
{
    uint index = gl_GlobalInvocationID.x;
    Particle particle = particles[index];
    vec3 position = particle.position.xyz;
    float lifetime = particle.position.w;
    if (lifetime < 0)
    {
        particles[index].position = particles[index].initialPosition;
    } else {
        float x = position.x;
        float y = position.y;
        float z = position.z;

        float vX = y+x;
        float vY = -x+(sin(y+z));
        float vZ = 0;

        vec3 velocity = vec3(vX, vY, vZ);
        position += velocity * timestep * speedMultiplier;
        //position = vec3(0.0f);
        lifetime -= timestep;
        particles[index].position = vec4(position, lifetime);
        particles[index].velocity.xyz = velocity;

    }
}
