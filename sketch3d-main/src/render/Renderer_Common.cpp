#include "render/Renderer_Common.h"

#include "system/Logger.h"
#include "system/Utils.h"

#include <fstream>
#include <string>
#include <vector>
using namespace std;

namespace Sketch3D {

bool ParseConfigFile(const string& filename, ConfigFileAttributes_t& configFileAttributes) {
    // Default values
    configFileAttributes.renderSystem = RenderSystem_t::OPENGL;
    configFileAttributes.displayFormat = DisplayFormat_t::X8R8G8B8;
    configFileAttributes.width = 1024;
    configFileAttributes.height = 768;
    configFileAttributes.refreshRate = 0;
    configFileAttributes.windowed = true;
    configFileAttributes.depthStencilBits = DepthStencilBits_t::D32;

    ifstream configFile(filename);
    if (!configFile.is_open()) {
        Logger::GetInstance()->Error("Couldn't open config file: " + filename);
        return false;
    }

    while (!configFile.eof()) {
        string line;
        getline(configFile, line);
        if (line[0] == '#') {
            continue;
        }

        vector<string> tokens = Tokenize(line, '=');

        // Remove all white spaces
        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (tokens[i] == " ") {
                tokens.erase(tokens.begin() + i);
            }
        }

        const string& attributes = tokens.front();
        const string& value = tokens.back();
        if (attributes == "[RenderSystem]") {
            if (value == "OpenGL") {
                configFileAttributes.renderSystem = RenderSystem_t::OPENGL;
            } else if (value == "Direct3D9") {
                configFileAttributes.renderSystem = RenderSystem_t::DIRECT3D9;
            } else {
                Logger::GetInstance()->Warning("Unsupported render system: " + value);
            }
        } else if (attributes == "[Width]") {
            configFileAttributes.width = (size_t)atoi(value.c_str());
        } else if (attributes == "[Height]") {
            configFileAttributes.height = (size_t)atoi(value.c_str());
        } else if (attributes == "[Windowed]") {
            if (value == "True") {
                configFileAttributes.windowed = true;
            } else if (value == "False") {
                configFileAttributes.windowed = false;
            } else {
                Logger::GetInstance()->Warning("Unrecognized parameter for \"[Windowed]\": " + value + " Defaulting to windowed mode");
                configFileAttributes.windowed = true;
            }
        } else if (attributes == "[DisplayFormat]") {
            if (value == "A1R5G5B5") {
                configFileAttributes.displayFormat = DisplayFormat_t::A1R5G5B5;
            } else if (value == "A2R10G10B10") {
                configFileAttributes.displayFormat = DisplayFormat_t::A2R10G10B10;
            } else if (value == "A8R8G8B8") {
                configFileAttributes.displayFormat = DisplayFormat_t::A8R8G8B8;
            } else if (value == "R5G6B5") {
                configFileAttributes.displayFormat = DisplayFormat_t::R5G6B5;
            } else if (value == "X8R8G8B8") {
                configFileAttributes.displayFormat = DisplayFormat_t::X8R8G8B8;
            } else {
                Logger::GetInstance()->Warning("Unrecognized parameter for \"[DisplayFormat]\": " + value + " Defaulting to X8R8G8B8");
                configFileAttributes.displayFormat = DisplayFormat_t::X8R8G8B8;
            }
        } else if (attributes == "[RefreshRate]") {
            configFileAttributes.refreshRate = (size_t)atoi(value.c_str());
        } else if (attributes == "[DepthStencilBits]") {
            if (value == "D16") {
                configFileAttributes.depthStencilBits = DepthStencilBits_t::D16;
            } else if (value == "D24S8") {
                configFileAttributes.depthStencilBits = DepthStencilBits_t::D24S8;
            } else if (value == "D32") {
                configFileAttributes.depthStencilBits = DepthStencilBits_t::D32;
            } else {
                Logger::GetInstance()->Warning("Unrecognized parameter for \"[DepthStencilBits]\": " + value + " Defaulting to D32");
                configFileAttributes.depthStencilBits = DepthStencilBits_t::D32;
            }
        } else {
            Logger::GetInstance()->Warning("Unknown config attributes: " + attributes);
        }
    }

    Logger::GetInstance()->Info("Successfully parsed initialize file: " + filename);

    return true;
}

}