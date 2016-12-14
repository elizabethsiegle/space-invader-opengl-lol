#include "main.h"
GLint alienDeathAnimationValue[COLOFALIEN*ROWOFALIEN];
GLint numberOfgrids = VIEWSZIE/STEP;
GLdouble isLeftKeyPressed, isRightKeyPressed, isSpacePressed, isDownKeyPressed, shipShouldMove;
GLdouble shipXtrans, shipYtrans;
GLdouble alienXtrans = SHIPWID,alienYtrans = ALIENROWINT, alienGroupV = SHIPWID;
GLdouble fireMissileV = SHIPHEI;
GLdouble failTimeCount = FAILREAD;
GLfloat shipColor[3] = {1, 1, 1};
Alien alienGroup[COLOFALIEN*ROWOFALIEN];
Node *sentBullet;
Node *shipBullet;
GLint timeCount = 0, hitReadValue = 0, hitTotal = 0;
GLint freezeForDebug = false, NextStepInDebug;
GLint failFlag = false;
GLint level = 7; // define the interval of bumb releasing, best using a prime
GLint previousFireTime;
void dropBumb(int t) {
  //drop a bumb, twice the time of interval
  int enemyColIndex[COLOFALIEN]; // record the bottom most alive alien index
  for (int i=0; i<COLOFALIEN; i++) {
    enemyColIndex[i] = -1;
  } //for
  for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
    if (alienGroup[i].ifAlive) {
      enemyColIndex[i%COLOFALIEN] = i;
    } //if
  } //for
  if (timeCount % (t) == 0) {
    for (int i=0; i<COLOFALIEN; i++) {
      if((i) % level == 2 && enemyColIndex[i] > -1){ //randomize bumb droping
        Node* tmp = makeNode(make_Missile(alienGroup[enemyColIndex[i]].centerX, alienGroup[enemyColIndex[i]].centerY, alienXtrans, alienYtrans));
        addNode(tmp, &sentBullet);
      } //if
    } //for
  } //if
  if(sentBullet != NULL) { //move attacking missile
    for (Node* tmp = sentBullet->head; tmp != NULL; tmp = tmp->next) {
      tmp->data->transY -= ALIENROWINT;
    } //for
  } //if
} //if timeocunt

int ifWin() {
  for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
    if (alienGroup[i].ifAlive == true) {
      return 0;
    } //if
  } //for
  return 1;
}
int ifFail() {
  for (int i=COLOFALIEN*ROWOFALIEN; i>0; --i) {
    if(alienGroup[i].ifAlive == true) {
      if(alienGroup[i].centerY + alienYtrans <= -VIEWSZIE || hitTotal == NUMLIVES) {
        //printf("fail");
        return 1;
      } //if
    } //if
  } //for
  return 0;
}

