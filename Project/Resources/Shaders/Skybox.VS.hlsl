#include "Skybox.hlsli"

struct VertexShaderInput {
    float4 position : POSITION0;
};

VertexShaderOutput main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}