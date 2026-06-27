// name : extmath.c 
// date : 2026-06-16 
// std  : C 23 standard 


//---------------------------------- include header files -------------------------------
#include <stdarg.h>
#include "extmath.h"

//---------------------------------- function definitions go here ----------------------- 
// Add a mathematical modulo function. 
int modulo(int a, int b) {
    return a % b < 0 ? ((a % b) + b) % b : a % b;
}

// Function for adding 2 2d vectors. 
Vec2D add2_vec2d(Vec2D vec1, Vec2D vec2) {
    return (Vec2D){ .i = vec1.i + vec2.i, .j = vec1.j + vec2.j };
}

// Function for adding as many as 2d vectors we need. 
Vec2D add_vec2d(int count, ...) {
    Vec2D result = {0.0, 0.0};

    // initialise a va_list type. 
    va_list args;

    // va_start(va_list args, last named argument)
    // function signature : va_start(va_list args, parmN) : where parmN is the parameter name. 
    // our last named argument here is the count. 
    va_start(args, count); 

    // for loop the arguments if less than the count. 
    for (int i = 0; i < count; i++) {
        Vec2D v = va_arg(args, Vec2D);
        result.i += v.i;
        result.j += v.j;
    }

    va_end(args);
    return result;

    // usage : add_vec2d(3, v1, v2, v3);
}