void draw() {
  // clear to background color
  glClear(GL_COLOR_BUFFER_BIT);
  // check if no alien left(WIN)
  if (ifWin()) {
    glPushMatrix();
    glRotatef(timeCount, 0, 0, 1);
    glBegin(GL_POLYGON);
    glVertex2f(-VIEWSZIE/2.0, 0);
    glVertex2f(0, -VIEWSZIE/2.0);
    glVertex2f(VIEWSZIE/2.0, 0);
    glVertex2f(0, VIEWSZIE/2.0);
    glEnd();
    glPopMatrix();
  } //if

  // draw Aliens
  glPushMatrix();
  for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
    if(alienGroup[i].ifAlive || (!alienGroup[i].ifAlive && alienDeathAnimationValue[i] > -1)){
      for (float j = 3.0; j>0; j--) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(alienXtrans, alienYtrans, 0);
        glTranslatef(alienGroup[i].centerX + SHIPWID/2, alienGroup[i].centerY + SHIPWID/2, 1);
        glScalef(j/3, j/3, 1);
        glTranslatef(-alienGroup[i].centerX - SHIPWID/2, -alienGroup[i].centerY - SHIPWID/2, -1);
        if(!alienGroup[i].ifAlive && alienDeathAnimationValue[i] > -1) {
          glTranslatef(alienGroup[i].centerX + SHIPWID/2, alienGroup[i].centerY + SHIPWID/2, 0);
          glScalef(READVALUEUNIT/alienDeathAnimationValue[i], READVALUEUNIT/alienDeathAnimationValue[i], 1);
          glTranslatef(-(alienGroup[i].centerX + SHIPWID/2), -(alienGroup[i].centerY + SHIPWID/2), 0);
        } //if
        glTranslatef(alienGroup[i].centerX + SHIPWID/2, alienGroup[i].centerY + SHIPWID/2, 0);
        // glRotatef(angle, 0, 0, 1);
        glTranslatef(-(alienGroup[i].centerX + SHIPWID/2), -(alienGroup[i].centerY + SHIPWID/2), 0);
        glBegin(GL_POLYGON);
        glVertex2f(alienGroup[i].centerX + SHIPWID/2 - STEP*2/3, alienGroup[i].centerY + SHIPWID/2 - STEP/6);
        glVertex2f(alienGroup[i].centerX + SHIPWID/2 - STEP*2/3, alienGroup[i].centerY + SHIPWID/2 + STEP/6);
        glVertex2f(alienGroup[i].centerX + SHIPWID/2 - STEP/6, alienGroup[i].centerY + SHIPWID/2 + STEP*2/3);
        glVertex2f(alienGroup[i].centerX + SHIPWID/2 + STEP/6, alienGroup[i].centerY + SHIPWID/2 + STEP*2/3);
        glVertex2f(alienGroup[i].centerX + SHIPWID/2 + STEP*2/3, alienGroup[i].centerY + SHIPWID/2 + STEP/6);
        glVertex2f(alienGroup[i].centerX + SHIPWID/2 + STEP*2/3, alienGroup[i].centerY + SHIPWID/2 - STEP/6);
        glVertex2f(alienGroup[i].centerX + SHIPWID/2 + STEP/6, alienGroup[i].centerY + SHIPWID/2 - STEP*2/3);
        glVertex2f(alienGroup[i].centerX + SHIPWID/2 - STEP/6, alienGroup[i].centerY + SHIPWID/2 - STEP*2/3);
        glEnd();
        glPopMatrix();
      } //for
      if(!alienGroup[i].ifAlive && alienDeathAnimationValue[i] > -1) {
        // if(timeCount % (ALIENSPEED/2) == 0) {
        //   alienDeathAnimationValue[i] += READVALUEUNIT;
        // } //if
        // if (alienDeathAnimationValue[i] >= READVALUEUNIT*DEATHREAD) {
        //   alienDeathAnimationValue[i] = -1;
        // } //if
      } //if
    } //for
  } //if
  glPopMatrix();
  // check if fire and draw bullet
  if (isSpacePressed) {
    Node* tmp = makeNode(make_Missile(SHIPINIX, SHIPINIY, shipXtrans, 0));
    addNode(tmp, &shipBullet);
    isSpacePressed = false;
  } //if
  if(shipBullet != NULL) {
    for (Node* tmp = shipBullet->head; tmp != NULL; tmp = tmp->next) {
      GLdouble centerXofThis = tmp->data->centerX + STEP;
      GLdouble centerYofThis = tmp->data->centerY + SHIPHEI;
      if(tmp->data->ifValid == true) {
        glPushMatrix();
        glTranslatef(tmp->data->transX, tmp->data->transY, 0);
        glBegin(GL_POLYGON);
        glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis - SHIPHEI/3);
        glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis + SHIPHEI/3);
        glVertex2f(centerXofThis - SHIPHEI/3, centerYofThis + SHIPHEI*2/3);
        glVertex2f(centerXofThis + SHIPHEI/3, centerYofThis + SHIPHEI*2/3);
        glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis + SHIPHEI/3);
        glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis - SHIPHEI/3);
        glVertex2f(centerXofThis + SHIPHEI/3, centerYofThis - SHIPHEI*2/3);
        glVertex2f(centerXofThis - SHIPHEI/3, centerYofThis - SHIPHEI*2/3);
        glEnd();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(tmp->data->transX, tmp->data->transY, 0);
        glTranslatef(centerXofThis, centerYofThis, 1);
        glScalef(.4, .4, 1);
        glTranslatef(-centerXofThis, -centerYofThis, -1);
        glBegin(GL_POLYGON);
        glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis - SHIPHEI/3);
        glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis + SHIPHEI/3);
        glVertex2f(centerXofThis - SHIPHEI/3, centerYofThis + SHIPHEI*2/3);
        glVertex2f(centerXofThis + SHIPHEI/3, centerYofThis + SHIPHEI*2/3);
        glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis + SHIPHEI/3);
        glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis - SHIPHEI/3);
        glVertex2f(centerXofThis + SHIPHEI/3, centerYofThis - SHIPHEI*2/3);
        glVertex2f(centerXofThis - SHIPHEI/3, centerYofThis - SHIPHEI*2/3);
        glEnd();
        glPopMatrix();
        for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
          if (alienGroup[i].ifAlive) {
            if((centerXofThis + tmp->data->transX - alienGroup[i].centerX - alienXtrans - STEP)*(centerXofThis + tmp->data->transX - alienGroup[i].centerX - alienXtrans- STEP) +
            (centerYofThis +tmp->data->transY - alienGroup[i].centerY -alienYtrans- SHIPHEI/2)*(centerYofThis + tmp->data->transY - alienGroup[i].centerY - alienYtrans - SHIPHEI/2)
            < STEP*STEP) {
              alienGroup[i].ifAlive = false;
              alienDeathAnimationValue[i] = 0;
              tmp->data->ifValid = false;
            } //if
          } //if
        } //for
      } //if
    } //of
  }
