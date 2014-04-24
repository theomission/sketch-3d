#ifndef SKETCH_3D_RENDER_SYSTEM_OPENGL_H
#define SKETCH_3D_RENDER_SYSTEM_OPENGL_H

#include "render/RenderSystem.h"

namespace Sketch3D {

// Forward declaration
class RenderContextOpenGL;

/**
 * @class RenderSystem
 * OpenGL implementation of the render system
 */
class RenderSystemOpenGL : public RenderSystem {
	public:
		RenderSystemOpenGL(WindowHandle windowHandle, unsigned int width, unsigned int height, bool windowed);
		~RenderSystemOpenGL();
		virtual bool Initialize();
		virtual void SetClearColor(float red, float green, float blue, float alpha=1.0f);
		virtual bool BeginRender();
		virtual void EndRender();
		virtual void Render();

	public:
		RenderContextOpenGL*	renderContext_;	/**< The render context to create for OpenGL */
};

}

#endif