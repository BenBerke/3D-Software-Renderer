//
// Created by berke on 5/20/2026.
//
#include "../Headers/Matrix3.h"

Matrix3 matrix3_multiply(Matrix3* m, Matrix3* n) {
    Matrix3 result;

    const float
          a = m->m[0][0], b = m->m[1][0], c = m->m[2][0],
          e = m->m[0][1], d = m->m[1][1], f = m->m[2][1],
          g = m->m[0][2], h = m->m[1][2], j = m->m[2][2];

    const float
          x = n->m[0][0], y = n->m[1][0], z = n->m[2][0],
          w = n->m[0][1], q = n->m[1][1], r = n->m[2][1],
          p = n->m[0][2], s = n->m[1][2], t = n->m[2][2];

    result.m[0][0] = a * x + b * w + c * p; result.m[1][0] = a * y + b * q + c * s; result.m[2][0] = a * z + b * r + c * t;
    result.m[0][1] = e * x + d * w + f * p; result.m[1][1] = e * y + d * q + f * s; result.m[2][1] = e * z + d * r + f * t;
    result.m[0][2] = g * x + h * w + j * p; result.m[1][2] = g * y + h * q + j * s; result.m[2][2] = g * z + h * r + j * t;

    return result;
}
Matrix3 matrix3_add(Matrix3* m, Matrix3* n) {
    Matrix3 result;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            result.m[i][j] = m->m[i][j] + n->m[i][j];
        }
    }
    return result;
}
Matrix3 matrix3_subtract(const Matrix3* m, const Matrix3* n) {
    Matrix3 result;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            result.m[i][j] = m->m[i][j] - n->m[i][j];
        }
    }
    return result;
}

Vector3 matrix3_multiply_vector3(const Matrix3* m, const Vector3 v) {
    const float
          a = m->m[0][0], b = m->m[1][0], c = m->m[2][0],
          d = m->m[0][1], e = m->m[1][1], f = m->m[2][1],
          g = m->m[0][2], h = m->m[1][2], j = m->m[2][2];

    const float x = v.x, y = v.y, z = v.z;

    const float newX = a * x + b * y + c * z;
    const float newY = d * x + e * y + f * z;
    const float newZ = g * x + h * y + j * z;

    return (Vector3){newX, newY, newZ};
}