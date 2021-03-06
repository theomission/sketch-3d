#ifndef SKETCH_3D_RENDER_SYSTEM_H
#define SKETCH_3D_RENDER_SYSTEM_H

#include "render/Renderer.h"

#include "math/Matrix4x4.h"
#include "math/Vector3.h"

#include "system/Platform.h"
#include "system/Window.h"
#include "system/WindowHandle.h"

#include <map>
#include <vector>
using namespace std;

namespace Sketch3D {

// Forward declaration
class BufferObjectManager;
class RenderQueue;
class RenderStateCache;
class RenderTexture;
class Shader;
class Texture2D;
class Texture3D;
class Vector2;

/**
 * @struct DeviceCapabilities_t
 * Return all the device capabilities in a struct
 */
struct DeviceCapabilities_t {
    int     maxActiveTextures_; /**< Maximum number of active textures supported by the GPU */
    int     maxNumberRenderTargets_; /**< Maximum number of render targets that can be used at the same time */
};

/**
 * @interface RenderSystem
 * This class serves as an interface for the underlying implementation of the
 * rendering systems, such as OpenGL and Direct3D.
 *
 * This interface is basically the context of the API.
 */
class SKETCH_3D_API RenderSystem {
	public:
		/**
		 * Constructor. Initializes the underlying API.
         * @param window The window for which we want to create a context for
		 */
										    RenderSystem(Window& window);

		/**
		 * Destructor. Free the underlying API
		 */
		virtual						       ~RenderSystem();

		/**
		 * Initialize the rendering system
         * @param renderParameters The rendering parameters to use when creating the render context
		 * @return true if the system was correctly initialized, false otherwise
		 */
		virtual bool					    Initialize(const RenderParameters_t& renderParameters)=0;

		/**
		 * Change the clear color
		 * @param red The red component
		 * @param green The green component
		 * @param blue The blue component
		 * @param alpha The alpha component
		 */
		virtual void					    SetClearColor(float red, float green, float blue, float alpha=1.0f) = 0;

		/**
		 * Clears the current framebuffer
         * @param buffer Which buffers to clear
		 */
		virtual void				        Clear(int buffer) const = 0;

        /**
         * Starts the rendering process
         */
        virtual void                        StartRender() = 0;

		/**
		 * Ends the rendering process
		 */
		virtual void					    EndRender() = 0;

        /**
         * Presents the drawn frame to the front buffer
         */
        virtual void                        PresentFrame() = 0;

		/**
		 * Set an orthogonal projection. This replace the current projection matrix
		 * @param left The left position of the viewing volume
		 * @param right The right position of the viewing volume
		 * @param bottom The bottom position of the viewing volume
		 * @param top The top position of the viewing volume
		 * @param nearPlane The near position of the viewing volume
		 * @param farPlane The far position of the viewing volume
		 */
		virtual Matrix4x4                   OrthoProjection(float left, float right, float bottom,
											                float top, float nearPlane, float farPlane) const = 0;

		/**
		 * Set a perspective projection. This replace the current projection matrix
		 * @param left The left position of the viewing volume
		 * @param right The right position of the viewing volume
		 * @param bottom The bottom position of the viewing volume
		 * @param top The top position of the viewing volume
		 * @param nearPlane The near position of the viewing volume
		 * @param farPlane The far position of the viewing volume
		 */
		virtual Matrix4x4                   PerspectiveProjection(float left, float right, float bottom,
												                  float top, float nearPlane, float farPlane) const = 0;

		/**
		 * Set the renderer's fill mode
		 * @param mode The mode to use for rendering the geometry
		 */
		void					            SetRenderFillMode(RenderMode_t mode);

        /**
         * Sets the camera viewport dimension
         * @param x The left position of the viewport
         * @param y The upper position of the viewport
         * @param width The width of the viewport
         * @param height The height of the viewport
         */
        virtual void                        SetViewport(size_t x, size_t y, size_t width, size_t height) = 0;

        /**
         * Enable or disable depth testing
         * @param val Enabled if true, disabled otherwise
         */
        void                                EnableDepthTest(bool val);

        /**
         * Enable or disable depth writing. This will only work if depth testing is enabled
         * @param val Enabled if true, disabled otherwise
         */
        void                                EnableDepthWrite(bool val);

        /**
         * Enable or disable color writing.
         * @param val Enabled if true, disabled otherwise
         */
        void                                EnableColorWrite(bool val);

        /**
         * Sets the depth compare function to use
         * @param comparison The comparison function to use
         */
        void                                SetDepthComparisonFunc(DepthFunc_t comparison);

