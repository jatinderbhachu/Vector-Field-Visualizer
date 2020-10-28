#include <glad/glad.h>
#include <Shader.h>
#include <perlin.h>
#include <Camera.h>

typedef struct {
    GLfloat x,y,z,w;
} vec4;



class GpuParticle
{
public:
    GpuParticle(int numParticles, int width, int height, Shader* particleShader);
    ~GpuParticle();

    void update(float delta);
    void render();

    void reset();

    void reloadShader(GLuint* programID);

    void reload(std::string x, std::string y, std::string z);

    void setParticleCount(int count) { NUM_PARTICLES = count; };
    void setParticleLifetime(float lifetime) { pLifetime = lifetime; };
    void setVecFieldSize(glm::vec3 size) { mVecFieldSize = size; };
    void setVecFieldPosition(glm::vec3 pos) { mVecFieldPos = pos; };
    void setSpeedMultiplier(float speed) { mSpeedMultiplier = speed; };

    void loadComputeShaderTemplate();

    static std::string computeShaderTemplate;

private:
    int NUM_PARTICLES, mWidth, mHeight;

    float mSpeedMultiplier = 2.0f;

    void generateVecField();
    
    glm::vec3 mVecFieldPos = {0, 0, 0};
    GLuint initialPositions, particlePositions, vectorField;
    //Shader* cShader;
    Shader* pShader;

    std::string xExpression = "0";
    std::string yExpression = "0";
    std::string zExpression = "0";
    float pLifetime = 10;

    glm::vec3 mVecFieldSize = glm::vec3(5, 5, 5);


    vec4 *initialData = nullptr;

    GLuint mComputeShader = 0;


};
