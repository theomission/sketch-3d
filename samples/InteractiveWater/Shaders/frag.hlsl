SamplerState sampler0;

Texture2D tex;

struct PS_INPUT {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET {
    return tex.Sample(sampler0, input.uv);
}