        /**
         * Select the culling method
         * @param cullingMethod The culling method to use
         */
        void                                SetCullingMethod(CullingMethod_t cullingMethod);

        /**
         * Create an empty shader
         * @return A pointer to a shader
         */
        virtual Shader*                     CreateShader() = 0;

        /**
         * Create an empty texture
         * @return A pointer to a texture
         */
        virtual Texture2D*                  CreateTexture2D() const = 0;

        /**
         * Create an empty texture
         * @return A pointer to a texture
         */
        virtual Texture3D*                  CreateTexture3D() const = 0;

        /**
         * Create a render texture
         * @param width The width of the render texture
         * @param height The height of the render texture
         * @param format The format of the render texture
         * @return A pointer to a render texture
         */
        virtual RenderTexture*              CreateRenderTexture(unsigned int width, unsigned int height, TextureFormat_t format) = 0;

        /**
         * Binds the render system frame buffer that is used to draw on the screen
         */
        virtual void                        BindScreenBuffer() const = 0;

        /**
         * Enable blending
         * @param val if true, enables blending, deactivate it otherwise
         */
        void                                EnableBlending(bool val);

        /**
         * Specifies the blending equation to use
         */
        void                                SetBlendingEquation(BlendingEquation_t equation);

        /**
         * Specifies the blending factor to use
         * @param srcFactor The factor to use for the source pixel (the pixel being processed)
         * @param dstFactor the factor to use for the destination pixel (the pixel already present in the framebuffer)
         */
        void                                SetBlendingFactor(BlendingFactor_t srcFactor, BlendingFactor_t dstFactor);

        /**
         * Convert a point in screen space into world space. This function uses the current perspective and view matrix
         * @param inversedViewProjection The inverse of the view-projection matrix
         * @param screenPoint A vector representing the point in screen space
         * @return A vector representing the point in world space
         */
        Vector3                             ScreenToWorldPoint(const Matrix4x4& inversedViewProjection, const Vector2& point) const;

        /**
         * Bind a texture to a texture unit
         * @param texture A pointer to the texture object to bind
         */
        virtual size_t                      BindTexture(const Texture* texture);

        /**
         * Bind the specified shader to the GPU
         * @param shader The shader to bind. If set to nullptr, then it will unbind the current shader
         */
        virtual void                        BindShader(const Shader* shader) = 0;

        /**
         * Extract the frustum planes from the view projection matrix
         * @param viewProjection The view projection matrix of the view frustum used to extract the planes
         * @return A FrustumPlanes_t object, containing the 6 view frustum planes
         */
        virtual FrustumPlanes_t             ExtractViewFrustumPlanes(const Matrix4x4& viewProjection) const = 0;

        /**
         * Draw the content of a buffer object using a shader made for drawing text
         * @param bufferObject The buffer object to draw
         * @param fontAtlas A texture atlas representing the font that is going to be used
         * @parma textColor The color of the text been rendered
         */
        void                                DrawTextBuffer(BufferObject* bufferObject, Texture2D* fontAtlas, const Vector3& textColor);

        size_t                              GetWidth() const { return width_; }
        size_t                              GetHeight() const { return height_; }
        const DeviceCapabilities_t* const   GetDeviceCapabilities() const { return &deviceCapabilities_; }
        BufferObjectManager*                GetBufferObjectManager() const;
        RenderStateCache*                   GetRenderStateCache() const;

	protected:
        Window&							    window_;        /**< The window */
		WindowHandle					    windowHandle_;	/**< The window's handle */
		unsigned int					    width_;			/**< The width of the window */
		unsigned int					    height_;		/**< The height of the window */
		bool							    windowed_;		/**< Is the window in windowed mode ? */

        vector<Shader*>                     shaders_;       /**< List of all shaders created */
        vector<RenderTexture*>              renderTextures_;/**< List of all render textures created */
        const Shader*                       boundShader_;   /**< The currently bound shader */

        DeviceCapabilities_t                deviceCapabilities_;
        BufferObjectManager*                bufferObjectManager_;
        RenderStateCache*                   renderStateCache_;

        Shader*                             textShader_;    /**< Shader used to draw text on screen */

		/**
		 * Query the device capabilities
		 */
		virtual void					    QueryDeviceCapabilities() = 0;

        /**
         * Create the shader that will be used to draw the text on screen
         */
        virtual void                        CreateTextShader() = 0;

        /**
         * Free the render system
         */
        void                                FreeRenderSystem();
};

}

#endif
