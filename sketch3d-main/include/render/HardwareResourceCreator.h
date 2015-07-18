#ifndef SKETCH_3D_HARDWARE_RESOURCE_CREATOR_H
#define SKETCH_3D_HARDWARE_RESOURCE_CREATOR_H

#include "system/Platform.h"

#include "render/RenderDevice_Common.h"
#include "render/Renderer_Common.h"
#include "render/SamplerState.h"
#include "render/Texture.h"
#include "render/VertexFormat.h"

#include <memory>
using namespace std;

namespace Sketch3D {
// Forward class declaration
class ConstantBuffer;
class DepthStencilTarget;
class IndexBuffer;
class FragmentShader;
class RenderTarget;
class VertexBuffer;
class VertexShader;

/**
 * @class HardwareResourceCreator
 * Base class for API dependent hardware resource creation
 */
class SKETCH_3D_API HardwareResourceCreator {
    public:
        virtual shared_ptr<ConstantBuffer>      CreateConstantBuffer(void* initialData, bool dynamic, bool immutable, size_t dataSize) = 0;
        virtual shared_ptr<DepthStencilTarget>  CreateDepthStencilTarget(size_t width, size_t height, DepthStencilBits_t depthStencilBits) = 0;
        virtual shared_ptr<IndexBuffer>         CreateIndexBuffer(void* initialData, bool dynamic, bool immutable, IndexFormat_t indexFormat, size_t numIndices) = 0;
        virtual shared_ptr<FragmentShader>      CreateFragmentShader() = 0;
        virtual shared_ptr<RenderTarget>        CreateRenderTarget(size_t width, size_t height, TextureFormat_t textureFormat) = 0;
        virtual shared_ptr<SamplerState>        CreateSamplerState(FilterMode_t filterMode, AddressMode_t addressModeU, AddressMode_t addressModeV, AddressMode_t addressModeW,
                                                                   ComparisonFunction_t comparisonFunction, const Vector4& borderColor=Vector4::ZERO) = 0;
        virtual shared_ptr<Texture2D>           CreateTexture2D(TextureMap* textureMap, TextureFormat_t textureFormat, bool dynamic, bool immutable) = 0;
        virtual shared_ptr<VertexBuffer>        CreateVertexBuffer(void* initialData, bool dynamic, bool immutable, VertexFormat* vertexFormat, size_t numVertices) = 0;
        virtual shared_ptr<VertexShader>        CreateVertexShader() = 0;
};
}

#endif