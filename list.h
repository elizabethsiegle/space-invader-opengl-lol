#ifndef __LIST__H
#define __LIST__H

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

typedef struct {
    GLdouble x;
    GLdouble y;
    GLint index;
    bool alive;
} Alien;

typedef struct bullet {
    GLdouble xPos;
    GLdouble yPos;
    GLdouble dx;
    GLdouble dy;
    bool active;
    GLint shootAngle;
} Bullet;

typedef struct Node {
    Bullet* data;
    struct Node* next;
    struct Node* h;
    struct Node* t;
} Node;

Bullet *makeBullet(GLdouble x, GLdouble y, GLdouble tx, GLdouble ty);
Node *makeNode(Bullet *b);

void addNode(Node *n, Node **llist);
void removeNode(Node *n, Node **llist);
void clearNodes(Node **llist);

#endif
