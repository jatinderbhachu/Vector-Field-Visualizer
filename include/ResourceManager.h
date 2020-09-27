#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <Texture.h>
#include <Shader.h>
#include <config.h>

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

        // fonts, audio, text, levels etc ...

    private:
        ResourceManager() {  }
        static std::map<std::string, Texture*> Textures;
        static std::map<std::string, Shader*> Shaders;
        static std::map<std::string, Shader*> ComputeShaders;
        static std::vector<std::string> textureNames;

};

