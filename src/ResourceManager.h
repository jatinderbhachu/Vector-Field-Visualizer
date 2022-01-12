#pragma once

#include <unordered_map>
#include <vector>

class Shader;
class Texture;

class ResourceManager {
    public:
        static void LoadShader(std::string name, std::string vert, std::string frag);
        static void LoadComputeShader(std::string name, std::string compute);
        static void LoadTexture(std::string name, std::string file);

        static Shader* GetShader(std::string name);
        static Shader* GetComputeShader(std::string name);
        static Texture* GetTexture(std::string name);
        static Texture* GetRandTexture();

        static void Clear();

    private:
        static std::unordered_map<std::string, Texture*> Textures;
        static std::unordered_map<std::string, Shader*> Shaders;
        static std::unordered_map<std::string, Shader*> ComputeShaders;
        static std::vector<std::string> textureNames;

};

