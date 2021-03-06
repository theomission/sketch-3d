#ifndef SKETCH_3D_RENDERER_H
#define SKETCH_3D_RENDERER_H

#include "math/Matrix4x4.h"
#include "math/Plane.h"
#include "math/Vector3.h"

#include "render/Renderer_Common.h"
#include "render/RenderQueue.h"
#include "render/RenderState.h"
#include "render/SceneTree.h"
#include "render/Texture.h"

#include "system/Platform.h"

#include <string>
using namespace std;

namespace Sketch3D {

// Forward class declaration
class BufferObjectManager;
class RenderStateCache;
class RenderSystem;
class RenderTexture;
class Shader;
class Sphere;
class Texture2D;
class Texture3D;
class Window;

/**
 * @class Renderer
 * This class provides an interface to used the underlying render system based
 * on its implementation (OpenGL or Direct3D). It is through this class that
 * the user can draw things on the screen and modify certain parameters of the
 * rendering context
 */
class SKETCH_3D_API Renderer {
	public:
		/**
		 * Destructor
		 */
						       ~Renderer();

		static Renderer*	    GetInstance();

		/**
		 * Initialize the renderer
		 * @param renderSystem The render system to use
		 * @param window The window for which to create a rendering context
         * @param renderParameters The rendering parameters to use when creating the render context
		 * @return true if the initialization went correctly
		 */
		bool				    Initialize(RenderSystem_t renderSystem,
									       Window& window, const RenderParameters_t& renderParameters);

		/**
		 * Change the clear color
		 * @param red The red component of the color
		 * @param green The green component of the color
		 * @param blue The blue component of the color
		 * @param alpha The alpha component of the color
		 */
		void				    SetClearColor(float red, float green, float blue, float alpha=1.0f) const;

		/**
		 * Clears the current framebuffer
         * @param buffer Which buffers to clear
		 */
		void				    Clear(int buffer=(CLEAR_BUFFER_COLOR|CLEAR_BUFFER_DEPTH)) const;

        /**
         * Starts the rendering process
         */
        void                    StartRender();

		/**
		 * End the rendering process
		 */
		void				    EndRender();

		/**
		 * The actual rendering
		 */
		void				    Render();

        /**
         * Presents the drawn frame to the front buffer
         */
        void                    PresentFrame();

		/**
		 * Set an orthogonal projection. This replace the current projection matrix
		 * @param left The left position of the viewing volume
		 * @param right The right position of the viewing volume
		 * @param bottom The bottom position of the viewing volume
		 * @param top The top position of the viewing volume
		 * @param nearPlane The near position of the viewing volume
		 * @param farPlane The far position of the viewing volume
		 */
		void				    OrthoProjection(float left, float right, float bottom,
											    float top, float nearPlane, float farPlane);

		/**
		 * Set a perspective projection. This replace the current projection matrix
		 * @param left The left position of the viewing volume
		 * @param right The right position of the viewing volume
		 * @param bottom The bottom position of the viewing volume
		 * @param top The top position of the viewing volume
		 * @param nearPlane The near position of the viewing volume
		 * @param farPlane The far position of the viewing volume
		 */
		void				    PerspectiveProjection(float left, float right, float bottom,
												      float top, float nearPlane, float farPlane);

		/**
		 * Set a perspective projection. This replace the current projection matrix
		 * @param fov The field of view in degrees in the y direction
		 * @param aspect The aspect ration
		 * @param nearPlane The near position of the viewing volume
		 * @param farPlane The far position of the viewing volume
		 */
		void				    PerspectiveProjection(float fov, float aspect,
												      float nearPlane, float farPlane);

		/**
		 * Set a new camera position, looking direction and up vector
		 * @param position The position of the camera
		 * @param point The point to look at
		 * @param up The up vector to use for the camera. UP by default
		 */
		void				    CameraLookAt(const Vector3& position,
										     const Vector3& point,
										     const Vector3& up=Vector3::UP);

        /**
         * Set the view matrix with a right, up, look vector and position. The right, up and look vector are
         * assumed to be orthonormalized.
         * @param right The right vector of the view matrix
         * @param up The up vector of the view matrix
         * @param look The look vector of the view matrix
         * @param position Position in view matrix
         */
        void                    SetViewMatrix(const Vector3& right, const Vector3& up, const Vector3& look,
                                              const Vector3& position);

		/**
		 * Set the renderer's fill mode
		 * @param mode The mode to use for rendering the geometry
		 */
		void				    SetRenderFillMode(RenderMode_t mode) const;

        /**
         * Sets the camera viewport dimension
         * @param x The left position of the viewport
         * @param y The upper position of the viewport
         * @param width The width of the viewport
         * @param height The height of the viewport
         */
        void                    SetViewport(size_t x, size_t y, size_t width, size_t height);

        /**
         * Enable or disable depth testing
         * @param val Enabled if true, disabled otherwise
         */
        void                    EnableDepthTest(bool val) const;

        /**
         * Enable or disable depth writing. This will only work if depth testing is enabled
         * @param val Enabled if true, disabled otherwise
         */
        void                    EnableDepthWrite(bool val) const;

        /**
         * Enable or disable color writing.
         * @param val Enabled if true, disabled otherwise
         */
        void                    EnableColorWrite(bool val) const;

        /**
         * Sets the depth compare function to use
         * @param comparison The comparison function to use
         */
        void                    SetDepthComparisonFunc(DepthFunc_t comparison) const;

        /**
         * Select the culling method
         * @param cullingMethod The culling method to use
         */
        void                    SetCullingMethod(CullingMethod_t cullingMethod);

        /**
         * Create a shader object. The user doesn't have to free the memory returned, the Renderer will take
         * care of it.
         * @return A pointer to a shader
         */
        Shader*                 CreateShader() const;

