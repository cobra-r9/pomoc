#ifndef EXTMATH_H
#define EXTMATH_H

// -------------------------------- declarations and definitions of all structs go here.

// define a struct for 2d vector.
typedef struct Vec2D {
    double i;
    double j;
} Vec2D;



// -------------------------------- only declarations of the functions go here. 
// the math modulo function 
int modulo(int a, int b);

// add two 2d vectors, return 2d vector. 
Vec2D add2_vec2d(Vec2D vec1, Vec2D vec2);
Vec2D add_vec2d(int count, ...);

#endif
