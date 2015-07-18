#include "render/Direct3D11/HardwareResourceCreatorDirect3D11.h"

#include "render/Direct3D11/BufferDirect3D11.h"
#include "render/Direct3D11/RenderTargetDirect3D11.h"
#include "render/Direct3D11/ShaderDirect3D11.h"
#include "render/Direct3D11/SamplerStateDirect3D11.h"
#include "render/Direct3D11/TextureDirect3D11.h"

#include "system/Logger.h"

#pragma warning( disable : 4005 )

#include <d3d11.h>

#pragma warning( default : 4005 )

namespace Sketch3D {
HardwareResourceCreatorDirect3D11::HardwareResourceCreatorDirect3D11(ID3D11Device* device, ID3D11DeviceContext* context) : device_(device), context_(context) {
}

shared_ptr<ConstantBuffer> HardwareResourceCreatorDirect3D11::CreateConstantBuffer()  {
    ConstantBuffer* constantBuffer = new ConstantBufferDirect3D11(device_, context_);
    return shared_ptr<ConstantBuffer>(constantBuffer);
}

shared_ptr<DepthStencilTarget> HardwareResourceCreatorDirect3D11::CreateDepthStencilTarget() {
    DepthStencilTarget* depthStencilTarget = new DepthStencilTargetDirect3D11(device_);
    return shared_ptr<DepthStencilTarget>(depthStencilTarget);
}

shared_ptr<IndexBuffer> HardwareResourceCreatorDirect3D11::CreateIndexBuffer() {
    IndexBuffer* indexBuffer = new IndexBufferDirect3D11(device_, context_);
    return shared_ptr<IndexBuffer>(indexBuffer);
}

shared_ptr<FragmentShader> HardwareResourceCreatorDirect3D11::CreateFragmentShader() {
    FragmentShader* fragmentShader = new FragmentShaderDirect3D11(device_);
    return shared_ptr<FragmentShader>(fragmentShader);
}

shared_ptr<RenderTarget> HardwareResourceCreatorDirect3D11::CreateRenderTarget() {
    RenderTarget* renderTarget = new RenderTargetDirect3D11(device_);
    return shared_ptr<RenderTarget>(renderTarget);
}

shared_ptr<SamplerState> HardwareResourceCreatorDirect3D11::CreateSamplerState() {
    SamplerState* samplerState = new SamplerStateDirect3D11(device_);
    return shared_ptr<SamplerState>(samplerState);
}

shared_ptr<Texture2D> HardwareResourceCreatorDirect3D11::CreateTexture2D() {
    Texture2D* texture2D = new Texture2DDirect3D11(device_);
    return shared_ptr<Texture2D>(texture2D);
}

shared_ptr<VertexBuffer> HardwareResourceCreatorDirect3D11::CreateVertexBuffer() {
    VertexBuffer* vertexBuffer = new VertexBufferDirect3D11(device_, context_);
    return shared_ptr<VertexBuffer>(vertexBuffer);
}

shared_ptr<VertexShader> HardwareResourceCreatorDirect3D11::CreateVertexShader() {
    VertexShader* vertexShader = new VertexShaderDirect3D11(device_);
    return shared_ptr<VertexShader>(vertexShader);
}
}