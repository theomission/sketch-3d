#include "render/OpenGL/RenderSystemOpenGL.h"

#include "render/OpenGL/gl/glew.h"
#include "render/OpenGL/gl/gl.h"

#include "render/OpenGL/BufferObjectManagerOpenGL.h"
#include "render/OpenGL/RenderTextureOpenGL.h"
#include "render/OpenGL/ShaderOpenGL.h"
#include "render/OpenGL/Texture2DOpenGL.h"
#include "render/OpenGL/Texture3DOpenGL.h"

#include "system/Logger.h"
#include "system/Window.h"

#if PLATFORM == PLATFORM_WIN32
#include "render/OpenGL/Win32/RenderContextOpenGLWin32.h"
#elif PLATFORM == PLATFORM_LINUX
#include "render/OpenGL/Unix/RenderContextOpenGLUnix.h"
#endif

namespace Sketch3D {

RenderSystemOpenGL::RenderSystemOpenGL(Window& window) : RenderSystem(window),
                                                         renderContext_(NULL)
{
	Logger::GetInstance()->Info("Current rendering API: OpenGL");
}

RenderSystemOpenGL::~RenderSystemOpenGL() {
	Logger::GetInstance()->Info("Shutdown OpenGL");
	delete renderContext_;
}

bool RenderSystemOpenGL::Initialize() {
	Logger::GetInstance()->Info("Initializing OpenGL...");

#if PLATFORM == PLATFORM_WIN32
	renderContext_ = new RenderContextOpenGLWin32();
#elif PLATFORM == PLATFORM_LINUX
    renderContext_ = new RenderContextOpenGLUnix();
#endif

	if (!renderContext_->Initialize(window_)) {
		Logger::GetInstance()->Error("Couldn't create OpenGL context");
		return false;
	}

	QueryDeviceCapabilities();

	// Some initial values
	glViewport(0, 0, width_, height_);

	Renderer::GetInstance()->PerspectiveProjection(45.0f, (float)width_ / (float)height_, 1.0f, 1000.0f);
    Renderer::GetInstance()->CameraLookAt(Vector3::ZERO, Vector3::LOOK);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    SetRenderFillMode(RenderMode_t::RENDER_MODE_FILL);

    bufferObjectManager_ = new BufferObjectManagerOpenGL;

	return true;
}

void RenderSystemOpenGL::SetClearColor(float red, float green, float blue, float alpha) const {
	glClearColor(red, green, blue, alpha);
}

void RenderSystemOpenGL::Clear(int buffer) const {
    GLuint clearBuffer = 0;
    if ((buffer & CLEAR_BUFFER_COLOR) != 0) {
        clearBuffer |= GL_COLOR_BUFFER_BIT;
    }

    if ((buffer & CLEAR_BUFFER_DEPTH) != 0) {
        clearBuffer |= GL_DEPTH_BUFFER_BIT;
    }

	glClear(clearBuffer);
}

void RenderSystemOpenGL::EndRender() {
	renderContext_->SwapBuffers();
}

void RenderSystemOpenGL::Render() {
	static float angle = 0.0f;
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	angle += 0.02f;
}

void RenderSystemOpenGL::SetRenderFillMode(RenderMode_t mode) const {
	switch (mode) {
		case RENDER_MODE_FILL:
			glPolygonMode(GL_FRONT, GL_FILL);
			break;
		case RENDER_MODE_WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case RENDER_MODE_POINT:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
	}
}

void RenderSystemOpenGL::SetViewport(size_t x, size_t y, size_t width, size_t height) {
    glViewport(x, y, width, height);
}

void RenderSystemOpenGL::EnableDepthTest(bool val) const {
    if (val) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void RenderSystemOpenGL::EnableDepthWrite(bool val) const {
    glDepthMask( (val) ? GL_TRUE : GL_FALSE );
}

void RenderSystemOpenGL::EnableColorWrite(bool val) const {
    if (val) {
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    } else {
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    }
}

void RenderSystemOpenGL::SetDepthComparisonFunc(DepthFunc_t comparison) const {
    GLenum depthFunc;
    switch (comparison) {
        case DEPTH_FUNC_NEVER:
            depthFunc = GL_NEVER;
            break;
        case DEPTH_FUNC_ALWAYS:
            depthFunc = GL_ALWAYS;
            break;
        case DEPTH_FUNC_LESS:
            depthFunc = GL_LESS;
            break;
        case DEPTH_FUNC_LEQUAL:
            depthFunc = GL_LEQUAL;
            break;
        case DEPTH_FUNC_EQUAL:
            depthFunc = GL_EQUAL;
            break;
        case DEPTH_FUNC_GEQUAL:
            depthFunc = GL_GEQUAL;
            break;
        case DEPTH_FUNC_GREATER:
            depthFunc = GL_GREATER;
            break;
        case DEPTH_FUNC_NOTEQUAL:
            depthFunc = GL_NOTEQUAL;
            break;
    }

    glDepthFunc(depthFunc);
}

void RenderSystemOpenGL::SetCullingMethod(CullingMethod_t cullingMethod) const {
    switch (cullingMethod) {
        case CULLING_METHOD_FRONT_FACE:
            glCullFace(GL_FRONT);
            break;
        case CULLING_METHOD_BACK_FACE:
            glCullFace(GL_BACK);
            break;
    }
}

Shader* RenderSystemOpenGL::CreateShader(const string& vertexFilename, const string& fragmentFilename, const vector<string>& vertexInputs) {
    shaders_.push_back(new ShaderOpenGL(vertexFilename, fragmentFilename, vertexInputs));
    return shaders_[shaders_.size() - 1];
}

Texture2D* RenderSystemOpenGL::CreateTexture2D() const {
    return new Texture2DOpenGL;
}

Texture3D* RenderSystemOpenGL::CreateTexture3D() const {
    return new Texture3DOpenGL;
}

RenderTexture* RenderSystemOpenGL::CreateRenderTexture(unsigned int width, unsigned int height, TextureFormat_t format) const {
    return new RenderTextureOpenGL(width, height, format);
}

void RenderSystemOpenGL::BindScreenBuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width_, height_);
}

