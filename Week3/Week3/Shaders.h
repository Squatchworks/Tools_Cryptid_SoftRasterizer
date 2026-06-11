#pragma once
#include "MyMath.h"

// The active vertex shader. Modifies an incoming vertex. Pre-Rasterization.
void (*VertexShader)(Vertex&) = 0;

// The active pixel shader. Modifies an outgoing pixel. Post-Rasterization.
void (*PixelShader)(unsigned int&) = 0;

// Applys the current world matrix to all
void VS_World(Vertex& vertex) 
{
    MultiplyVertexByMatrix(vertex, SV_WorldMatrix);
    MultiplyVertexByMatrix(vertex, viewMatrix);
    MultiplyVertexByMatrix(vertex, ProjectionMatrix);
    PerformPerspectiveDivide(vertex);
}

void PS_White(unsigned int& pixelColor) 
{
    // Set the pixel color to white
    pixelColor = WHITE_COLOR;
}

void PS_Green(unsigned int& pixelColor) 
{
    // Set the pixel color to green
    pixelColor = GREEN_COLOR;
}
