#include "render/OpenGL/ShaderOpenGL.h"

#include "render/VertexFormat.h"
#include "render/OpenGL/ErrorCheckingOpenGL.h"

#include "system/Logger.h"

#include <vector>
using namespace std;

namespace Sketch3D {

ShaderOpenGL::ShaderOpenGL()
    : shader_(0)
{
}

ShaderOpenGL::~ShaderOpenGL()
{
    if (shader_ > 0)
    {
        GL_CALL( glDeleteProgram(shader_) );
    }
}

GLuint ShaderOpenGL::GetShader() const
{
    return shader_;
}

bool ShaderOpenGL::ValidateShader() const
{
    GLint length = 0;
    GL_CALL( glGetProgramiv(shader_, GL_INFO_LOG_LENGTH, &length) );

	if (length > 1)
    {
		char* infoLog = new char[length + 1];
		int charsWritten = 0;
		GL_CALL( glGetProgramInfoLog(shader_, length, &charsWritten, infoLog) );
		Logger::GetInstance()->Error(string(infoLog));
		delete[] infoLog;

        return false;
	}

    return true;
}

bool FragmentShaderOpenGL::InitializeFromSource(const string& source)
{
    if (shader_ > 0)
    {
        Logger::GetInstance()->Warning("Trying to initialize an already initialized fragment shader");
        return false;
    }

    const char* sourcePtr = source.c_str();
    GL_CALL( shader_ = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &sourcePtr) );

    if (!ValidateShader())
    {
        Logger::GetInstance()->Error("Fragment shader creation failed");
        return false;
    }

    return true;
}

VertexShaderOpenGL::VertexShaderOpenGL()
    : vertexFormat_(nullptr)
{
}

bool VertexShaderOpenGL::InitializeFromSource(const string& source)
{
    if (shader_ > 0)
    {
        Logger::GetInstance()->Warning("Trying to initialize an already initialized vertex shader");
        return false;
    }

    const char* sourcePtr = source.c_str();
    GL_CALL( shader_ = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &sourcePtr) );

    if (!ValidateShader())
    {
        Logger::GetInstance()->Error("Vertex shader creation failed");
        return false;
    }

    return true;
}

bool VertexShaderOpenGL::CreateInputLayout(VertexFormat* vertexFormat)
{
    if (shader_ == 0) {
        Logger::GetInstance()->Warning("You have to create the vertex shader before creating the input layout");
        return false;
    }

    vertexFormat_ = vertexFormat;

    return true;
}

VertexFormat* VertexShaderOpenGL::GetVertexFormat() const
{
    return vertexFormat_;
}

}