#pragma once
#include "Defines.h"
#include <cmath>
#include <stdio.h>
#include <vector>

int convert2DTo1D(int x, int y, int width) 
{
	return y * width + x;
}


unsigned int BGRAtoARGB(unsigned int C) 
{
	unsigned int B = (C & 0xFF000000) >> 24; // Blue
	unsigned int G = (C & 0x00FF0000) >> 16; // Green
	unsigned int R = (C & 0x0000FF00) >> 8;  // Red
	unsigned int A = (C & 0x000000FF);       // Alpha

	unsigned int A_shifted = A << 24; // Alpha to most significant byte
	unsigned int R_shifted = R << 16; // Red to second most significant byte
	unsigned int G_shifted = G << 8;  // Green to third most significant byte
	unsigned int B_shifted = B;       // Blue to least significant byte

	unsigned int result = A_shifted | R_shifted | G_shifted | B_shifted;

	return result;
}


unsigned int AlphaBlend(unsigned int destColor, unsigned int srcColor) 
{
	// Break down DestinationColor and SourceColor into 4 channels each
	unsigned int DestAlpha = (destColor & 0xFF000000) >> 24;
	unsigned int DestRed = (destColor & 0x00FF0000) >> 16;
	unsigned int DestGreen = (destColor & 0x0000FF00) >> 8;
	unsigned int DestBlue = (destColor & 0x000000FF);

	unsigned int SrcAlpha = (srcColor & 0xFF000000) >> 24;
	unsigned int SrcRed = (srcColor & 0x00FF0000) >> 16;
	unsigned int SrcGreen = (srcColor & 0x0000FF00) >> 8;
	unsigned int SrcBlue = (srcColor & 0x000000FF);

	// Calculate fAlphaRatio
	float fAlphaRatio = (float)SrcAlpha / MAX_COLOR_VALUE;

	// Interpolate all 4 channels based on fAlphaRatio
	unsigned int FinalAlpha = (unsigned int)(DestAlpha * (1.0f - fAlphaRatio) + SrcAlpha * fAlphaRatio);
	unsigned int FinalRed = (unsigned int)(DestRed * (1.0f - fAlphaRatio) + SrcRed * fAlphaRatio);
	unsigned int FinalGreen = (unsigned int)(DestGreen * (1.0f - fAlphaRatio) + SrcGreen * fAlphaRatio);
	unsigned int FinalBlue = (unsigned int)(DestBlue * (1.0f - fAlphaRatio) + SrcBlue * fAlphaRatio);

	// Reassemble the 4 color channels into a single unsigned int
	unsigned int Result = (FinalAlpha << 24) | (FinalRed << 16) | (FinalGreen << 8) | FinalBlue;

	return Result;
}

Matrix4x4 CreateScalingMatrix(float x, float y, float z)
{
	Matrix4x4 scalingMatrix;
	scalingMatrix.data[0][0] = x;
	scalingMatrix.data[0][1] = 0.0f;
	scalingMatrix.data[0][2] = 0.0f;
	scalingMatrix.data[0][3] = 0.0f;
	scalingMatrix.data[1][0] = 0.0f;
	scalingMatrix.data[1][1] = y;
	scalingMatrix.data[1][2] = 0.0f;
	scalingMatrix.data[1][3] = 0.0f;
	scalingMatrix.data[2][0] = 0.0f;
	scalingMatrix.data[2][1] = 0.0f;
	scalingMatrix.data[2][2] = z;
	scalingMatrix.data[2][3] = 0.0f;
	scalingMatrix.data[3][0] = 0.0f;
	scalingMatrix.data[3][1] = 0.0f;
	scalingMatrix.data[3][2] = 0.0f;
	scalingMatrix.data[3][3] = 1.0f;
	return scalingMatrix;
}

Matrix4x4 CreateTranslationMatrix(float x, float y, float z) 
{
	Matrix4x4 translationMatrix;
	translationMatrix.data[0][0] = 1.0f;
	translationMatrix.data[0][1] = 0.0f;
	translationMatrix.data[0][2] = 0.0f;
	translationMatrix.data[0][3] = 0.0f;
	translationMatrix.data[1][0] = 0.0f;
	translationMatrix.data[1][1] = 1.0f;
	translationMatrix.data[1][2] = 0.0f;
	translationMatrix.data[1][3] = 0.0f;
	translationMatrix.data[2][0] = 0.0f;
	translationMatrix.data[2][1] = 0.0f;
	translationMatrix.data[2][2] = 1.0f;
	translationMatrix.data[2][3] = 0.0f;
	translationMatrix.data[3][0] = x;
	translationMatrix.data[3][1] = y;
	translationMatrix.data[3][2] = z;
	translationMatrix.data[3][3] = 1.0f;
	return translationMatrix;
}

