#version 440
layout (local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
layout (std430, binding = 0) buffer SpawnBuffer {
  vec4 Spawn[];
};
layout (std430, binding = 1) buffer ParticleBuffer {
    vec4 Particle[];
};
layout (std430, binding = 2) buffer VectorBuffer {
    vec4 Vector[];
};
uniform float timestep;
float speed = 2.0;
void main()
{
    uint index = gl_GlobalInvocationID.x;
    vec3 position = Particle[index].xyz;
    float lifetime = Particle[index].w;
    if (lifetime < 0)
    {
        Particle[index] = Spawn[index];
    }else{
        float x = position.x;
        float y = position.y;
        float z = position.z;
        float vX = %s;
        float vY = %s;
        float vZ = %s;
        vec3 velocity = vec3(vX, vY, vZ);
        position += velocity * timestep * speed;
        lifetime -= timestep;
        Particle[index] = vec4(position, lifetime);
        Vector[index] = vec4(velocity, 1);
    }
}
