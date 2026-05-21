//
// Created by berke on 5/20/2026.
//

#include "../Headers/Projection.h"

#include <math.h>

#include "../Headers/config.h"
#include "../Headers/Matrix3.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define PI 3.14159265358979323846f

float deg_to_rad(float deg) {
    return deg * (PI / 180.0f);
}

float rad_to_deg(float rad) {
    return rad * (180.0f / PI);
}

static float edge_function(float ax, float ay, float bx, float by, float px, float py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

void clear_depth_buffer(float* depthBuffer) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        depthBuffer[i] = 0.0f;
    }
}

static float triangle_area_2d(TexturedVertex a, TexturedVertex b, TexturedVertex c) {
    return (b.position.x - a.position.x) * (c.position.y - a.position.y) -
           (b.position.y - a.position.y) * (c.position.x - a.position.x);
}

Vector3 view_transformation(const Vector3 pos, const Vector3 cameraDir, const Vector3 cameraPos) {
    Matrix3 first;
    first.m[0][0] = 1; first.m[1][0] = 0;                  first.m[2][0] = 0;
    first.m[0][1] = 0; first.m[1][1] = cosf(cameraDir.x);  first.m[2][1] = sinf(cameraDir.x);
    first.m[0][2] = 0; first.m[1][2] = -sinf(cameraDir.x); first.m[2][2] = cosf(cameraDir.x);

    Matrix3 second;
    second.m[0][0] = cosf(cameraDir.y); second.m[1][0] = 0; second.m[2][0] = -sinf(cameraDir.y);
    second.m[0][1] = 0;                 second.m[1][1] = 1; second.m[2][1] = 0;
    second.m[0][2] = sinf(cameraDir.y); second.m[1][2] = 0; second.m[2][2] = cosf(cameraDir.y);

    Matrix3 third;
    third.m[0][0] = cosf(cameraDir.z);  third.m[1][0] = sinf(cameraDir.z); third.m[2][0] = 0;
    third.m[0][1] = -sinf(cameraDir.z); third.m[1][1] = cosf(cameraDir.z); third.m[2][1] = 0;
    third.m[0][2] = 0;                  third.m[1][2] = 0;                third.m[2][2] = 1;

    const Vector3 fourth = vector3_subtract(pos, cameraPos);

    const Matrix3 fm = matrix3_multiply(&first, &second);
    const Matrix3 sm = matrix3_multiply(&fm, &third);
    const Vector3 tm = matrix3_multiply_vector3(&sm, fourth);

    return tm;
}

bool project_textured_vertex(
    Vector3 worldPos,
    Vector2 uv,
    Vector3 cameraDir,
    Vector3 cameraPos,
    TexturedVertex* out
) {
    Vector3 cameraSpace = view_transformation(worldPos, cameraDir, cameraPos);

    if (cameraSpace.z <= NEAR_PLANE) return false;

    const float fovRad = deg_to_rad(FOV);
    const float focalLength = ((float)SCREEN_WIDTH * 0.5f) / tanf(fovRad * 0.5f);

    out->position.x = focalLength * cameraSpace.x / cameraSpace.z + (float)SCREEN_WIDTH * 0.5f;
    out->position.y = (float)SCREEN_HEIGHT * 0.5f - focalLength * cameraSpace.y / cameraSpace.z;

    out->uv.x = uv.x;
    out->uv.y = uv.y;

    out->invZ = 1.0f / cameraSpace.z;

    return true;
}

int sign(const int p1x, const int  p1y, const int p2x, const int p2y, const int p3x, const int p3y) {
    return (p1x - p3x) * (p2y - p3y) - (p2x - p3x) * (p1y - p3y);
}

