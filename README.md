# Tools Cryptid Soft Rasterizer

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![Platform](https://img.shields.io/badge/platform-Windows%20x64-lightgrey)
![Renderer](https://img.shields.io/badge/renderer-Software%20%E2%80%94%20No%20GPU%20API-darkgrey)
![Build](https://img.shields.io/badge/build-Visual%20Studio%202022-purple)

A **software rasterizer built entirely from scratch in C++** — no GPU API, no graphics library. Every pixel is computed in software: 3D geometry is transformed through a hand-built matrix pipeline, projected to screen space, rasterized with barycentric interpolation, and written directly into a raw pixel back buffer rendered to a Win32 window.

Two cubes spin in real time against a 3D grid, demonstrating the full graphics pipeline running on the CPU.

> Part of the Squatchworks portfolio. Developed by [Jacob Blackburn](https://github.com/Squatchworks).

---

## What It Demonstrates

This project implements the foundational graphics pipeline that GPUs execute in hardware — rebuilt entirely in software:

| Stage | Implementation |
|---|---|
| **Vertex transformation** | Model → World → View → Clip space via `MultiplyVertexByMatrix` |
| **Perspective projection** | Hand-built `CreatePerspectiveMatrix` from FOV, aspect ratio, near/far planes |
| **Perspective divide** | `PerformPerspectiveDivide` — homogeneous W division to NDC |
| **NDC → Screen mapping** | `NDCtoScreen` — viewport transform to pixel coordinates |
| **Rasterization** | Bresenham-style line rasterizer (`DrawLine`) and barycentric triangle fill (`FillTriangle`) |
| **Depth buffering** | Per-pixel float depth buffer; `plotPixelWithDepth` rejects occluded fragments |
| **Alpha blending** | Per-channel RGBA interpolation via `AlphaBlend` |
| **Programmable shaders** | Function pointer `VertexShader` and `PixelShader` — same concept as GPU shader stages |

---

## Features

- Two independently transformed cubes rendered simultaneously
  - Cube 1: continuous Y-axis rotation driven by a `std::chrono` frame timer
  - Cube 2: pre-composed TRS (Translate × Rotate × Scale) world matrix, scaled to 30%
- 3D reference grid (11×11 lines, world-space) rendered via the same vertex/line pipeline
- Camera positioned with a -18° X-axis tilt and Z offset, inverted via `GetInverseAffineOrthogonalMatrix` to produce the view matrix
- Depth buffer cleared each frame and tested per pixel to handle front-to-back occlusion correctly
- All transforms represented as row-major `Matrix4x4` structs with hand-written multiply, rotate X/Y/Z, scale, translate, and inverse functions
- Back buffer written directly to a Win32 `RasterSurface` window each frame

---

## Architecture
Week3/
├── Main.cpp # Entry point — setup, game loop, per-frame transform and draw calls
├── Defines.h # All global state: back buffer, depth buffer, vertex definitions,
│ # Matrix4x4, cube geometry, grid geometry, world/view/projection matrices
├── MyMath.h # Full math library — matrix ops, vertex transform, NDC→screen,
│ # perspective matrix, view matrix, inverse affine, alpha blend, BGRA convert
├── RasterFunc.h # Rasterizer — clearScreen, plotPixel, plotPixelWithDepth, BLIT,
│ # DrawLine (Bresenham), FillTriangle (barycentric), DrawGrid, DrawCube
├── Shaders.h # Programmable shader stage — VS_World (MVP transform + perspective divide),
│ # PS_White, PS_Green — assigned via function pointer each draw call
├── RasterSurface.h # Win32 window abstraction (RS_Initialize, RS_Update, RS_Shutdown)
├── RasterSurface.cpp # Win32 window implementation
├── XTime.h / .cpp # High-resolution frame timer using QueryPerformanceCounter
└── Week3.sln # Visual Studio 2022 solution

**Shader pipeline:** `VertexShader` and `PixelShader` are global function pointers set before each draw call — `VS_World` applies the full MVP transform chain and perspective divide; pixel shaders run per-pixel after rasterization. This mirrors how real GPU pipelines bind shader programs per draw call.

---

## How to Build

### Prerequisites
- Visual Studio 2022 with the **Desktop development with C++** workload
- Windows 10/11 x64

### Build steps

1. Clone the repo: git clone https://github.com/Squatchworks/Tools_Cryptid_SoftRasterizer.git
2. Open `Week3/Week3.sln` in Visual Studio 2022.
3. Set configuration to **Debug | x64** or **Release | x64**.
4. **Build → Build Solution** (`Ctrl+Shift+B`).
5. Run — an 800×500 window will open showing the two spinning cubes.

No external dependencies. No package manager required.

---

## Lessons Learned

- **Row-major vs. column-major matters** — `MultiplyVertexByMatrix` accesses `matrix.data[column][row]` which means transforms chain right-to-left in matrix multiply order. Getting this wrong produces silent garbage output, not a crash.
- **Depth buffer must clear every frame** — forgetting to reset `depthBuffer[i] = 1.0f` each frame causes fragments from previous frames to permanently occlude new geometry.
- **Barycentric coordinates unify rasterization and interpolation** — the same alpha/beta/gamma weights used to test if a pixel is inside a triangle can interpolate depth, color, or UV coordinates for free. This is the foundation of how real GPU fragment shaders work.
- **Function pointer shader stages** — assigning `VertexShader = VS_World` before a draw call and `PixelShader = PS_White` per primitive is a simple but accurate model of how GPU pipeline state objects bind shader programs. Extending this pattern with per-pixel color interpolation or texture sampling is a natural next step.
- **`GetInverseAffineOrthogonalMatrix`** — for an affine orthogonal matrix, the inverse is just the transpose of the rotation + negated dot products for translation. Deriving and implementing this from first principles is a common graphics interview topic.

---

## Roadmap

### Current build
- Full MVP software rasterizer with depth buffer
- Programmable vertex/pixel shader function pointer pipeline
- Two animated cubes + 3D reference grid
- Per-pixel alpha blending and BGRA color conversion

### Potential extensions
- Texture mapping via UV interpolation (UVs already stored in `Vertex` struct)
- Gouraud shading via per-vertex color interpolation across triangles
- Backface culling via cross product winding order test
- Frustum clipping before rasterization

---

## Author

**Jacob Blackburn** — Gameplay programmer and C++ developer based in Russells Point, Ohio.
- GitHub: [@Squatchworks](https://github.com/Squatchworks)
- LinkedIn: [linkedin.com/in/squatchworks](https://linkedin.com/in/squatchworks)
