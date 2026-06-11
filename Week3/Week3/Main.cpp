#include "RasterFunc.h"
#include <iostream>
#include "RasterSurface.h"
#include "XTime.h"

void printMatrix(Matrix4x4& matrix, int precision = 2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%.*f ", precision, matrix.data[i][j]);
        }
        printf("\n");
    }
}

int main(void)
{
    CreateViewMatrix();
    for (int i = 0; i < NUM_PIXELS; i++) {
        depthBuffer[i] = 1.0f;
    }
    XTime Time;
    double lastRotationTime = 0.0;
    RS_Initialize("Jacob Blackburn Assignment 2", WIDTH, HEIGHT);
    LoadGridLines();
    LoadCube();
    ProjectionMatrix = CreatePerspectiveMatrix(fov, ASPECT_RATIO, nearPlane, farPlane);
    printf("Perspective Projection Matrix:\n");
for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
        printf("%f ", ProjectionMatrix.data[i][j]);
    }
    printf("\n");
}
    VertexShader = VS_World;  // Assign VertexShader outside the loop

    // Initialize the second cube's world matrix
    Matrix4x4 rotationMatrix = CreateRotationXMatrix(45.0 * 3.14159 / 180.0);
    Matrix4x4 translationMatrix = CreateTranslationMatrix(3, 1.0, 2.0);
    Matrix4x4 scalingMatrix = CreateScalingMatrix(0.3, 0.3, 0.3);
    Matrix4x4 FirstFactor = MultiplyMatrices(translationMatrix, scalingMatrix);
    secondCubeWorldMatrix = MultiplyMatrices(rotationMatrix, FirstFactor);

    do //loop is running each fram so anything inside the loop is gonna affect fram rates
    {
        Time.Signal();
        // Clear the screen
        clearScreen(BackBuffer, WIDTH, HEIGHT, 0x000000);

        DrawGrid();
        // Calculate the elapsed time since the last rotation
        double currentTime = Time.TotalTime();
        double elapsedTime = currentTime - lastRotationTime;

        // Check if it's time to rotate the cube
        if (elapsedTime >= 0.01) { // Rotate every 10 milliseconds
            // Calculate the rotation angle based on the elapsed time
            double angle = 0.01; // 0.01 radians per rotation

            // Create a rotation matrix around the y-axis
            Matrix4x4 rotationMatrix = CreateRotationYMatrix(angle);

            // Multiply the cube's world matrix by the rotation matrix
            cubeWorldMatrix = MultiplyMatrices(rotationMatrix, cubeWorldMatrix);

            //// Update the second cube's world matrix
            //Matrix4x4 rotation2MatrixX = CreateRotationXMatrix(-0.01); // Rotate around the x-axis
            //secondCubeWorldMatrix = MultiplyMatrices(rotation2MatrixX, secondCubeWorldMatrix);

            //Matrix4x4 rotation3Matrix = CreateRotationZMatrix(0.01);
            //secondCubeWorldMatrix = MultiplyMatrices(rotation3Matrix, secondCubeWorldMatrix);

            // Update the last rotation time
            lastRotationTime = currentTime;
        }

        // Draw the first cube
        SV_WorldMatrix = cubeWorldMatrix;
        DrawCube();

        // Draw the second cube
        SV_WorldMatrix = secondCubeWorldMatrix;
        DrawCube();

    } while (RS_Update(BackBuffer, NUM_PIXELS));
    delete[] BackBuffer;
    RS_Shutdown();

    return 0;
}