bool is_inside_triangle(const Vector2 pixel, const Triangle* tri) {
    const int d1 =
        sign(pixel.x, pixel.y, tri->vertices[0].position.x, tri->vertices[0].position.y, tri->vertices[1].position.x, tri->vertices[1].position.y);
    const int d2 =
        sign(pixel.x, pixel.y, tri->vertices[1].position.x, tri->vertices[1].position.y, tri->vertices[2].position.x, tri->vertices[2].position.y);
    const int d3 =
        sign(pixel.x, pixel.y, tri->vertices[2].position.x, tri->vertices[2].position.y, tri->vertices[0].position.x, tri->vertices[0].position.y);

    const bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    const bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

Triangle create_triangle(const Vector2 v1, const Vector2 v2, const Vector2 v3) {
    Triangle tri;

    tri.minX = (int)MIN(v1.x, MIN(v2.x, v3.x));
    tri.maxX = (int)MAX(v1.x, MAX(v2.x, v3.x));
    tri.minY = (int)MIN(v1.y, MIN(v2.y, v3.y));
    tri.maxY = (int)MAX(v1.y, MAX(v2.y, v3.y));

    tri.vertices[0].position = v1;
    tri.vertices[1].position = v2;
    tri.vertices[2].position = v3;

    return tri;
}

void render_textured_triangle(
    uint32_t* framebuffer,
    float* depthBuffer,
    const TexturedVertex v0,
    const TexturedVertex v1,
    const TexturedVertex v2,
    const Texture* texture
) {
    int minX = (int)floorf(MIN(v0.position.x, MIN(v1.position.x, v2.position.x)));
    int maxX = (int)ceilf(MAX(v0.position.x, MAX(v1.position.x, v2.position.x)));
    int minY = (int)floorf(MIN(v0.position.y, MIN(v1.position.y, v2.position.y)));
    int maxY = (int)ceilf(MAX(v0.position.y, MAX(v1.position.y, v2.position.y)));

    minX = MAX(0, minX);
    maxX = MIN(SCREEN_WIDTH - 1, maxX);
    minY = MAX(0, minY);
    maxY = MIN(SCREEN_HEIGHT - 1, maxY);

    const float area = edge_function(v0.position.x, v0.position.y, v1.position.x,
        v1.position.y, v2.position.x, v2.position.y);

    if (fabsf(area) < 0.0001f) return;

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            const float px = (float)x + 0.5f;
            const float py = (float)y + 0.5f;

            float w0 = edge_function(v1.position.x, v1.position.y, v2.position.x, v2.position.y, px, py) / area;
            float w1 = edge_function(v2.position.x, v2.position.y, v0.position.x, v0.position.y, px, py) / area;
            float w2 = edge_function(v0.position.x, v0.position.y, v1.position.x, v1.position.y, px, py) / area;

            if ((w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) ||
            (w0 <= 0.0f && w1 <= 0.0f && w2 <= 0.0f)) {
                // Perspective-correct UV interpolation
                const float invZ =
                    w0 * v0.invZ +
                    w1 * v1.invZ +
                    w2 * v2.invZ;

                const float uOverZ =
                    w0 * v0.uv.x * v0.invZ +
                    w1 * v1.uv.x * v1.invZ +
                    w2 * v2.uv.x * v2.invZ;

                const float vOverZ =
                    w0 * v0.uv.y * v0.invZ +
                    w1 * v1.uv.y * v1.invZ +
                    w2 * v2.uv.y * v2.invZ;

                const int index = y * SCREEN_WIDTH + x;

                if (invZ > depthBuffer[index]) {
                    const float u = uOverZ / invZ;
                    const float v = vOverZ / invZ;

                    framebuffer[index] = sample_texture(texture, u, v);
                    depthBuffer[index] = invZ;
                }
            }
        }
    }
}

void render_textured_quad(
    float* depthBuffer,
    uint32_t *framebuffer,
    const Quad *quad,
    Vector3 cameraDir,
    Vector3 cameraPos,
    const Texture *texture) {

    TexturedVertex topLeft;
    TexturedVertex topRight;
    TexturedVertex bottomLeft;
    TexturedVertex bottomRight;


    if (!project_textured_vertex(quad->vertices[0], (Vector2){0.0f, 0.0f}, cameraDir, cameraPos, &topLeft) ||
        !project_textured_vertex(quad->vertices[1], (Vector2){1.0f, 0.0f}, cameraDir, cameraPos, &topRight) ||
        !project_textured_vertex(quad->vertices[2], (Vector2){0.0f, 1.0f}, cameraDir, cameraPos, &bottomLeft) ||
        !project_textured_vertex(quad->vertices[3], (Vector2){1.0f, 1.0f}, cameraDir, cameraPos, &bottomRight)) {
        return;
    }

    render_textured_triangle(framebuffer, depthBuffer, bottomLeft, topLeft, topRight, texture);
    render_textured_triangle(framebuffer, depthBuffer, topRight, bottomRight, bottomLeft, texture);
}
