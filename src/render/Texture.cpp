#include "render/Texture.h"

#include "render/Renderer.h"

namespace Sketch3D {

Texture::Texture(bool generateMipmaps) : width_(0), height_(0), generateMipmaps_(generateMipmaps), filterMode_(FILTER_MODE_NEAREST),
					 wrapMode_(WRAP_MODE_CLAMP), format_(TEXTURE_FORMAT_RGB24)
{
}

Texture::Texture(unsigned int width, unsigned int height, bool generateMipmaps,
                 FilterMode_t filterMode, WrapMode_t wrapMode, TextureFormat_t format) : width_(width),
																                         height_(height),
                                                                                         generateMipmaps_(generateMipmaps),
																                         filterMode_(filterMode),
																                         wrapMode_(wrapMode),
                                                                                         format_(format)
{
}

Texture::~Texture() {
}

size_t Texture::Bind() const {
    return Renderer::GetInstance()->BindTexture(this);
}

void Texture::SetWidth(unsigned int width) {
	width_ = width;
}

void Texture::SetHeight(unsigned int height) {
	height_ = height;
}

void Texture::SetGenerateMipmaps(bool generateMipmaps) {
    generateMipmaps_ = generateMipmaps;
}

void Texture::SetFilterMode(FilterMode_t mode) {
	filterMode_ = mode;
    SetFilterModeImpl();
}

void Texture::SetWrapMode(WrapMode_t mode) {
	wrapMode_ = mode;
    SetWrapModeImpl();
}

void Texture::SetTextureFormat(TextureFormat_t format) {
    format_ = format;
}

unsigned int Texture::GetWidth() const {
	return width_;
}

unsigned int Texture::GetHeight() const {
	return height_;
}

bool Texture::GetGenerateMipmaps() const {
    return generateMipmaps_;
}

FilterMode_t Texture::GetFilterMode() const {
	return filterMode_;
}

WrapMode_t Texture::GetWrapMode() const {
	return wrapMode_;
}

TextureFormat_t Texture::GetTextureFormat() const {
    return format_;
}

}