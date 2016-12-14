#ifndef __SHIP__H
#define __SHIP__H
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
typedef struct alien {
    GLdouble x;
    GLdouble y;
    GLint index;
    bool alive;
} Alien;
void checkAndFire(); //GLint x, GLint y, GLint w, GLint h
void drawShip();

#endif
