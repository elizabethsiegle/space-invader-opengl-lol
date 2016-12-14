#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "list.h"
#include <stdbool.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#define WIDTH 700
#define HEIGHT 800
#define VIEWSZIE 1
#define STEP .1 //STEP IS HALF SHIPWID
#define SHIPINIX 0
#define SHIPINIY -.85
#define SHIPWID .2
#define SHIPHEI .05
#define COLOFALIEN 7
#define ROWOFALIEN 5
#define ALIENROWINT .2 // alien radius
#define ALIENSPEED 50 // second
#define FIRECOOLINGTIME 25 // half a second
#define DEATHREAD 5
#define READVALUEUNIT 20.0
#define NUMLIVES 5
#define FAILREAD 5

void alienShoot(int numTime);
