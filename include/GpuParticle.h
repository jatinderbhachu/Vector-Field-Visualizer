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
    void setPosition(glm::vec3 pos) { mPosition = pos; };
    glm::vec3 getPosition() const { return mPosition; };

    void reset();

    void reloadShader(GLuint* programID);

    void reload(std::string x, std::string y, std::string z);

    void loadComputeShaderTemplate();

    static std::string computeShaderTemplate;

private:
    int NUM_PARTICLES, mWidth, mHeight;
    glm::vec3 mPosition = {0, 0, 0};
    GLuint initialPositions, particlePositions, vectorField;
    //Shader* cShader;
    Shader* pShader;

    std::string xExpression = "0";
    std::string yExpression = "0";
    std::string zExpression = "0";


    vec4 *initialData;

    GLuint mComputeShader = 0;


};
