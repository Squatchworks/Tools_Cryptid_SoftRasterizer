#pragma once
#include "Shaders.h"
#include "math.h"


const int GRID_SIZE = 11;

void clearScreen(unsigned int* BackBuffer, int WIDTH, int HEIGHT, unsigned int color) 
{
    if (BackBuffer == nullptr) {
        // Handle null pointer dereference
        return;
    }

    int numPixels = WIDTH * HEIGHT;

    for (int i = 0; i < numPixels; i++) {
        BackBuffer[i] = color;
    }
}

void plotPixel(unsigned int* BackBuffer, int x, int y, int WIDTH, int HEIGHT, unsigned int color)
{
    if (BackBuffer == nullptr) {
        // Handle null pointer dereference
        return;
    }

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        // Handle out-of-bounds access
        return;
    }

    // Convert the 2D coordinates to a 1D index
    int index = convert2DTo1D(x, y, WIDTH);

    // Set the pixel color at the index
    BackBuffer[index] = color;
}

void plotPixelWithDepth(unsigned int* BackBuffer, int x, int y, int WIDTH, int HEIGHT, unsigned int color, float depth) {
    // Check if the pixel is closer than the current depth value
    if (depth < depthBuffer[convert2DTo1D(x, y, WIDTH)]) {
        // Draw the pixel
        BackBuffer[convert2DTo1D(x, y, WIDTH)] = color;
        depthBuffer[convert2DTo1D(x, y, WIDTH)] = depth;
    }
}

void BLIT(int srcX, int srcY, int srcWIDTH, int srcHEIGHT, int dstX, int dstY, int srcTextureWIDTH,
    int dstTextureWIDTH, unsigned int* pSourceTextureArray, unsigned int* pDestinationRaster) 
{
    // Loop through each row of the source texture
    for (int y = 0; y < srcHEIGHT; y++) {
        // Loop through each column of the source texture
        for (int x = 0; x < srcWIDTH; x++) {
            // Calculate the source index
            int srcIndex = convert2DTo1D(srcX + x, srcY + y, srcTextureWIDTH);

            // Calculate the destination index
            int dstIndex = convert2DTo1D(dstX + x, dstY + y, dstTextureWIDTH);

            // Check if the destination index is within the bounds of the raster
            if (dstIndex >= 0 && dstIndex < dstTextureWIDTH * dstTextureWIDTH) 
            {
                // Copy the pixel from the source texture to the destination raster
                unsigned int srcPixel = pSourceTextureArray[srcIndex];
                unsigned int dstPixel = pDestinationRaster[dstIndex];

                // Perform alpha blending
                unsigned int blendedPixel = AlphaBlend(dstPixel, srcPixel);

                pDestinationRaster[dstIndex] = blendedPixel;
            }
        }
    }
}

void DrawLine(const Vertex& start, const Vertex& end) 
{
    Vertex Copy_start = start;
    Vertex Copy_end = end;

    // Apply the vertex shader to the start and end vertices
    if (VertexShader) 
    {
        VertexShader(Copy_start);
        VertexShader(Copy_end);
    }
    //printf("Transformed Start Vertex: (%f, %f, %f)\n", Copy_start.x, Copy_start.y, Copy_start.z);
    //printf("Transformed End Vertex: (%f, %f, %f)\n", Copy_end.x, Copy_end.y, Copy_end.z);
    Copy_start = NDCtoScreen(Copy_start, WIDTH, HEIGHT);
    Copy_end = NDCtoScreen(Copy_end, WIDTH, HEIGHT);

    // Calculate the differences in x and y
    int dx = Copy_end.x - Copy_start.x;
    int dy = Copy_end.y - Copy_start.y;

    // Determine the direction of the line
    int xDir = (dx > 0) ? 1 : -1;
    int yDir = (dy > 0) ? 1 : -1;

    // Check if the line is steep
    bool isSteep = abs(dy) > abs(dx);

    // Calculate the length of the line
    int length = (isSteep) ? abs(dy) : abs(dx);

    // Iterate over the length of the line
    for (int i = 0; i <= length; i++) 
    {
        // Calculate the parameter t
        double t = (double)i / length;

        // Calculate the x and y coordinates
        Vertex point;
        point.x = Copy_start.x + xDir * round(t * abs(dx));
        point.y = Copy_start.y + yDir * round(t * abs(dy));
        unsigned int copy_color = start.color;

        if (PixelShader) { PixelShader(copy_color); }
        // Plot the pixel
        plotPixel(BackBuffer, point.x, point.y, WIDTH, HEIGHT, copy_color);
    }
}

