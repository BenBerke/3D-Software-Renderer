//
// Created by berke on 5/20/2026.
//

#ifndef SOFTWARERENDERER_MATRIX3_H
#define SOFTWARERENDERER_MATRIX3_H
#include "Vector3.h"

typedef struct Matrix3 {
    float m[3][3];
} Matrix3;

Matrix3 matrix3_multiply(Matrix3* m, Matrix3* n);
Matrix3 matrix3_add(Matrix3* m, Matrix3* n);
Matrix3 matrix3_subtract(const Matrix3* m, const Matrix3* n);

Vector3 matrix3_multiply_vector3(const Matrix3* m, const Vector3 v);

#endif //SOFTWARERENDERER_MATRIX3_H