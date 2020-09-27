#include <ResourceManager.h>

std::map<std::string, Texture*>  ResourceManager::Textures;
std::map<std::string, Shader*>   ResourceManager::Shaders;
std::map<std::string, Shader*> ResourceManager::ComputeShaders;
std::vector<std::string> ResourceManager::textureNames;


void ResourceManager::LoadShader(std::string name, std::string vert, std::string frag){
    std::string vertPath = SHADERPATH+vert;
    std::string fragPath = SHADERPATH+frag;
    Shader* shader = new Shader();
    shader->loadShaders(vertPath.c_str(), fragPath.c_str());
    Shaders[name] = shader;
}

void ResourceManager::LoadComputeShader(std::string name, std::string compute){
    std::string computeShaderPath = SHADERPATH+compute;
    Shader* shader = new Shader();
    shader->loadComputeShader(computeShaderPath.c_str());
    ComputeShaders[name] = shader;
}

void ResourceManager::LoadTexture(std::string name, std::string file){
    std::string texPath = TEXTUREPATH+file;
    Texture* texture = new Texture();
    textureNames.push_back(name);
    texture->loadTexture(texPath.c_str());
    printf("Loading texture %s, texID: %u\n", texPath.c_str(), texture->mTexture);
    Textures[name] = texture;
}

Shader* ResourceManager::GetShader(std::string name){
    return Shaders[name];
}

Shader* ResourceManager::GetComputeShader(std::string name){
    return ComputeShaders[name];
}

Texture* ResourceManager::GetTexture(std::string name){
    return Textures[name];
}

Texture* ResourceManager::GetRandTexture(){
    const char* texName = textureNames[(int)ceil(rand() % textureNames.size())].c_str();
    return Textures[texName];
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders	
    for (auto iter : Shaders){
        printf("[DELETING PROGRAM]: %s\n", iter.first.c_str());
        iter.second->use();
        glDeleteProgram(iter.second->programID());
        delete iter.second;
    }

    for (auto iter : ComputeShaders){
        printf("[DELETING PROGRAM]: %s\n", iter.first.c_str());
        iter.second->use();
        glDeleteProgram(iter.second->programID());
        delete iter.second;
    }

    // (Properly) delete all textures
    for (auto iter : Textures){
        glDeleteTextures(1, &iter.second->mTexture);
        delete iter.second;
    }
}
