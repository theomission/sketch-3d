#ifndef SKETCH_3D_TEXTURE_H
#define SKETCH_3D_TEXTURE_H

namespace Sketch3D {

/**
 * @enum FilterMode_t
 * The filter mode of the texture
 */
enum FilterMode_t {
	FILTER_MODE_NEAREST,
    FILTER_MODE_LINEAR
};

/**
 * @enum WrapMode_t
 * The wrap mode of the texture
 */
enum WrapMode_t {
	WRAP_MODE_CLAMP,
	WRAP_MODE_REPEAT,
    WRAP_MODE_CLAMP_TO_BORDER
};

/**
 * @enum TextureFormat_t
 * The format of the texture, describing how to interpret the bytes
 */
enum TextureFormat_t {
	TEXTURE_FORMAT_RGB24,
	TEXTURE_FORMAT_RGBA32,
    TEXTURE_FORMAT_BGR24,
    TEXTURE_FORMAT_BGRA32,

    // There should be a separation between byte texture format and floating point texture format
    TEXTURE_FORMAT_R32F,
    TEXTURE_FORMAT_RG32F,
    TEXTURE_FORMAT_RGB32F,
    TEXTURE_FORMAT_RGBA32F,

    // Special texture formats
    TEXTURE_FORMAT_DEPTH
};

/**
 * @class Texture
 * This class serves as a base class to other classes that act as textures,
 * such as the Texture2D and RenderTexture classes.
 */
class Texture {
	public:
		/**
		 * Constructor. Initializes the width and height to 0, the filter mode
		 * to FILTER_MODE_POINT and the wrap mode to WRAP_MODE_CLAMP and the texture
		 * format is set to TEXTURE_FORMAT_RGB24
		 */
						Texture();

		/**
		 * Constructor. Initializes the filter mode to FILTER_MODE_POINT and
		 * the wrap mode to WRAP_MODE_CLAMP by default.
		 * @param width The width of the texture
		 * @param height The height of the texture
		 * @param filterMode The filter mode to use
		 * @param wrapMode The wrap mode to use
         * @param format The format to use
		 */
						Texture(unsigned int width, unsigned int height,
								FilterMode_t filterMode=FILTER_MODE_NEAREST,
								WrapMode_t wrapMode=WRAP_MODE_REPEAT,
								TextureFormat_t format=TEXTURE_FORMAT_RGB24);

        /**
         * Destructor
         */
        virtual        ~Texture();

        /**
         * Create the actual texture handle
         * @return true if the texture was created correctly
         */
        virtual bool            Create() = 0;

        /**
         * Activate the texture
         * @param unit On which texture unit should we bind the texture
         */
        virtual void            Bind(unsigned int unit) = 0;

		void			SetWidth(unsigned int width);
		void			SetHeight(unsigned int height);
		void			SetFilterMode(FilterMode_t mode);
		void			SetWrapMode(WrapMode_t mode);
        void		    SetTextureFormat(TextureFormat_t format);

		unsigned int	GetWidth() const;
		unsigned int	GetHeight() const;
		FilterMode_t	GetFilterMode() const;
		WrapMode_t		GetWrapMode() const;
        TextureFormat_t	GetTextureFormat() const;

	protected:
		unsigned int	width_;	/**< The width of the texture */
		unsigned int	height_;	/**< The height of the texture */
		FilterMode_t	filterMode_;	/**< The filter mode of the texture */
		WrapMode_t		wrapMode_;	/**< The wrap mode of the texture */
        TextureFormat_t format_;    /**< The texture format */
};

}

#endif