void RenderSystemOpenGL::EnableBlending(bool val) const {
    if (val) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
}

void RenderSystemOpenGL::SetBlendingEquation(BlendingEquation_t equation) const {
    GLenum blendingEquation;
    switch (equation) {
        case BLENDING_EQUATION_ADD:
            blendingEquation = GL_FUNC_ADD;
            break;
        case BLENDING_EQUATION_SUBTRACT:
            blendingEquation = GL_FUNC_SUBTRACT;
            break;
        case BLENDING_EQUATION_REVERSE_SUBTRACT:
            blendingEquation = GL_FUNC_REVERSE_SUBTRACT;
            break;
        case BLENDING_EQUATION_MIN:
            blendingEquation = GL_MIN;
            break;
        case BLENDING_EQUATION_MAX:
            blendingEquation = GL_MAX;
            break;
    }

    glBlendEquation(blendingEquation);
}

void RenderSystemOpenGL::SetBlendingFactor(BlendingFactor_t srcFactor, BlendingFactor_t dstFactor) const {
    glBlendFunc(GetBlendingFactor(srcFactor), GetBlendingFactor(dstFactor));
}

void RenderSystemOpenGL::BindTexture(const Texture* texture, size_t unit) const {
    TextureType_t textureType = texture->GetType();
    size_t textureName = 0;

    glActiveTexture(GL_TEXTURE0 + unit);
    if (textureType == TEXTURE_TYPE_2D) {
        textureName = dynamic_cast<const Texture2DOpenGL*>(texture)->textureName_;
        glBindTexture(GL_TEXTURE_2D, textureName);
    } else if (textureType == TEXTURE_TYPE_3D) {
        textureName = dynamic_cast<const Texture3DOpenGL*>(texture)->textureName_;
        glBindTexture(GL_TEXTURE_2D, textureName);
    } else {
        Logger::GetInstance()->Error("Invalid texture type for binding!");
        return;
    }
}

void RenderSystemOpenGL::QueryDeviceCapabilities() {
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &deviceCapabilities_.maxActiveTextures_);
}

unsigned int RenderSystemOpenGL::GetBlendingFactor(BlendingFactor_t factor) const {
    unsigned int blendingFactor;
    switch (factor) {
        case BLENDING_FACTOR_ZERO:
            blendingFactor = GL_ZERO;
            break;
        case BLENDING_FACTOR_ONE:
            blendingFactor = GL_ONE;
            break;
        case BLENDING_FACTOR_SRC_COLOR:
            blendingFactor = GL_SRC_COLOR;
            break;
        case BLENDING_FACTOR_ONE_MINUS_SRC_COLOR:
            blendingFactor = GL_ONE_MINUS_SRC_COLOR;
            break;
        case BLENDING_FACTOR_DST_COLOR:
            blendingFactor = GL_DST_COLOR;
            break;
        case BLENDING_FACTOR_ONE_MINUS_DST_COLOR:
            blendingFactor = GL_ONE_MINUS_DST_COLOR;
            break;
        case BLENDING_FACTOR_SRC_ALPHA:
            blendingFactor = GL_SRC_ALPHA;
            break;
        case BLENDING_FACTOR_ONE_MINUS_SRC_ALPHA:
            blendingFactor = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLENDING_FACTOR_DST_ALPHA:
            blendingFactor = GL_DST_ALPHA;
            break;
        case BLENDING_FACTOR_ONE_MINUS_DST_ALPHA:
            blendingFactor = GL_ONE_MINUS_DST_ALPHA;
            break;
        case BLENDING_FACTOR_SRC_ALPHA_SATURATE:
            blendingFactor = GL_SRC_ALPHA_SATURATE;
            break;
    }

    return blendingFactor;
}

}
