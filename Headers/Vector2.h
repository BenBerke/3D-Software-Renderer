//
// Created by berke on 3/11/2026.
//

#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct {
    float x, y;
} Vector2;

float vector2_length(Vector2 vector);
float vector2_distance(Vector2 vector1, Vector2 vector2);
Vector2 vector2_add(Vector2 vector1, Vector2 vector2);
Vector2 vector2_subtract(Vector2 vector1, Vector2 vector2);
Vector2 vector2_normalized(Vector2 vector);
Vector2 vector2_multiply_with_float(Vector2 vector1, float f);
void vector2_swap(Vector2* a, Vector2* b);



#endif //VECTOR2_H