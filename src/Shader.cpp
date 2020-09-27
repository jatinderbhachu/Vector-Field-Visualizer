#include <Shader.h>

Shader::Shader(){
    _programID =     -1;
}

Shader::~Shader()
{
    glDeleteProgram(_programID);
}


GLuint Shader::loadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream, Line)) {
            VertexShaderCode += "\n" + Line;
        }
        VertexShaderStream.close();
    }else{
        printf("Can't open %s\n", vertex_file_path);
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line)) {
            FragmentShaderCode += "\n" + Line;
        }
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s", &VertexShaderErrorMessage[0]);
    }


    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , nullptr);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s", &FragmentShaderErrorMessage[0]);
    }


    // Link the program

    printf("Linking program\n");
    _programID = glCreateProgram();
    glAttachShader(_programID, VertexShaderID);
    glAttachShader(_programID, FragmentShaderID);
    glLinkProgram(_programID);

    // Check the program
    glGetProgramiv(_programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(_programID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s", &ProgramErrorMessage[0]);
    }

    glDetachShader(_programID, VertexShaderID);
    glDetachShader(_programID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return _programID;
}

GLuint Shader::loadComputeShader(const char* computeShaderPath)
{
    GLuint compShader = glCreateShader(GL_COMPUTE_SHADER);

    // error logging
    GLint Result = GL_FALSE;
    int InfoLogLength;

    std::string ComputeShaderCode;
    std::ifstream VertexShaderStream(computeShaderPath, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream, Line)) {
            ComputeShaderCode += "\n" + Line;
        }
        VertexShaderStream.close();
    }else{
        printf("Can't open %s\n", computeShaderPath);
        return 0;
    }

    const char* compShaderPtr = ComputeShaderCode.c_str();

    printf("Compiling Compute shader : %s\n", computeShaderPath);
    glShaderSource(compShader, 1, &compShaderPtr, nullptr);
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


    _programID = glCreateProgram();
    glAttachShader(_programID, compShader);
    glLinkProgram(_programID);

    printf("Linking Compute Program: %s\n", computeShaderPath);
    glGetProgramiv(_programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error in linking compute shader\n");
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(compShader, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s", &FragmentShaderErrorMessage[0]);
    }

    return _programID;

}
