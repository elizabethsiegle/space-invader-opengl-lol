#include "list.h"
Missile *make_Missile(GLdouble x, GLdouble y, GLdouble transX, GLdouble transY) {
  Missile *newMissile;
  if ( (newMissile = (Missile *) malloc( sizeof(Missile) ) )!= 0) {
    newMissile->centerX = x;
    newMissile->centerY = y;
    newMissile->transX = transX;
    newMissile->transY = transY;
    newMissile->ifValid = true;
  }
  else {
    printf("Out of memory!\n");
    exit(0);
  }
  return newMissile;
}
Node *makeNode (Missile* x) {
  Node *newN;
  if ( (newN = (Node *) malloc( sizeof(Node) ) )!= 0) {
    newN->data = x;
    newN->next = NULL;
    newN->head = NULL;
    newN->tail = NULL;
  }
  else {
    printf("Out of memory!\n");
    exit(0);
  }
  return newN;
}
void addNode (Node *p, Node** list) {
  if (*list == NULL) {
    (*list) = p;
    (*list)->head = p;
    (*list)->tail = p;
  }
  else {
    (*list)->tail->next = p;
    (*list)->tail = p;
  }
}

void deleteNode(Node* p, Node** list){
  Node *tmp, *tmp2;
  if ((p == (*list)->head) && (p ==(*list)->tail)){
    (*list)->head = (*list)->tail = NULL;
  }
  else if(p == (*list)->head) {
    (*list)->head = p->next;
  }
  else {
    for (tmp2 = (*list)->head, tmp=NULL; tmp2 != p; tmp2 = tmp2->next);
    if (p == (*list)->tail) {
      (*list)->tail = tmp;
    } //if
    tmp->next = p->next;
  } //else
}

void clearNode(Node** list) {
  Node *tmp, *tmp2;
  for (tmp = (*list)->head; tmp != NULL; tmp = tmp2) {
    tmp2 = tmp->next;
    free(tmp->data);
    free(tmp);
  }
  (*list)->head = (*list)->tail = NULL;
}
