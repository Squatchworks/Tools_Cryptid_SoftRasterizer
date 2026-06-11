#pragma once

const int HEIGHT = 500;
const int WIDTH = 800;
const int NUM_PIXELS = WIDTH * HEIGHT;
unsigned int* BackBuffer = new unsigned int[NUM_PIXELS];
float* depthBuffer = new float[NUM_PIXELS];
float fov = 90.0f * (3.14159 / 180.0f); // Convert degrees to radians
const float ASPECT_RATIO = static_cast<float>(WIDTH) / HEIGHT;
float nearPlane = 0.1f;
float farPlane = 10.0f;
const float MAX_COLOR_VALUE = 255.0f;
const unsigned int WHITE_COLOR = 0xFFFFFFFF;
const unsigned int GREEN_COLOR = 0xFF00FF00;

struct Vector3 
{
	float x, y, z;
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
};

struct Vertex 
{
	float x, y, z;
	float w;
	unsigned int color;
	float u, v;
};

struct LinePair 
{
	Vertex Start;
	Vertex End;
};

struct Matrix4x4 
{
	float data[4][4];
};

const Matrix4x4 Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

Matrix4x4 SV_WorldMatrix = Identity;

Matrix4x4 gridWorldMatrix = Identity;

Matrix4x4 cubeWorldMatrix =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, .25, 0, 1
};

Matrix4x4 secondCubeWorldMatrix = Identity;

Matrix4x4 viewMatrix = Identity;

Matrix4x4 ProjectionMatrix = Identity;

LinePair gridVertices[22];

void LoadGridLines()
{
	for (int i = 0; i < 10; i++) {
		float x = -0.5f + (i * 0.1f);
		Vertex left = { x, 0.0f, -0.5f, 1, 0xFFFFFFFF };
		Vertex right = { x, 0.0f, 0.5f, 1, 0xFFFFFFFF };
		gridVertices[i] = { left, right };
	}

	for (int i = 0; i < 10; i++) {
		float z = -0.5f + (i * 0.1f);
		Vertex left = { -0.5f, 0.0f, z, 1, 0xFFFFFFFF };
		Vertex right = { 0.5f, 0.0f, z, 1, 0xFFFFFFFF };
		gridVertices[i + 10] = { left, right };
	}
}

Vertex TopFrontLeft = { -0.25f, 0.25f, -0.25f, 1, 0xFFFFFFFF, 0.0f, 0.0f};
Vertex TopFrontRight = { 0.25f, 0.25f, -0.25f, 1, 0xFFFFFFFF, 1.0f, 0.0f};
Vertex BottomFrontLeft = { -0.25f, -0.25f, -0.25f, 1, 0xFFFFFFFF, 1.0f, 1.0f};
Vertex BottomFrontRight = { 0.25f, -0.25f, -0.25f, 1, 0xFFFFFFFF, 0.0f, 1.0f};

Vertex TopBackLeft = { -0.25f, 0.25f, 0.25f, 1, 0xFFFFFFFF, 0.0f, 0.0f};
Vertex TopBackRight = { 0.25f, 0.25f, 0.25f, 1, 0xFFFFFFFF, 1.0f, 0.0f};
Vertex BottomBackLeft = { -0.25f, -0.25f, 0.25f, 1, 0xFFFFFFFF, 1.0f, 1.0f};
Vertex BottomBackRight = { 0.25f, -0.25f, 0.25f, 1, 0xFFFFFFFF, 0.0f, 1.0f};

LinePair TopFront = { TopFrontLeft, TopFrontRight };
LinePair BottomFront = { BottomFrontLeft, BottomFrontRight };
LinePair LeftFront = { BottomFrontLeft, TopFrontLeft };
LinePair RightFront = { BottomFrontRight, TopFrontRight };
LinePair TopBack = { TopBackLeft, TopBackRight };
LinePair BottomBack = { BottomBackLeft, BottomBackRight };
LinePair LeftBack = { BottomBackLeft, TopBackLeft };
LinePair RightBack = { BottomBackRight, TopBackRight };
LinePair TopLeftFRBA = { TopFrontLeft, TopBackLeft };
LinePair ToPRightFRBA = { TopFrontRight, TopBackRight };
LinePair BottomLeftFRBA = { BottomFrontLeft, BottomBackLeft };
LinePair BottomRightFRBA = { BottomFrontRight, BottomBackRight };

LinePair CubeLines[12];

void LoadCube()
{
	CubeLines[0] = TopFront;
	CubeLines[1] = BottomFront;
	CubeLines[2] = LeftFront;
	CubeLines[3] = RightFront;
	CubeLines[4] = TopBack;
	CubeLines[5] = BottomBack;
	CubeLines[6] = LeftBack;
	CubeLines[7] = RightBack;
	CubeLines[8] = TopLeftFRBA;
	CubeLines[9] = ToPRightFRBA;
	CubeLines[10] = BottomLeftFRBA;
	CubeLines[11] = BottomRightFRBA;
}