        /**
         * Create a 2D texture object. The user have to free the memory returned, the Renderer will not take
         * care of it.
         * @return A pointer to a 2D texture
         */
        Texture2D*              CreateTexture2D() const;

        /**
         * Create a 2D texture object from a file. The user doesn't have to free the memory returned, the Renderer will take
         * care of it.
         * @param generateMipmaps If set to true, generate mipmaps for this texture
         * @return A pointer to a 2D texture
         */
        Texture2D*              CreateTexture2DFromFile(const string& filename, bool generateMipmaps=false) const;

        /**
         * Create a 3D texture object. The user have to free the memory returned, the Renderer will not take
         * care of it.
         * @return A pointer to a 3D texture
         */
        Texture3D*              CreateTexture3D() const;

        /**
         * Create a render texture. The user doesn't have to free the memory returned, the Renderer will take
         * care of it.
         * @param width The width of the render texture
         * @param height The height of the render texture
         * @param format The format of the render texture
         * @return A pointer to a render texture
         */
        RenderTexture*          CreateRenderTexture(unsigned int width, unsigned int height, TextureFormat_t format) const;

        /**
         * Binds the render system frame buffer that is used to draw on the screen
         */
        void                    BindScreenBuffer() const;

        /**
         * Enable blending
         * @param val if true, enables blending, deactivate it otherwise
         */
        void                    EnableBlending(bool val) const;

        /**
         * Specifies the blending equation to use
         */
        void                    SetBlendingEquation(BlendingEquation_t equation) const;

        /**
         * Specifies the blending factor to use
         * @param srcFactor The factor to use for the source pixel (the pixel being processed)
         * @param dstFactor the factor to use for the destination pixel (the pixel already present in the framebuffer)
         */
        void                    SetBlendingFactor(BlendingFactor_t srcFactor, BlendingFactor_t dstFactor) const;

        /**
         * Convert a point in screen space into world space. This function uses the current perspective and view matrix
         * @param screenPoint A vector representing the point in screen space
         * @return A vector representing the point in world space
         */
        Vector3                 ScreenToWorldPoint(const Vector2& point) const;

        /**
         * Bind a given texture to a texture unit. This function chooses a texture unit by using
         * a Least Recently Used (LRU) cache.
         * @param texture The texture to cache
         * @return The texture unit to which the texture has been bound to
         */
        size_t                  BindTexture(const Texture* texture);

        /**
         * Bind the specified shader to be the active one.
         * @param shader The shader to activate. If set to nullptr, then it will unbind the current shader
         */
        void                    BindShader(const Shader* shader);

        /**
         * Extract the frustum planes from the view projection matrix
         * @return A FrustumPlanes_t object, containing the 6 view frustum planes
         */
        FrustumPlanes_t         ExtractViewFrustumPlanes() const;

        /**
         * Enable frustum culling of the processed nodes
         * @param val If true, the culling will be enabled and disabled if false
         */
        void                    EnableFrustumCulling(bool bal);

        /**
         * Draw the content of a buffer object using a shader made for drawing text
         * @param bufferObject The buffer object to draw
         * @param fontAtlas A texture atlas representing the font that is going to be used
         * @parma textColor The color of the text been rendered
         */
        void                    DrawTextBuffer(BufferObject* bufferObject, Texture2D* fontAtlas, const Vector3& textColor);

		const Matrix4x4&	    GetProjectionMatrix() const;
		const Matrix4x4&	    GetViewMatrix() const;
		const Matrix4x4&	    GetViewProjectionMatrix() const;
        float                   GetNearFrustumPlane() const;
        float                   GetFarFrustumPlane() const;

		const SceneTree&	    GetSceneTree() const;
		SceneTree&			    GetSceneTree();

        BufferObjectManager*    GetBufferObjectManager() const;
        RenderStateCache*       GetRenderStateCache() const;

        size_t                  GetScreenWidth() const;
        size_t                  GetScreenHeight() const;
        CullingMethod_t         GetCullingMethod() const;

	private:
		static Renderer		    instance_;	            /**< Singleton instance of this class */

		RenderSystem*		    renderSystem_;		    /**< The render system that is currently being used */

		Matrix4x4			    projection_;		    /**< The projection matrix */
		Matrix4x4			    view_;				    /**< The view matrix */
		Matrix4x4			    viewProjection_;	    /**< Pre-computed view-projection matrix */
        float                   nearFrustumPlane_;      /**< The near plane of the view frustum */
        float                   farFrustumPlane_;       /**< The far plane of the view frustum */

		SceneTree			    sceneTree_;			    /**< The scene tree to render */

        BufferObjectManager*    bufferObjectManager_;   /**< Buffer object manager used to create the buffer objects */

        RenderQueue             opaqueRenderQueue_;     /**< The render queue used for drawing opaque objects */
        RenderQueue             transparentRenderQueue_;/**< The render queue used for drawing transparent objects */
        bool                    useFrustumCulling_;     /**< If set to true, frustum culling will be used */
        RenderParameters_t      renderParamters_;       /**< The rendering parameters used when creating the rendering context */

        // Old viewport data
        size_t                  oldViewportX_;
        size_t                  oldViewportY_;
        size_t                  oldViewportWidth_;
        size_t                  oldViewportHeight_;

        CullingMethod_t         cullingMethod_;         /**< Current culling method */

        /**
         * Initializes some default values
         */
        void                    SetDefaultRenderingValues();

		/**
		 * Constructor
		 */
							    Renderer();

		/**
		 * Copy-constructor - here only to disallow copy
		 */
							    Renderer(const Renderer& src);

		/**
		 * Assignment operator - here only to disallow assignment
		 */
							    Renderer& operator=(const Renderer& rhs);
};

}

#endif
