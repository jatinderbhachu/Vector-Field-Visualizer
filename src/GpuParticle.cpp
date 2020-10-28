#include <GpuParticle.h>
#include <stdio.h>
#include <string.h>

std::string GpuParticle::computeShaderTemplate = std::string("");

void GpuParticle::loadComputeShaderTemplate() {
    std::string ComputeShaderCode;
    std::ifstream VertexShaderStream("./resources/shaders/computeShaderTemplate.cs", std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream, Line)) {
            ComputeShaderCode += "\n" + Line;
        }
        VertexShaderStream.close();
    }else{
        printf("Can't open compute shader template.\n");
        return;
    }

    //GpuParticle::computeShaderTemplate = ComputeShaderCode.c_str();

    GpuParticle::computeShaderTemplate = ComputeShaderCode;

    //strcpy(GpuParticle::computeShaderTemplate, ComputeShaderCode.c_str());
    //strcpy_s(&GpuParticle::computeShaderTemplate, ComputeShaderCode.c_str());
}

void GpuParticle::reload(std::string x, std::string y, std::string z) {
    xExpression = x;
    yExpression = y;
    zExpression = z;
    reloadShader(&mComputeShader);
    generateVecField();
    reset();
}

void GpuParticle::reloadShader(GLuint* programID) {

    GLuint compShader = glCreateShader(GL_COMPUTE_SHADER);

    char* formattedShader = new char[(
                computeShaderTemplate.size() 
                + xExpression.size()
                + yExpression.size() 
                + zExpression.size()) ];

    // add the x y z equations to the compute shader tempalte
    sprintf(formattedShader, computeShaderTemplate.c_str(), xExpression.c_str(), yExpression.c_str(), zExpression.c_str());

    // error logging
    GLint Result = GL_FALSE;
    int InfoLogLength;

    printf("Compiling Compute shader\n");
    glShaderSource(compShader, 1, &formattedShader, nullptr);
    glCompileShader(compShader);

    // Check Compute Shader
    glGetShaderiv(compShader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(compShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error in compiling compute shader\n");
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(compShader, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s", &FragmentShaderErrorMessage[0]);
    }


    GLuint computeShaderP = glCreateProgram();
    glAttachShader(computeShaderP, compShader);
    glLinkProgram(computeShaderP);

    printf("Linking Compute Program\n");
    glGetProgramiv(computeShaderP, GL_LINK_STATUS, &Result);
    glGetProgramiv(computeShaderP, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error in linking compute shader\n");
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(compShader, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s", &FragmentShaderErrorMessage[0]);
    }

    // free memory
    delete [] formattedShader;
    glad_glDeleteShader(compShader);

    glDeleteProgram(*programID);

    *programID = computeShaderP;

}

// TODO: set max lifetime of particle system, when lifetime reaches 0, destroy particle system
GpuParticle::GpuParticle(int numParticles, int width, int height, Shader* particleShader) : NUM_PARTICLES(numParticles), mWidth(width), mHeight(height), pShader(particleShader)
{

    loadComputeShaderTemplate();
    //reloadShader(&mComputeShader);


    glGenBuffers(1, &initialPositions);
    glGenBuffers(1, &particlePositions);
    glGenBuffers(1, &vectorField);

    generateVecField();
}

void GpuParticle::generateVecField() {
    // free the prev data
    if(initialData != nullptr) delete [] initialData;

    initialData = new vec4[NUM_PARTICLES];
    vec4 *particleData = new vec4[NUM_PARTICLES];
    vec4 *vectorFieldData = new vec4[NUM_PARTICLES];

    float cubeSize = std::cbrt(NUM_PARTICLES);
    float increment = mVecFieldSize.x/cubeSize;
    if(increment < 0.5) increment = 0.5;

    float x = -(mVecFieldSize.x)/2;
    float y = -(mVecFieldSize.y)/2;
    float z = -(mVecFieldSize.z)/2;

    for (int i = 0; i < NUM_PARTICLES; i++){
        float lifetime = (rand() / (double)RAND_MAX)*pLifetime;
        vec4 spawn;
        spawn.x = x + mVecFieldPos.x;
        spawn.y = y + mVecFieldPos.y;
        spawn.z = z + mVecFieldPos.z;
        spawn.w = lifetime;


        initialData[i] = spawn;
        particleData[i] = spawn;
        vectorFieldData[i].x = 0.0;
        vectorFieldData[i].y = 0.0;
        vectorFieldData[i].z = 0.0;
        vectorFieldData[i].w = 0.0;

        x+=increment;
        if(x >= (mVecFieldSize.x)/2){
            y+=increment;
            x = -(mVecFieldSize.x)/2;
        }
        if(y >= (mVecFieldSize.y)/2){
            y = -(mVecFieldSize.y)/2;
            z+=increment;
        }
        if(z >= (mVecFieldSize.z)/2){
            z = -(mVecFieldSize.z)/2;
        }

    }


    glBindBuffer(GL_ARRAY_BUFFER, initialPositions);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * sizeof(vec4), initialData, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, particlePositions);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * sizeof(vec4), particleData, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ARRAY_BUFFER, vectorField);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * sizeof(vec4), vectorFieldData, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete [] particleData;
    delete [] vectorFieldData;
}

GpuParticle::~GpuParticle(){
    glBindBuffer(GL_ARRAY_BUFFER, initialPositions);
    glDeleteBuffers(1, &initialPositions);
    glBindBuffer(GL_ARRAY_BUFFER, particlePositions);
    glDeleteBuffers(1, &particlePositions);
    glBindBuffer(GL_ARRAY_BUFFER, vectorField);
    glDeleteBuffers(1, &vectorField);

    delete [] initialData;
}

void GpuParticle::reset() {
    glBindBuffer(GL_ARRAY_BUFFER, particlePositions);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * sizeof(vec4), initialData, GL_DYNAMIC_COPY);

}

void GpuParticle::update(float delta)
{
    glUseProgram(mComputeShader);

    glUniform1f(glGetUniformLocation(mComputeShader, "timestep"), delta);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, initialPositions);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, particlePositions);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vectorField);

    glDispatchCompute((NUM_PARTICLES/128) + 1, 1, 1);

    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void GpuParticle::render()
{

    pShader->use();
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, mVecFieldPos);
    glm::mat4 MVP = Camera::_projectionMatrix * Camera::_viewMatrix * modelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(pShader->programID(), "mvp"),  1, GL_FALSE, glm::value_ptr(MVP));

    glBindBuffer (GL_ARRAY_BUFFER, particlePositions);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);

    glBindBuffer (GL_ARRAY_BUFFER, vectorField);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);

    glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

}