Matrix4x4 CreateRotationXMatrix(float angle) 
{
	Matrix4x4 rotationMatrix;
	rotationMatrix.data[0][0] = 1.0f;
	rotationMatrix.data[0][1] = 0.0f;
	rotationMatrix.data[0][2] = 0.0f;
	rotationMatrix.data[0][3] = 0.0f;
	rotationMatrix.data[1][0] = 0.0f;
	rotationMatrix.data[1][1] = cos(angle);
	rotationMatrix.data[1][2] = -sin(angle);
	rotationMatrix.data[1][3] = 0.0f;
	rotationMatrix.data[2][0] = 0.0f;
	rotationMatrix.data[2][1] = sin(angle);
	rotationMatrix.data[2][2] = cos(angle);
	rotationMatrix.data[2][3] = 0.0f;
	rotationMatrix.data[3][0] = 0.0f;
	rotationMatrix.data[3][1] = 0.0f;
	rotationMatrix.data[3][2] = 0.0f;
	rotationMatrix.data[3][3] = 1.0f;
	return rotationMatrix;
}

Matrix4x4 CreateRotationYMatrix(float angle) 
{
	Matrix4x4 result;
	result.data[0][0] = cos(angle);
	result.data[0][1] = 0.0f;
	result.data[0][2] = sin(angle);
	result.data[0][3] = 0.0f;
	result.data[1][0] = 0.0f;
	result.data[1][1] = 1.0f;
	result.data[1][2] = 0.0f;
	result.data[1][3] = 0.0f;
	result.data[2][0] = -sin(angle);
	result.data[2][1] = 0.0f;
	result.data[2][2] = cos(angle);
	result.data[2][3] = 0.0f;
	result.data[3][0] = 0.0f;
	result.data[3][1] = 0.0f;
	result.data[3][2] = 0.0f;
	result.data[3][3] = 1.0f;
	return result;
}

Matrix4x4 CreateRotationZMatrix(float angle) 
{
	Matrix4x4 result;
	result.data[0][0] = cos(angle);
	result.data[0][1] = -sin(angle);
	result.data[0][2] = 0.0f;
	result.data[0][3] = 0.0f;
	result.data[1][0] = sin(angle);
	result.data[1][1] = cos(angle);
	result.data[1][2] = 0.0f;
	result.data[1][3] = 0.0f;
	result.data[2][0] = 0.0f;
	result.data[2][1] = 0.0f;
	result.data[2][2] = 1.0f;
	result.data[2][3] = 0.0f;
	result.data[3][0] = 0.0f;
	result.data[3][1] = 0.0f;
	result.data[3][2] = 0.0f;
	result.data[3][3] = 1.0f;
	return result;
}

Vertex NDCtoScreen(Vertex vertex, int width, int height) 
{
	if (width == 0 || height == 0) {
		printf("Error: Width or height is zero.\n");
		return vertex; 
	}

	Vertex screenVertex;
	screenVertex.x = (vertex.x + 1) * (width / 2.0f);
	screenVertex.y = (1 - vertex.y) * (height / 2.0f);
	screenVertex.z = vertex.z; // assuming z-coordinate remains unchanged
	screenVertex.w = vertex.w; // assuming w-coordinate remains unchanged

	return screenVertex;
}

void MultiplyVertexByMatrix(Vertex& vertex, Matrix4x4& matrix) 
{
	Vertex transformedVertex;

	float x = vertex.x;
	float y = vertex.y;
	float z = vertex.z;
	float w = vertex.w;

	transformedVertex.x = matrix.data[0][0] * x + matrix.data[1][0] * y + matrix.data[2][0] * z + matrix.data[3][0] * w;
	transformedVertex.y = matrix.data[0][1] * x + matrix.data[1][1] * y + matrix.data[2][1] * z + matrix.data[3][1] * w;
	transformedVertex.z = matrix.data[0][2] * x + matrix.data[1][2] * y + matrix.data[2][2] * z + matrix.data[3][2] * w;
	transformedVertex.w = matrix.data[0][3] * x + matrix.data[1][3] * y + matrix.data[2][3] * z + matrix.data[3][3] * w;

	vertex = transformedVertex;
}

Matrix4x4 MultiplyMatrices(Matrix4x4& a, Matrix4x4& b) 
{
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.data[i][j] = 0.0f;
			for (int k = 0; k < 4; k++) {
				result.data[i][j] += a.data[i][k] * b.data[k][j];
			}
		}
	}
	return result;
}

