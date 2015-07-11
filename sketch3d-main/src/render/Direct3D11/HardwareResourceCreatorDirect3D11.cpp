#include "render/Direct3D11/HardwareResourceCreatorDirect3D11.h"

#include "render/Direct3D11/BufferDirect3D11.h"
#include "render/Direct3D11/ShaderDirect3D11.h"

#pragma warning( disable : 4005 )

#include <d3d11.h>

#pragma warning( default : 4005 )

namespace Sketch3D {
HardwareResourceCreatorDirect3D11::HardwareResourceCreatorDirect3D11(ID3D11Device* device, ID3D11DeviceContext* context) : device_(device), context_(context) {
}

shared_ptr<ConstantBuffer> HardwareResourceCreatorDirect3D11::CreateConstantBuffer(void* initialData, bool dynamic, bool immutable, size_t dataSize)  {
    ConstantBuffer* constantBuffer = new ConstantBufferDirect3D11(device_, context_, initialData, dynamic, immutable, dataSize);
    return shared_ptr<ConstantBuffer>(constantBuffer);
}

shared_ptr<IndexBuffer> HardwareResourceCreatorDirect3D11::CreateIndexBuffer(void* initialData, bool dynamic, bool immutable, IndexFormat_t indexFormat, size_t numIndices) {
    IndexBuffer* indexBuffer = new IndexBufferDirect3D11(device_, context_, initialData, dynamic, immutable, indexFormat, numIndices);
    return shared_ptr<IndexBuffer>(indexBuffer);
}

shared_ptr<FragmentShader> HardwareResourceCreatorDirect3D11::CreateFragmentShader() {
    FragmentShader* fragmentShader = new FragmentShaderDirect3D11(device_);
    return shared_ptr<FragmentShader>(fragmentShader);
}

shared_ptr<VertexBuffer> HardwareResourceCreatorDirect3D11::CreateVertexBuffer(void* initialData, bool dynamic, bool immutable, VertexFormat* vertexFormat, size_t numVertices) {
    VertexBuffer* vertexBuffer = new VertexBufferDirect3D11(device_, context_, initialData, dynamic, immutable, vertexFormat, numVertices);
    return shared_ptr<VertexBuffer>(vertexBuffer);
}

shared_ptr<VertexShader> HardwareResourceCreatorDirect3D11::CreateVertexShader() {
    VertexShader* vertexShader = new VertexShaderDirect3D11(device_);
    return shared_ptr<VertexShader>(vertexShader);
}
}