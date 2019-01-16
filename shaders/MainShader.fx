
//--------------------------------------------------------------------------------------
// Переменные константных буферов
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 ) // b0 - индекс буфера
{
	matrix World;
	matrix View;
	matrix Projection;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT	// формат выходных данных вершинного шейдера
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};



//--------------------------------------------------------------------------------------
// Вершинный шейдер
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Pos : POSITION, float4 Color : COLOR )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    // Трансформация позиции вершины при помощи умножения на матрицу
    output.Pos = mul( Pos, World ); // сначала в пространство мира
    output.Pos = mul( output.Pos, View ); // затем в пространство вида
    output.Pos = mul( output.Pos, Projection ); // в проекционное пространство
    output.Color = Color;
    return output;
}


//--------------------------------------------------------------------------------------
// Пиксельный шейдер
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return input.Color;
}

float4 PS1(VS_OUTPUT input) : SV_Target
{   float4 a = (1.0f,1.0f,1.0f,1.0f);
	return a;
}


Texture2D ObjTexture: register(t0);
Texture2D ObjTexture1: register(t1);
Texture2D ObjTexture2: register(t2);
SamplerState ObjSamplerState: register(s0);




struct VS_OUTPUT2
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};



VS_OUTPUT2 VS2(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{

	VS_OUTPUT2 output = (VS_OUTPUT2)0;
	output.Pos = mul(inPos, World); // сначала в пространство мира
	output.Pos = mul(output.Pos, View); // затем в пространство вида
	output.Pos = mul(output.Pos, Projection); // в проекционное пространство
	output.TexCoord = inTexCoord;

	return output;
}

float4 PS2(VS_OUTPUT2 input) : SV_TARGET
{
	return ObjTexture.Sample(ObjSamplerState, input.TexCoord);
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////

struct VS_OUTPUT6
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 TexCoord : TEXCOORD;
};

VS_OUTPUT6 VS6(float4 inPos : POSITION, float3 inNorm : NORMAL, float2 inTexCoord : TEXCOORD)
{

	VS_OUTPUT6 output = (VS_OUTPUT6)0;
	output.Pos = mul(inPos, World); // сначала в пространство мира
	output.Pos = mul(output.Pos, View); // затем в пространство вида
	output.Pos = mul(output.Pos, Projection); // в проекционное пространство
	//output.Norm = mul(inNorm, World);
	//output.Norm = mul(output.Norm, View);
	output.Norm = inNorm;
	output.TexCoord = inTexCoord;

	float pi = 3.14159265359f;
	//output.TexCoord.x = 0.5 + ((atan2(inNorm.z, inNorm.x)) / (pi*2));
	//output.TexCoord.y = 0.5 - ((asin(inNorm.y)) / pi);

	//output.TexCoord.x = (asin(inNorm.x) / pi) + 0.5;
   //output.TexCoord.y = (asin(inNorm.y) / pi) + 0.5;
	return output;
}


float4 PS6(VS_OUTPUT6 input) : SV_TARGET
{
	float4 finalColor = 0;
	finalColor.x = finalColor.y = finalColor.z = 0.5f;
float  pi = 3.141592f;
input.TexCoord.x = (0.5 + (atan2(input.Norm.z, input.Norm.x) / (2 * pi)));
input.TexCoord.y = (0.5f - (asin(input.Norm.y) / pi));

finalColor*=ObjTexture.Sample(ObjSamplerState, input.TexCoord);
return finalColor;
}




struct VS_OUTPUT7
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 TexCoord : TEXCOORD;
};


VS_OUTPUT7 VS7(float4 inPos : POSITION, float3 inNorm : NORMAL, float2 inTexCoord : TEXCOORD)
{

	VS_OUTPUT7 output = (VS_OUTPUT7)0;
	output.Pos = mul(inPos, World); // сначала в пространство мира
	output.Pos = mul(output.Pos, View); // затем в пространство вида
	output.Pos = mul(output.Pos, Projection); // в проекционное пространство
	output.Norm = mul(inNorm, World);
	output.TexCoord = inTexCoord;

	return output;
}


float4 PS7(VS_OUTPUT7 input) : SV_TARGET
{
	float4 finalColor = 0;
	finalColor.x = finalColor.y = finalColor.z = 1.1f;
	finalColor *= ObjTexture.Sample(ObjSamplerState, input.TexCoord);
	return finalColor;
}


/*


float3 norm = normalize(input.Norm);
float theta = acos(norm.z);
float phi = atan2(norm.y, norm.x)*2;

float2 texCoord = float2(phi / 2, theta) / 3.14159265;

//float4 color = 1.0f;
//color.rgb = ObjTexture(ObjSamplerState, texCoord).rgb;
//return color;

return ObjTexture.Sample(ObjSamplerState, texCoord);

// VERTEX SHADER
float  p = 3.141592f;
float3 x = float3(1, 0, 0);
float3 z = float3(0, 0, 1);

float3 norm = normalize(input.normal);

// find theta using the normal and z
float theta = acos(dot(norm,z));

// find the x-y projection of the normal
float3 proj = normalize(float3(norm.x, norm.y, 0));

// find phi using the x-y projection and x
float phi = acos(dot(proj,x));

// if x-y projection is in quadrant 3 or 4, then phi = 2PI - phi
if (proj.y < 0)
phi = 2*p - phi;

float3 coords = float3(phi / (2*p), theta / p, 0);

output.texCoord = coords;


// PIXEL SHADER
float4 color = float4(tex2D(gSampler,float2(input.texCoord.x,input.texCoord.y)).rgb,1);
return color;

*/