void DrawGrid() 
{
    SV_WorldMatrix = gridWorldMatrix;
    for (int i = 0; i < 11; i++) 
    {
        // Draw horizontal lines
        PixelShader = PS_White;
        DrawLine({ -0.5f, 0.0f, -0.5f + (i * 0.1f), 1, 0xFFFFFFFF }, { 0.5f, 0.0f, -0.5f + (i * 0.1f), 1, 0xFFFFFFFF });
    }

    for (int i = 0; i < 11; i++) 
    {
        // Draw vertical lines
        PixelShader = PS_White;
        DrawLine({ -0.5f + (i * 0.1f), 0.0f, -0.5f, 1, 0xFFFFFFFF }, { -0.5f + (i * 0.1f), 0.0f, 0.5f, 1, 0xFFFFFFFF });
    }
}

void FillTriangle(Vertex v1, Vertex v2, Vertex v3) 
{
    // Calculate the barycentric coordinates of the triangle
    float minX = std::min(v1.x, std::min(v2.x, v3.x));
    float maxX = std::max(v1.x, std::max(v2.x, v3.x));
    float minY = std::min(v1.y, std::min(v2.y, v3.y));
    float maxY = std::max(v1.y, std::max(v2.y, v3.y));

    for (int y = minY; y <= maxY; y++) 
    {
        for (int x = minX; x <= maxX; x++) 
        {
            // Calculate the barycentric coordinates of the pixel
            float alpha = ((v2.y - v3.y) * (x - v3.x) + (v3.x - v2.x) * (y - v3.y)) / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
            float beta = ((v3.y - v1.y) * (x - v3.x) + (v1.x - v3.x) * (y - v3.y)) / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
            float gamma = 1.0f - alpha - beta;

            // If the pixel is inside the triangle, draw it
            if (alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f) 
            {
                // Calculate the depth value of the pixel
                float depth = alpha * v1.z + beta * v2.z + gamma * v3.z;

                // Check if the pixel is closer than the current depth value
                if (depth < depthBuffer[convert2DTo1D(x, y, WIDTH)]) 
                {
                    // Draw the pixel
                    plotPixelWithDepth(BackBuffer, x, y, WIDTH, HEIGHT, 0xFFFFFFFF, depth);
                }
            }
        }
    }
}

void DrawCube() 
{
    // Draw the front face
    FillTriangle(TopFrontLeft, TopFrontRight, BottomFrontLeft);
    FillTriangle(TopFrontLeft, BottomFrontLeft, BottomFrontRight);

    // Draw the back face
    FillTriangle(TopBackLeft, TopBackRight, BottomBackLeft);
    FillTriangle(TopBackLeft, BottomBackLeft, BottomBackRight);

    // Draw the left face
    FillTriangle(TopFrontLeft, TopBackLeft, BottomBackRight);
    FillTriangle(TopFrontLeft, BottomBackRight, BottomFrontRight);

    // Draw the right face
    FillTriangle(TopFrontRight, TopBackRight, BottomBackLeft);
    FillTriangle(TopFrontRight, BottomBackLeft, BottomFrontLeft);

    // Draw the top face
    FillTriangle(TopFrontLeft, TopFrontRight, TopBackRight);
    FillTriangle(TopFrontLeft, TopBackRight, TopBackLeft);

    // Draw the bottom face
    FillTriangle(BottomFrontRight, BottomFrontLeft, BottomBackLeft);
    FillTriangle(BottomFrontRight, BottomBackLeft, BottomBackRight);
}