bool IsAffine(const Matrix4x4& matrix) 
{
	// Check if the bottom row is [0, 0, 0, 1]
	if (matrix.data[3][0] != 0 || matrix.data[3][1] != 0 || matrix.data[3][2] != 0 || matrix.data[3][3] != 1) {
		return false;
	}

	// Check if the determinant of the top-left 3x3 submatrix is non-zero
	float det = matrix.data[0][0] * (matrix.data[1][1] * matrix.data[2][2] - matrix.data[1][2] * matrix.data[2][1]) -
		matrix.data[0][1] * (matrix.data[1][0] * matrix.data[2][2] - matrix.data[1][2] * matrix.data[2][0]) +
		matrix.data[0][2] * (matrix.data[1][0] * matrix.data[2][1] - matrix.data[1][1] * matrix.data[2][0]);
	if (det == 0) {
		return false;
	}

	return true;
}

bool IsOrthogonal(const Matrix4x4& matrix) 
{
	// Check if the dot product of any two different columns (or rows) is 0
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 3; j++) {
			float dotProduct = matrix.data[i][0] * matrix.data[j][0] +
				matrix.data[i][1] * matrix.data[j][1] +
				matrix.data[i][2] * matrix.data[j][2];
			if (dotProduct != 0) {
				return false;
			}
		}
	}

	// Check if the dot product of any column (or row) with itself is 1
	for (int i = 0; i < 3; i++) {
		float dotProduct = matrix.data[i][0] * matrix.data[i][0] +
			matrix.data[i][1] * matrix.data[i][1] +
			matrix.data[i][2] * matrix.data[i][2];
		if (dotProduct != 1) {
			return false;
		}
	}

	return true;
}

float DotProduct(const Vector3& v1, const Vector3& v2) 
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

Matrix4x4 GetInverseAffineOrthogonalMatrix(const Matrix4x4& matrix) 
{
	Matrix4x4 inverse;

	// Calculate the 3x3 transpose of the input matrix
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			inverse.data[i][j] = matrix.data[j][i];
		}
	}

	// Calculate the new position
	Vector3 axisX(matrix.data[0][0], matrix.data[0][1], matrix.data[0][2]);
	Vector3 axisY(matrix.data[1][0], matrix.data[1][1], matrix.data[1][2]);
	Vector3 axisZ(matrix.data[2][0], matrix.data[2][1], matrix.data[2][2]);
	Vector3 axisW(matrix.data[3][0], matrix.data[3][1], matrix.data[3][2]);

	inverse.data[3][0] = -DotProduct(axisX, axisW);
	inverse.data[3][1] = -DotProduct(axisY, axisW);
	inverse.data[3][2] = -DotProduct(axisZ, axisW);

	// Set default 0.0s and 1.0s for the .w components of the inverse matrix
	inverse.data[0][3] = 0.0f;
	inverse.data[1][3] = 0.0f;
	inverse.data[2][3] = 0.0f;
	inverse.data[3][3] = 1.0f;
	return inverse;
}

void CreateViewMatrix() 
{
	// Create a 4x4 translation matrix that translates backwards along the local Z-axis -1 unit and up along the Y-axis 1 unit
	Matrix4x4 translationMatrix = CreateTranslationMatrix(0.0, 0.0, -1.0);

	// Create a 4x4 rotation matrix rotated -18 degrees on the X axis
	float angle = -18.0 * 3.14159 / 180.0; // Convert degrees to radians
	Matrix4x4 rotationMatrix = CreateRotationXMatrix(angle);

	// Multiply the translation matrix by the rotation matrix to get the camera matrix
	Matrix4x4 cameraMatrix = MultiplyMatrices(translationMatrix, rotationMatrix);

	// Take the inverse of the camera matrix to get the view matrix
	viewMatrix = GetInverseAffineOrthogonalMatrix(cameraMatrix);
}

Matrix4x4 CreatePerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) 
{
	float scale = 1.0f / tan(fov/2);
	Matrix4x4 projectionMatrix;
	projectionMatrix.data[0][0] = scale;
	projectionMatrix.data[0][1] = 0.0f;
	projectionMatrix.data[0][2] = 0.0f;
	projectionMatrix.data[0][3] = 0.0f;
	projectionMatrix.data[1][0] = 0.0f;
	projectionMatrix.data[1][1] = scale * ASPECT_RATIO;
	projectionMatrix.data[1][2] = 0.0f;
	projectionMatrix.data[1][3] = 0.0f;
	projectionMatrix.data[2][0] = 0.0f;
	projectionMatrix.data[2][1] = 0.0f;
	projectionMatrix.data[2][2] = (farPlane + nearPlane) / (farPlane - nearPlane);
	projectionMatrix.data[2][3] = 1.0f;
	projectionMatrix.data[3][0] = 0.0f;
	projectionMatrix.data[3][1] = 0.0f;
	projectionMatrix.data[3][2] = (farPlane * nearPlane) / (farPlane - nearPlane);
	projectionMatrix.data[3][3] = 0.0f;
	return projectionMatrix;
}

void PerformPerspectiveDivide(Vertex& vertex) 
{
	vertex.x /= vertex.w;
	vertex.y /= vertex.w;
	vertex.z /= vertex.w;
}

