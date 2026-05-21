//
// Created by berke on 5/20/2026.
//

#include "../Headers/Vector3.h"

#include <math.h>

float vector3_length(const Vector3 vector) {
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

float vector3_distance(const Vector3 vector1, const Vector3 vector2) {
    float y = vector1.y - vector2.y;
    float x = vector1.x - vector2.x;
    float z = vector1.z - vector2.z;
    return sqrtf(x * x + y * y + z * z);
}

Vector3 vector3_add(const Vector3 vector1, const Vector3 vector2) {
    Vector3 result = {vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z};
    return result;
}
Vector3 vector3_subtract(const Vector3 vector1, const Vector3 vector2) {
    Vector3 result = {vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z};
    return result;
}
Vector3 vector3_normalized(const Vector3 vector) {
    const float length = vector3_length(vector);
    const Vector3 result = {vector.x / length, vector.y / length, vector.z / length};
    return result;
}
Vector3 vector3_multiply_f(const Vector3 vector, const float f) {
    const Vector3 result = {vector.x * f, vector.y * f, vector.z * f};
    return result;
}
