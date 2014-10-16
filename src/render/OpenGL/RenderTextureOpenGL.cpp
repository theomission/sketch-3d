#include "render/OpenGL/RenderTextureOpenGL.h"

#include "render/OpenGL/Texture2DOpenGL.h"
#include "system/Logger.h"

#include "render/OpenGL/gl/glew.h"


namespace Sketch3D {

RenderTextureOpenGL::RenderTextureOpenGL(unsigned int width, unsigned int height, TextureFormat_t format) :
        RenderTexture(width, height, format), framebuffer_(0), renderbuffer_(0), depthBufferBound_(false), texturesAttached_(false)
{
}

RenderTextureOpenGL::~RenderTextureOpenGL() {
    if (renderbuffer_ != 0) {
        glDeleteRenderbuffers(1, &renderbuffer_);
    }

    if (framebuffer_ != 0) {
        glDeleteFramebuffers(1, &framebuffer_);
    }
}

bool RenderTextureOpenGL::AddDepthBuffer() {   
    if (depthBufferBound_) {
        return false;
    } else if (renderbuffer_ != 0) {
        return true;
    }

    if (framebuffer_ == 0) {
        glGenFramebuffers(1, &framebuffer_);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glGenRenderbuffers(1, &renderbuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

bool RenderTextureOpenGL::AttachTextureToDepthBuffer(Texture2D* texture) {
    if (depthBufferBound_) {
        return true;
    } else if (renderbuffer_ != 0) {
        return false;
    }

    if (texture->GetWidth() != width_ || texture->GetHeight() != height_) {
        Logger::GetInstance()->Error("Texture used as attachment for the depth buffer isn't of the same size as the render texture");
        return false;
    }

    if (framebuffer_ == 0) {
        glGenFramebuffers(1, &framebuffer_);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dynamic_cast<Texture2DOpenGL*>(texture)->textureName_, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Logger::GetInstance()->Info("Couldn't attach texture to depth buffer");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    depthBufferBound_ = true;
    Logger::GetInstance()->Info("Texture successfully attached to depth buffer");

    return true;
}

bool RenderTextureOpenGL::AttachTextures(const vector<Texture2D*>& textures) {
    if (texturesAttached_) {
        return true;
    }

    vector<GLuint> textureNames;
    textures_ = textures;
    for (size_t i = 0; i < textures_.size(); i++) {
        Texture2DOpenGL* texture = dynamic_cast<Texture2DOpenGL*>(textures_[i]);

        if (texture != nullptr) {
            if (texture->width_ != width_) {
                Logger::GetInstance()->Error("Couldn't create render texture, texture #" + to_string(i) + " doesn't have the appropriate width.");
                return false;
            }

            if (texture->height_ != height_) {
                Logger::GetInstance()->Error("Couldn't create render texture, texture #" + to_string(i) + " doesn't have the appropriate height.");
                return false;
            }

            if (texture->format_ != format_) {
                Logger::GetInstance()->Error("Couldn't create render texture, texture #" + to_string(i) + " doesn't have the appropriate format.");
                return false;
            }

            textureNames.push_back(texture->textureName_);
        } else {
            Logger::GetInstance()->Error("Couldn't create render texture, texture #" + to_string(i) + " is null.");
            return false;
        }
    }

    if (framebuffer_ == 0) {
        glGenFramebuffers(1, &framebuffer_);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    for (size_t i = 0; i < textureNames.size(); i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureNames[i], 0);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        texturesAttached_ = true;
        Logger::GetInstance()->Info("Render texture #" + to_string(framebuffer_) + " was succesfully created");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return true;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &framebuffer_);
    framebuffer_ = 0;

    Logger::GetInstance()->Error("Couldn't create render texture, frame buffer is not complete");

    return false;
}

void RenderTextureOpenGL::Bind() const {
    if (framebuffer_ == 0 && renderbuffer_ == 0) {
        return;
    }

    if (framebuffer_ != 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

        size_t numBuffers = textures_.size() + depthBufferBound_;
        GLenum* buffers = new GLenum[numBuffers];
        if (depthBufferBound_) {
            buffers[0] = GL_NONE;
        }

        for (size_t i = depthBufferBound_; i < numBuffers; i++) {
            buffers[i] = GL_COLOR_ATTACHMENT0 + (i - depthBufferBound_);
        }
        glDrawBuffers(numBuffers, buffers);
        delete[] buffers;

        glViewport(0, 0, width_, height_);
    }
}

}