// draw Attacking missile in the linked list
  if(sentBullet != NULL) {
    for (Node* tmp = sentBullet->head; tmp != NULL; tmp = tmp->next) {
      GLdouble centerXofThis = tmp->data->centerX + STEP;
      GLdouble centerYofThis = tmp->data->centerY + SHIPHEI;
      if(tmp->data->ifValid == true) {
        glPushMatrix();
        if((centerXofThis + tmp->data->transX - SHIPINIX - shipXtrans - STEP)*(centerXofThis + tmp->data->transX - SHIPINIX - shipXtrans- STEP) +
        (centerYofThis +tmp->data->transY - SHIPINIY - SHIPHEI/2)*(centerYofThis + tmp->data->transY - SHIPINIY - SHIPHEI/2)
        < STEP*STEP) {
          tmp->data->ifValid = false;
          hitReadValue = READVALUEUNIT*DEATHREAD;
          hitTotal++;
          //printf("HIT!!\n");
          exit(0);
        }
        glTranslatef(tmp->data->transX, tmp->data->transY, 0);
        glBegin(GL_POLYGON);
        glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis - SHIPHEI/6);
        glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis + SHIPHEI/6);
        glVertex2f(centerXofThis - SHIPHEI/6, centerYofThis + SHIPHEI*2/3);
        glVertex2f(centerXofThis + SHIPHEI/6, centerYofThis + SHIPHEI*2/3);
        glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis + SHIPHEI/6);
        glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis - SHIPHEI/6);
        glVertex2f(centerXofThis + SHIPHEI/6, centerYofThis - SHIPHEI*2/3);
        glVertex2f(centerXofThis - SHIPHEI/6, centerYofThis - SHIPHEI*2/3);
        glEnd();
        glPopMatrix();
      } //if
    } //if
  } //for
  // draw SHIP
  glColor3f(1, 1, 1);
  glPushMatrix();
  // update if key pressed, ship move horizontally animation

  if (!freezeForDebug) {
    if(shipShouldMove && isRightKeyPressed && shipXtrans <= VIEWSZIE - SHIPWID - STEP){
      shipXtrans += STEP;
      isRightKeyPressed -= STEP;
    } //if
    else if(shipShouldMove && isLeftKeyPressed && shipXtrans > SHIPWID - VIEWSZIE - STEP){
      shipXtrans -= STEP;
      isLeftKeyPressed -= STEP;
    }
    else{
      shipShouldMove = false;
      isRightKeyPressed = false;
      isLeftKeyPressed = false;
    }
  }
  else{
    printf("ship x:%f, y:%f \n", SHIPINIX+shipXtrans, SHIPINIY+shipYtrans);
  }
  if (hitReadValue > 0) {
    if (timeCount % (ALIENSPEED/3) == 0 ) {
      hitReadValue -= READVALUEUNIT;
    }
  }
  //ship body
  glPushMatrix();
  glTranslatef(shipXtrans, 0, 0);
  if(hitReadValue <= 0){
    glColor3f(1, 0, 0);
  }
  else {
    glColor3f(0, 1, 0);
 }
 glBegin(GL_POLYGON);
 glVertex2f(SHIPINIX + SHIPWID*4/5, SHIPINIY + 1.5*SHIPHEI);
 glVertex2f(SHIPINIX + SHIPWID*3.5/5, SHIPINIY + 2.5*SHIPHEI);
 glVertex2f(SHIPINIX + SHIPWID*1.5/5, SHIPINIY + 2.5*SHIPHEI);
 glVertex2f(SHIPINIX + SHIPWID/5, SHIPINIY + 1.5*SHIPHEI);
 glVertex2f(SHIPINIX + SHIPWID/5, SHIPINIY - SHIPHEI);
 glVertex2f(SHIPINIX + SHIPWID*4/5, SHIPINIY - SHIPHEI);
 glEnd();
 glPopMatrix();
 // glColor3f(.5, .5, .5);

 // check if alien reaches bottom or no life left
 if(ifFail()) {
   failFlag = true;
 }
 if (failFlag) {
   if (timeCount % 10 == 0 && failTimeCount > 0) {
     failTimeCount --;
   } //if
   glPushMatrix();
   glBegin(GL_POLYGON);
  //  glColor3f(failTimeCount/FAILREAD, failTimeCount/FAILREAD, failTimeCount/FAILREAD);
   glVertex2f(-VIEWSZIE, -VIEWSZIE);
   glVertex2f(VIEWSZIE, -VIEWSZIE);
   glVertex2f(VIEWSZIE, VIEWSZIE);
   glVertex2f(-VIEWSZIE, VIEWSZIE);
   glEnd();
   glPopMatrix();
 } //if
 // update alien movements

 if (freezeForDebug) {
   if (timeCount % (ALIENSPEED*2) < ALIENSPEED) {
     NextStepInDebug = false;
   } //if
 }
 else {
   timeCount++;
 }
 // MOVE ALIEN
 if(alienYtrans > 0) {
   if (timeCount % ALIENSPEED == 0) {
     if (alienXtrans < STEP || alienXtrans >= COLOFALIEN*STEP - STEP) {
       alienGroupV *= -1;
       alienYtrans -= SHIPHEI;
     } //if
     dropBumb(ALIENSPEED*2);
     alienXtrans += alienGroupV;
   }
 }
 else if(alienYtrans > -3*ALIENROWINT) {
   if (timeCount % (ALIENSPEED/2) == 0){
     if (alienXtrans < STEP || alienXtrans >= COLOFALIEN*STEP - STEP) {
       alienGroupV *= -1;
       alienYtrans -= SHIPHEI;
     } //if
     dropBumb(ALIENSPEED);
     alienXtrans += alienGroupV;
   } //if
 }
 else {
   if (timeCount % (ALIENSPEED/5) == 0) {
     if (alienXtrans < STEP || alienXtrans >= COLOFALIEN*STEP - STEP) {
       alienGroupV *= -1;
       alienYtrans -= SHIPHEI;
     } //if
     dropBumb(ALIENSPEED*2/5);
     alienXtrans += alienGroupV;
   } //if
 } //else

 //send ship bSullet
 if (timeCount % (ALIENSPEED/10) == 0) {
   if(shipBullet != NULL) {
     for (Node* tmp = shipBullet->head; tmp != NULL; tmp = tmp->next) {
       tmp->data->transY += SHIPHEI;
     } //for
   } //if
 } //if
 if(shipBullet != NULL){
   Node *tmp, *tmp2;
   for (tmp = shipBullet->head; tmp != NULL; tmp = tmp2) {
     tmp2 = tmp->next;
     if ((tmp->data->centerY + tmp->data->transY >VIEWSZIE) ||
     (tmp->data->centerY + tmp->data->transY <-VIEWSZIE) ||
     (tmp->data->centerX + tmp->data->transX >VIEWSZIE) ||
     (tmp->data->centerX + tmp->data->transX <-VIEWSZIE)) {
       tmp->data->ifValid = false;
       deleteNode(tmp, &shipBullet);
     } //if
   } //for
 } //if
 if (timeCount % 360 == 0) {
   timeCount = 0;
 } //if
 // if (angle % 360 == 0) {
 //   timeCount = 0;
 // } //if
}

