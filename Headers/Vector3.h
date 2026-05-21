//
// Created by berke on 5/20/2026.
//

#ifndef SOFTWARERENDERER_VECTOR3_H
#define SOFTWARERENDERER_VECTOR3_H

typedef struct {
    float x, y, z;
} Vector3;

float vector3_length(Vector3 vector);
float vector3_distance(Vector3 vector1, Vector3 vector2);
Vector3 vector3_add(Vector3 vector1, Vector3 vector2);
Vector3 vector3_subtract(Vector3 vector1, Vector3 vector2);
Vector3 vector3_normalized(Vector3 vector);
Vector3 vector3_multiply_f(Vector3 vector1, float f);

#endif //SOFTWARERENDERER_VECTOR3_H