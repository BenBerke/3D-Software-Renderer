https://github.com/user-attachments/assets/217ae787-5aea-4bc3-9c8a-2fdfc01d3382

# 3D Software Renderer

A lightweight, dependency-free 3D software renderer built from scratch in C, featuring custom linear algebra transformations, projection math, and a perspective-correct rasterizer. Written utilizing intrinsic SIMD 

---

## Features

* **Custom View Transformation Matrix:** Rotates and translates 3D world coordinates into camera space using manual Euler angle rotation matrices ($X$, $Y$, $Z$).
* **3D to 2D Perspective Projection:** Projects 3D camera-space points onto a 2D viewport based on field of view (FOV) and focal length.
* **Barycentric Rasterization:** Utilizes 2D edge functions to accurately determine pixel coverage inside triangles.
* **Perspective-Correct Texture Mapping:** Interpolates UV coordinates accurately using $\frac{1}{Z}$ depth coordinates to prevent texture warping.
* **Z-Buffering:** Includes a linear depth buffer implementation to handle correct surface overlapping and visibility tracking.
* **Primitive Assembly:** Supports rendering textured triangles and automatic quad-to-triangle splitting.

---
## Performance

Benchmarked on an **AMD Ryzen 7 6800H** at a framebuffer resolution of **1680 × 960**.

- **400 cubes** (**2,400 quads** - **4,800 triangles**)
- Approximately **4.3 ms rasterization time**
- Approximately **120 FPS total**

---

## Core Architecture & Math Implemented

### 1. View Transformation
Transforms a point from world space to camera space using a combined matrix derived from camera position and direction angles:
* Applies individual $X$, $Y$, and $Z$ rotation matrices via standard trigonometric calculations.
* Translates world positions relative to the camera vector.

### 2. Projection
Points behind the defined `NEAR_PLANE` are cleanly clipped. Valid vertices are mapped to screen space coordinates using perspective division

### 3. Perspective-Correct Interpolation
To solve the classic perspective mapping distortion issue, texture coordinates are linearly interpolated across the triangle face in screen space using the inverse of Z

---

## File Structure Dependencies

This source code expects a specific project hierarchy to resolve headers:
```text
├── Headers/
│   ├── Projection.h   # Geometric primitives (Quad, Triangle, TexturedVertex)
│   ├── config.h       # Engine configurations (SCREEN_WIDTH, SCREEN_HEIGHT, FOV, NEAR_PLANE)
│   ├── Matrix3.h      # Matrix definitions and vector multi-functions
└── Source/
    └── Projection.c   # Main rendering & rasterization pipeline (this file)