void init() {
  // Set the clear color to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
  sentBullet = NULL;
  shipBullet = NULL;
  for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
    alienGroup[i].centerX = -VIEWSZIE + SHIPWID*(i%COLOFALIEN);
    alienGroup[i].centerY = VIEWSZIE - ALIENROWINT*(i/COLOFALIEN) - SHIPHEI*(ROWOFALIEN+1);
    alienGroup[i].ifAlive = true;
    alienGroup[i].index = i;
    alienDeathAnimationValue[i] = -1;
  }
}

void framebuffer_resize(GLFWwindow* w, GLint width, GLint height){
  glViewport(0, 0, width, height);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  switch(action) {
    case GLFW_PRESS :
      shipShouldMove = true;
      break;
    // case GLFW_RELEASE :
    //   break;
    case GLFW_REPEAT :
      shipShouldMove = true;
      break;
    default :
      printf("shouldn't happen\t");
      break;
  } //switch
  switch(key) {
    case GLFW_KEY_ESCAPE :
    case 'q' :
    case 'Q':
    if(sentBullet != NULL){
      clearNode(&sentBullet);
    }
    if(shipBullet != NULL){
      clearNode(&shipBullet);
    } //if
    glfwSetWindowShouldClose(w, GL_TRUE);
    break;
  case GLFW_KEY_LEFT:
    isLeftKeyPressed = SHIPWID;
    isRightKeyPressed = false;
    break;
  case GLFW_KEY_RIGHT:
    isRightKeyPressed = SHIPWID;
    isLeftKeyPressed = false;
    break;
  case GLFW_KEY_DOWN:
    isDownKeyPressed = true;
    if (freezeForDebug == true) {
      NextStepInDebug = true;
    } //if
    freezeForDebug = true;
    break;
  case GLFW_KEY_SPACE:
    if(timeCount - previousFireTime > FIRECOOLINGTIME || previousFireTime - timeCount > FIRECOOLINGTIME){
      isSpacePressed = true;
      previousFireTime = timeCount;
    }
    break;
  case 'r' :
  case 'R':
    freezeForDebug = false;
    break;
  default:
    break;
  }
}
int main(int argc, char **argv) {
  GLFWwindow* window;
  // Initialize the library
  if (!glfwInit())
    exit(EXIT_FAILURE);
  // Create a window and its OpenGL context
  window = glfwCreateWindow(WIDTH, HEIGHT, "hw2", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowPos(window, WIDTH, HEIGHT);
  // Make the window's context current
  glfwMakeContextCurrent(window);
  // Assign reshape() to be the function called whenever
  // a reshape event occurs
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);
  // Assign keyboard() to be the function called whenever
  // a key is pressed or released
  glfwSetKeyCallback(window, keyboard);
  init();
  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    draw();
    // Swap front and back buffers
    glfwSwapBuffers(window);
    // Poll for and process events
    glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
