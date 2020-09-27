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
    vec4 status = Particle[index];
    vec3 position = status.xyz;
    float lifetime = status.w;
    if (lifetime < 0)
    {
        Particle[index] = Spawn[index];
        //Particle[index] = vec4(1-rand(position.xy)*2, 0, 1-rand(position.zx)*2, Spawn[index].w);
        //Particle[index] = vec4(0, 0, 0, Spawn[index].w);
    }else{
        //vec3 velocity = curlNoise(position) * 0.6;
        //vec3 velocity = snoiseVec3(position) * 0.6;
        //vec3 velocity = Vector[index].xyz * 0.6;
        float x = position.x;
        float y = position.y;
        float z = position.z;
        float o = 54.38;
        float r = 3.5;
        //float vX = o*(y-x);
        //float vY = r*x - y - x*z;
        //float vZ = x*y - 0.2*z;
        float vX = y+x;
        float vY = -x+(sin(y+z));
        float vZ = 0;
        //float vX = y*z;
        //float vY = x*z;
        //float vZ = x*y;

        vec3 velocity = vec3(vX, vY, vZ);
        position += velocity * timestep * speed;
        lifetime -= timestep;
        Particle[index] = vec4(position, lifetime);
    }
}
