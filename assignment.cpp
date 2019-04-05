//----------------------------------------------------------------------------------//
//  Gavin McGill                                                                    //
//                                                                                  //
//  FILE NAME: assignment.cpp                                                       //
//  2018                                                                            //
//----------------------------------------------------------------------------------//

#include <iostream>
#include <cmath> 
#include <GL/glut.h>
#include "loadTGA.h"
#include "loadBMP.h"
#define DIR "media/skybox/"
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F   //To get rid of seams between textures

GLUquadricObj*	q;
GLuint txId[8];   //Texture ids
float angle=0, look_x, look_z=-1., eye_x, eye_z;  //Camera parameters
float eye_y = 0;
float spongetheta = 0;
int thetapls = 1;
int spongestep = 0;
float chestopen = 0;
int cheststate = 0;
int bubbles = 10;

float garryposx = 0;
float garryposz = 0;
float garrynx = 10;
float garrynz = 10;
int garryrot = 45;

int camerastate = 1;
int epressed = 0;

float bubbleup[15] = {0,1,2,3,4,4.5,13,5.5,15,6.5,17.5,1.2};
float bubblex[15] = {0.5,0.4,0.1,0.3,0.1,0.5,0.4,0.1,0.3,0.1};
float bubblez[15] = {0.4,0.3,0.5,0.2,0.3,0.5,0.4,0.1,0.3,0.1};

bool* keyStates = new bool[256];

float carposx = 10;
float carposz = 4;
float carrot = 0;
float carRotobj = 1.5;
float carspeed = 0;

float carposx2 = 200;
float carposz2 = -4;
float carrot2 = 0;
float carRotobj2 = 0;
float carspeed2 = 0.5;

float yellow[4] = {1, 0.9, 0.1, 0.8};
float greyblue[4] = {1, 0.9, 0.1, 0.8};
float brown[4] = {1, 0.9, 0.1, 0.8};
float grey[4] = {0.2, 0.2, 0.2, 0.2};
float white[4]  = {0.5, 0.5, 0.5, 0.5};

const int N = 25; 
float vx[N] = {0,0,0,1.1,1.2,1.25,1.3,1.4,1.5,1.55,1.65,1.7,1.72,1.74,1.7,1.68,1.60,1.55,1.5,1.4,1.39,1.136,1.2,1.1,0};
float vy[N] = {0,0,0.2,0.4,0.5,0.7,0.8,0.9,1.1,1.5,2.5,3,3.2,3.5,3.7,3.8,3.9,4,4.1,4.1,4.3,4.4,4.5,4.6,4.7};
float vz[N] = {0};

float getCarDist(){// here
    float xdif = carposx - eye_x;
    float zdif = carposz - eye_z;
    float totaldiff = sqrt((xdif*xdif)+(zdif*zdif));
    return totaldiff;
}

float getGaryDist(){// here
    float xdif = carposx - ((int)garryposx / 1000);
    printf("x:%f\n",xdif);
    float zdif = carposz - ((int)garryposz / 1000);
    printf("z:%f\n",zdif);
    float totaldiff = sqrt((xdif*xdif)+(zdif*zdif));
    return totaldiff;
}

void carTimer(){
    carposx -= carspeed * cos(carrot);
    carposz -= carspeed * sin(carrot);
    
    if (carposx < -300){
        carposx = 299;
    }
    if (carposx > 300){
        carposx = -299;
    }
    if (carRotobj > carrot + 0.02){
        carrot += 0.03;
    }else if(carRotobj < carrot - 0.02){
        carrot -= 0.03;
    }else{
        carrot = carRotobj;
        }
    

}

void carTimer2(){
    carposx2 -= carspeed2 * cos(carrot2);
    carposz2 -= carspeed2 * sin(carrot2);
    
    if (carposx2 < -300){
        carposx2 = 299;
    }
    if (carposx2 > 300){
        carposx2 = -299;
    }
    
    if (carRotobj2 > carrot2 + 0.02){
        carrot2 += 0.03;
    }else if(carRotobj2 < carrot2 - 0.02){
        carrot2 -= 0.03;
    }else{
        carrot2 = carRotobj2;
        }
    

}
//--------------------------------------------------------------------------------
void timer(int value){
    carTimer();
    carTimer2();
    
    if (garryposz < garrynz){
        garryposz += 1;
    } else if (garryposz > garrynz) {
        garryposz -= 1;
    } else {
        garrynz = rand() % 1000;
    }
    
    
    if (garryposx < garrynx){
        garryposx += 1;
    } else if (garryposx > garrynx) {
        garryposx -= 1;
    } else {
        garrynx = rand() % 1000;
    }
    
    if (garryposz < garrynz){
        if (garryposx < garrynx){
            garryrot = -135;
        } else if (garryposx > garrynx) {
            garryrot = -45;
        }
    } else if (garryposz > garrynz) {
        if (garryposx < garrynx){
            garryrot = 135;
        } else if (garryposx > garrynx) {
            garryrot = 45;
        }
    }
    
    
for(int i = 0; i < bubbles; i++){
    bubbleup[i] += 0.1;
    if (bubbleup[i] > 20){
            bubbleup[i] = 0;
            bubblez[i] = (rand()%50)/50;
            bubblex[i] = (rand()%50)/25;
        }
    }
    
    spongestep++;
    
    if (cheststate == 0){
           chestopen -= 2; 
    if (chestopen < -80){
        cheststate = 1;
        }
    }
    
    if (cheststate == 1){
           chestopen -= 0.01; 
    if (chestopen < -85){
        cheststate = 2;
        }
    }
    
    if (cheststate == 2){
           chestopen += 2; 
    if (chestopen > -2){
        cheststate = 3;
        }
    }
    
    if (cheststate == 3){
           chestopen += 0.005; 
    if (chestopen > 0){
        cheststate = 0;
        }
    }
    
    if (spongestep > 360){
        spongestep = 0;
        }
    if (thetapls == 0){
        spongetheta -= 5;
        
    if (spongetheta < -50){
            thetapls = 1;
        }
    }else{
        spongetheta += 5;
        
        if (spongetheta > 50){
            thetapls = 0;
            }
        }
glutPostRedisplay();
glutTimerFunc(2, timer, 0);
}
//--------------------------------------------------------------------------------
void loadTexture(){
    
	glGenTextures(8, txId); 	// Create 2 texture ids
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("media/Wall.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("media/floor.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
    glBindTexture(GL_TEXTURE_2D, txId[2]);
	loadTGA(DIR "left.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** front ***
	glBindTexture(GL_TEXTURE_2D, txId[3]);
	loadBMP("media/squid.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** right ***
	glBindTexture(GL_TEXTURE_2D, txId[4]);
	loadBMP("media/door.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** back***
	glBindTexture(GL_TEXTURE_2D, txId[5]);
	loadBMP(DIR "down.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** top ***
	glBindTexture(GL_TEXTURE_2D, txId[6]);
	loadTGA("media/wooden.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// *** down ***
	glBindTexture(GL_TEXTURE_2D, txId[7]);
	loadTGA(DIR "down.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    

}
//--------------------------------------------------------------------------------
void special(int key, int x, int y){
    
    
	if(key == GLUT_KEY_LEFT) {
        if (camerastate == 1){
            angle -= 0.1;  //Change direction
        }
    }
	else if(key == GLUT_KEY_RIGHT) {
        if (camerastate == 1){
            angle += 0.1;
        }
    }
    
    
	else if(key == GLUT_KEY_DOWN)
	{  //Move backward
        if (camerastate == 1){
		eye_x -= 0.2*sin(angle);
		eye_z += 0.2*cos(angle);
        }
	}
	else if(key == GLUT_KEY_UP)
	{ //Move forward
        if (camerastate == 1){
		eye_x += 0.3*sin(angle);
		eye_z -= 0.3*cos(angle);
    }
        
        
	}
    
    if (camerastate == 1){
        eye_y = 0;
        look_x = eye_x + 100*sin(angle);
        look_z = eye_z - 100*cos(angle);
    } else {
        eye_y = 1.6;
        }
	glutPostRedisplay();
}
//--------------------------------------------------------------------------------
void skybox(){
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);


  glBindTexture(GL_TEXTURE_2D, txId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1000,  -10, 1000);
  glTexCoord2f(1.0, 0.0); glVertex3f(-1000, -10., -1000);
  glTexCoord2f(1.0, 1.0); glVertex3f(-1000, 1000., -1000);
  glTexCoord2f(0.0, 1.0); glVertex3f(-1000, 1000, 1000);
  glEnd();


  glBindTexture(GL_TEXTURE_2D, txId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);glVertex3f(-1000,  -10, -1000);
  glTexCoord2f(1.0, 0.0);glVertex3f(1000, -10., -1000);
  glTexCoord2f(1.0, 1.0);glVertex3f(1000, 1000, -1000);
  glTexCoord2f(0.0, 1.0);glVertex3f(-1000,  1000, -1000);
  glEnd();


  glBindTexture(GL_TEXTURE_2D, txId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(1000,  -10, -1000);
  glTexCoord2f(1.0, 0.0); glVertex3f(1000, -10, 1000);
  glTexCoord2f(1.0, 1.0); glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0.0, 1.0); glVertex3f(1000,  1000,  -1000);
  glEnd();



  glBindTexture(GL_TEXTURE_2D, txId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f( 1000, -10, 1000);
  glTexCoord2f(1.0, 0.0); glVertex3f(-1000, -10,  1000);
  glTexCoord2f(1.0, 1.0); glVertex3f(-1000, 1000,  1000);
  glTexCoord2f(0.0, 1.0); glVertex3f( 1000, 1000, 1000);
  glEnd();
  

  glBindTexture(GL_TEXTURE_2D, txId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1000, 1000, -1000);
  glTexCoord2f(1.0, 0.0); glVertex3f(1000, 1000,  -1000);
  glTexCoord2f(1.0, 1.0); glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0.0, 1.0); glVertex3f(-1000, 1000, 1000);
  glEnd();
  
    glEnable(GL_LIGHTING);

  
}
//--------------------------------------------------------------------------------
void drawbubbles(){
        
        glDisable(GL_TEXTURE_2D);
        glColor3ub(20,255,255);
        glPushMatrix();
            glTranslatef(-6,-1,-9);
            
            for (int i = 0; i < bubbles; i ++){
                glPushMatrix();
                    glTranslatef((sin(bubbleup[i])/3)+bubblex[i],bubbleup[i],(sin(bubbleup[i])/4)+bubblez[i]);
                    glutSolidSphere(0.1,20,30);
                glPopMatrix();
            }
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
}
//--------------------------------------------------------------------------------
void spongebob(){
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();

        glTranslatef(10,0,-110);// center of circle
        glRotatef(spongestep,0,1,0);//move
        glTranslatef(35,0,0);//radius of circle

        
        glColor3ub(255, 197, 22);	//sponge
        glPushMatrix();
          glTranslatef(0, 5.5, 0);
          glScalef(3.5, 4, 1.4);
          glutSolidCube(1);
        glPopMatrix();
        
        glColor3ub(140, 70, 30);	
        glPushMatrix();
          glTranslatef(0, 3.7, 0);
          glScalef(3.6, 0.9, 1.5);
          glutSolidCube(1);
        glPopMatrix();
        
        glColor3ub(255, 255, 255);	
        glPushMatrix();
          glTranslatef(0, 4.3, 0);
          glScalef(3.55, 0.445, 1.45);
          glutSolidCube(1);
        glPopMatrix();

        glColor3ub(255, 198, 25);	//Right leg
        glPushMatrix();
          glTranslatef(0.8, 3.4, 0);
          glRotatef(spongetheta, -1, 0, 0);
          glTranslatef(-0.8, -3.4, 0);
          glTranslatef(-0.8, 2.1, 0);
          glScalef(0.5, 3, 0.5);
          glutSolidCube(1);
        glPopMatrix();
        
        glColor3ub(255, 198, 25);   //Left leg
        glPushMatrix();
          glTranslatef(0.8, 3.4, 0);
          glRotatef(spongetheta, 1, 0, 0);
          glTranslatef(-0.8, -3.4, 0);
          glTranslatef(0.8, 2.1, 0);
          glScalef(0.5, 3, 0.5);
          glutSolidCube(1);
        glPopMatrix();

        glColor3ub(255, 198, 25);	//Right arm
        glPushMatrix();
            glTranslatef(2, 5.5, 0);
          glRotatef(spongetheta, 1, 0, 0);
          glTranslatef(-2, -5.5, 0);
          glTranslatef(-2, 3.5, 0);
          glScalef(0.5, 3, 0.5);
          glutSolidCube(1);
        glPopMatrix();

        glColor3ub(255, 198, 25);	//Left arm
        glPushMatrix();
            glTranslatef(2, 5.5, 0);
          glRotatef(spongetheta, -1, 0, 0);
          glTranslatef(-2, -5.5, 0);
          glTranslatef(2, 3.5, 0);
          glScalef(0.5, 3, 0.5);
          glutSolidCube(1);
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}
//--------------------------------------------------------------------------------
void walls(){
    glBindTexture(GL_TEXTURE_2D, txId[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);	
 	glBegin(GL_QUADS);

	////////////////////// BACK WALL ///////////////////////

    glTexCoord2f(0.0, 3.0); glVertex3f(-15, 1, -15);
    glTexCoord2f(0.0, 0.0); glVertex3f(-15, -1, -15);
    glTexCoord2f(20.0, 0.0); glVertex3f(15, -1, -15);
    glTexCoord2f(20.0, 3.0); glVertex3f(15, 1, -15);

	////////////////////// FRONT WALL ///////////////////////
    
    glTexCoord2f(0.0, 2.0); glVertex3f(-15, 1, 15);
    glTexCoord2f(0.0, 0.0); glVertex3f(-15, -1, 15);
    glTexCoord2f(12.0, 0.0); glVertex3f(15, -1, 15);
    glTexCoord2f(12.0, 2.0); glVertex3f(15, 1, 15);
     
	////////////////////// LEFT WALL ///////////////////////

	 glTexCoord2f(0.0, 2.0); glVertex3f(-15, 1, -15);
  	 glTexCoord2f(0.0, 0.0); glVertex3f(-15, -1, -15);
	 glTexCoord2f(12.0, 0.0); glVertex3f(-15, -1, 15);
 	 glTexCoord2f(12.0, 2.0); glVertex3f(-15, 1, 15);


	////////////////////// RIGHT WALL ///////////////////////

     glTexCoord2f(0.0, 2.0); glVertex3f(15, 1, -15);
	 glTexCoord2f(0.0, 0.0); glVertex3f(15, -1, -15);
 	 glTexCoord2f(12.0, 0.0); glVertex3f(15, -1, 15);
 	 glTexCoord2f(12.0, 2.0); glVertex3f(15, 1, 15);

	glEnd();
}
//--------------------------------------------------------------------------------
void floor(){
    
      glBindTexture(GL_TEXTURE_2D, txId[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1000, -1.2, 1000);
  glTexCoord2f(10.0, 0.0); glVertex3f(1000, -1.2,  1000);
  glTexCoord2f(10.0, 10.0); glVertex3f(1000, -1.2, -1000);
  glTexCoord2f(0.0, 10.0); glVertex3f(-1000, -1.2, -1000);
  glEnd();




    glBindTexture(GL_TEXTURE_2D, txId[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glColor3f(0.6,0.2,0.3);
    glBegin(GL_QUADS);
    for(int flx = -350; flx < 350; flx++)
	{
		for(int flz = -350;  flz < 350; flz++)
		{
			glTexCoord2f(0.0, 1.0); glVertex3f(flx, -1, flz);
			glTexCoord2f(0.0, 0.0); glVertex3f(flx, -1, flz+1);
			glTexCoord2f(1.0, 0.0); glVertex3f(flx+1, -1, flz+1);
			glTexCoord2f(1.0, 1.0); glVertex3f(flx+1, -1, flz);
		}
	}
    glEnd();
    
    

}
//--------------------------------------------------------------------------------
void road(){
    glDisable(GL_TEXTURE_2D);
    glColor3ub(40,40,60);
    for(int i = 0; i < 6000; i ++){
        glPushMatrix();
            glScalef(0.5,0.02,2);
            glTranslatef(1500-(0.5*i),-50.5,-2);
            glutSolidCube(2);
        glPopMatrix();
    }
  glEnable(GL_TEXTURE_2D);
}
//--------------------------------------------------------------------------------
void squidwardsHouse(){
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
        glColor3ub(60,80,100);
        glTranslatef(2, -1, -25);
        glRotatef(-90,1,0,0);
        GLUquadric *t;
        t = gluNewQuadric();
        gluCylinder(t, 4, 3, 15, 30, 20);
        gluQuadricDrawStyle(t, GLU_FILL);
        
    glPopMatrix();
    
    glPushMatrix();// steps
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[6]);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        
        glTranslatef(2, -1, -20);
        glScalef(1,0.05,0.4);
        glColor3ub(120,55,10);
        glutSolidCube(2);
                  glBegin(GL_QUADS);
          glTexCoord2f(0.4, 0.0); glVertex3f(-1, 1.05, 1);
          glTexCoord2f(0.4, 1); glVertex3f(1, 1.05,  1);
          glTexCoord2f(0.0, 1); glVertex3f(1, 1.05, -1);
          glTexCoord2f(0.0, 0.0); glVertex3f(-1, 1.05, -1);
          glEnd();
        glTranslatef(0,0, 5);
        glutSolidCube(2);
                  glBegin(GL_QUADS);
          glTexCoord2f(0.4, 0.0); glVertex3f(-1, 1.05, 1);
          glTexCoord2f(0.4, 1); glVertex3f(1, 1.05,  1);
          glTexCoord2f(0.0, 1); glVertex3f(1, 1.05, -1);
          glTexCoord2f(0.0, 0.0); glVertex3f(-1, 1.05, -1);
          glEnd();
        glTranslatef(0,0, 5);
        glutSolidCube(2);
                  glBegin(GL_QUADS);
          glTexCoord2f(0.4, 0.0); glVertex3f(-1, 1.05, 1);
          glTexCoord2f(0.4, 1); glVertex3f(1, 1.05,  1);
          glTexCoord2f(0.0, 1); glVertex3f(1, 1.05, -1);
          glTexCoord2f(0.0, 0.0); glVertex3f(-1, 1.05, -1);
          glEnd();
        glTranslatef(0,0, 5);
        glutSolidCube(2);
                  glBegin(GL_QUADS);
          glTexCoord2f(0.4, 0.0); glVertex3f(-1, 1.05, 1);
          glTexCoord2f(0.4, 1); glVertex3f(1, 1.05,  1);
          glTexCoord2f(0.0, 1); glVertex3f(1, 1.05, -1);
          glTexCoord2f(0.0, 0.0); glVertex3f(-1, 1.05, -1);
          glEnd();
        glTranslatef(0,0, 5);
        glutSolidCube(2);
                  glBegin(GL_QUADS);
          glTexCoord2f(0.4, 0.0); glVertex3f(-1, 1.05, 1);
          glTexCoord2f(0.4, 1); glVertex3f(1, 1.05,  1);
          glTexCoord2f(0.0, 1); glVertex3f(1, 1.05, -1);
          glTexCoord2f(0.0, 0.0); glVertex3f(-1, 1.05, -1);
          glEnd();
        glTranslatef(0,0, 5);
        glutSolidCube(2);
                  glBegin(GL_QUADS);
          glTexCoord2f(0.4, 0.0); glVertex3f(-1, 1.05, 1);
          glTexCoord2f(0.4, 1); glVertex3f(1, 1.05,  1);
          glTexCoord2f(0.0, 1); glVertex3f(1, 1.05, -1);
          glTexCoord2f(0.0, 0.0); glVertex3f(-1, 1.05, -1);
          glEnd();
        glTranslatef(0,0, 5);
        glutSolidCube(2);
        

        
          glBegin(GL_QUADS);
          glTexCoord2f(0.4, 0.0); glVertex3f(-1, 1.05, 1);
          glTexCoord2f(0.4, 1); glVertex3f(1, 1.05,  1);
          glTexCoord2f(0.0, 1); glVertex3f(1, 1.05, -1);
          glTexCoord2f(0.0, 0.0); glVertex3f(-1, 1.05, -1);
          glEnd();

        
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
            
    glPopMatrix();
    
    glPushMatrix();//eyebrow
        glColor3ub(65,90,110);
        glTranslatef(2, 10, -22);
        glScalef(2.5,0.5,1);
        glutSolidCube(2);
    glPopMatrix();
        glPushMatrix();//house door
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBegin(GL_QUADS);
      glTexCoord2f(0.2, 0.0); glVertex3f(3.5,  -1, -21);
      glTexCoord2f(1.1, 0.0); glVertex3f(1, -1, -21);
      glTexCoord2f(1.1, 1.); glVertex3f(1, 2, -21);
      glTexCoord2f(0.2, 1.); glVertex3f(3.5, 2, -21);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    /*glPushMatrix();//house door
        glColor3ub(65,90,110);
        glTranslatef(2, 0.4, -22.5);
        glScalef(1,1.5,1.5);
        glutSolidCube(2);
    glPopMatrix();*/
    


    
    glPushMatrix();;//house ears
        glColor3ub(65,90,110);
        glTranslatef(-1.8, 6, -25);
        glScalef(1,2.5,0.5);
        glutSolidCube(2);
        glTranslatef(7.6, 0, 0);
        glutSolidCube(2);
    glPopMatrix();
    
    glPushMatrix();//house nose
        glColor3ub(65,90,110);
        glRotatef(0,0,0,0);
        glTranslatef(2, 8, -21.5);
        glRotatef(-5,1,0,0);
        glScalef(0.8,2.5,0.5);
        glutSolidCube(2);
    glPopMatrix();
    
    glPushMatrix();//top windows
        glColor3ub(100,143,216);
        glRotatef(0,0,0,0);
        glTranslatef(0.5, 8.5, -22);
        glRotatef(-20,0,1,0);
        glutSolidTorus(0.15,0.7,20,40);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(100,143,216);
        glRotatef(0,0,0,0);
        glTranslatef(3.5, 8.5, -22);
        glRotatef(20,0,1,0);
        glutSolidTorus(0.15,0.7,20,40);
    glPopMatrix();
} 
//--------------------------------------------------------------------------------
void patrickshouse(){
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
        glColor3ub(139,69,19);
        glTranslatef(-20,-1,-25);
        glutSolidSphere(4,30,20);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    
}
//--------------------------------------------------------------------------------
void treasurechest(){
    
    glEnable(GL_TEXTURE_2D);
     
    glBindTexture(GL_TEXTURE_2D, txId[6]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
    glPushMatrix();
        glTranslatef(-5.5,-0.85,-9);
        glScalef(1.5, 1, 1.1);
        glutSolidCube(1);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
    glColor3ub(205,183,0);
    glPushMatrix();
        glTranslatef(-5.5,-0.39,-9);
        glScalef(1.4, 0.1, 1);
        glutSolidCube(1);
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, txId[6]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
    glPushMatrix();
        glTranslatef(-5.5,-0.3,-9);
        glTranslatef(0,0,-0.5);
        glRotatef(chestopen,1,0,0);
        glTranslatef(0,0,0.5);
        glScalef(1.5, 0.1, 1.1);
        glutSolidCube(1);
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);
    
}
//--------------------------------------------------------------------------------
void garry (){
    printf("garry ded %f\n",getGaryDist());
    if(getGaryDist() < 20){
    printf("garry ded %f\n",getGaryDist());    
    }
    
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
     glScalef(0.2,0.2,0.2);
     
        
        glTranslatef((garryposx/20),-4,-10 - (garryposz/20));
        glRotatef(garryrot,0,1,0);
    
    glPushMatrix();
        glColor3ub(255,20,147);
        //glScalef(0.5,0.5,0.5);
        glutSolidTorus(0.5, 0.5,15, 20);
    glPopMatrix();
    glColor3ub(140,180,240);
    glPushMatrix();
        glTranslatef(0,-0.8,0);
        glScalef(6.5,1,1.6);
        glutSolidCube(0.5);
    glPopMatrix();
    
    
    glPushMatrix();
        glTranslatef(-1,1,-0.2);
        glRotatef(90,1,0,0);
        GLUquadric *q;
        q = gluNewQuadric();
        gluCylinder(q, 0.05, 0.05, 2, 8, 8);
        gluQuadricDrawStyle(q, GLU_FILL);
        //glutSolidCube(0.5);
    glPopMatrix();
    
    glColor3f(1,1,1);
    glPushMatrix();
        glTranslatef(-1,1,-0.2);
        glutSolidSphere(0.2, 15, 15);
    glPopMatrix();
    
    glPushMatrix();
    glColor3ub(140,180,240);
        glTranslatef(-1,1,0.2);
        glRotatef(90,1,0,0);
        GLUquadric *qc;
        qc = gluNewQuadric();
        gluCylinder(qc, 0.05, 0.05, 2, 8, 8);
        gluQuadricDrawStyle(q, GLU_FILL);
        //glutSolidCube(0.5);
    glPopMatrix();
    
    glColor3f(1,1,1);
    glPushMatrix();
        glTranslatef(-1,1,0.2);
        glutSolidSphere(0.2, 15, 15);
    glPopMatrix();

    
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}
//--------------------------------------------------------------------------------
void car(){
    
    
        float lgt_pos2[] = {-4,3,0, 1.0f};
        float lgt_pos2DIR[] = {-3,-1,0};

    
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
            
            glTranslatef(carposx,-0.5,carposz);
            glRotatef(carrot*(-57.24),0,1,0);
            
            glColor3ub(200,100,100);//body
            glPushMatrix();
                glScalef(3,0.8,2);
                glutSolidCube(1);
            glPopMatrix();
            
            glColor3ub(150,100,150);//cab
            glPushMatrix();
                glTranslatef(0.2,0.5,0);
                glScalef(1.5,1,1.8);
                glutSolidCube(1);
            glPopMatrix();
            
            glColor3ub(135,206,250);//windscreen
            glPushMatrix();
                glTranslatef(-0.55,0.65,0);
                glScalef(0.1,0.5,1.7);
                glutSolidCube(1);
            glPopMatrix();
            
            glColor3ub(0,0,0);//wheels
            glPushMatrix();
                
                glTranslatef(0.9,-0.25,0.95);
                glRotatef((carposx+carposz)*-15,0,0,1);
                glScalef(0.06,0.06,0.06);
                glutSolidTorus(3, 3,8, 8);
            glPopMatrix();
            
            glPushMatrix();
                
                glTranslatef(-0.9,-0.25,0.95);
                glRotatef((carposx+carposz)*-15,0,0,1);
                glScalef(0.06,0.06,0.06);
                glutSolidTorus(3, 3,8, 8);
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(0.9,-0.25,-0.95);
                glRotatef((carposx+carposz)*-15,0,0,1);
                glScalef(0.06,0.06,0.06);
                glutSolidTorus(3, 3,8, 8);
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(-0.9,-0.25,-0.95);
                glRotatef((carposx+carposz)*-15,0,0,1);
                glScalef(0.06,0.06,0.06);
                glutSolidTorus(3, 3,8, 8);
            glPopMatrix();
            glDisable(GL_LIGHTING);
            glColor3ub(250,250,220);//headlights
            glPushMatrix();
                glTranslatef(-1.5,0,0.7);
                glScalef(0.1,0.25,0.5);
                glutSolidCube(1);
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(-1.5,0,-0.7);
                glScalef(0.1,0.25,0.5);
                glutSolidCube(1);
            glPopMatrix();
            glEnable(GL_LIGHTING);

        glLightfv(GL_LIGHT1,GL_POSITION, lgt_pos2);   //light position
        glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION, lgt_pos2DIR);   //light position
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 35.0);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,2);
            
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
}

void car2(){
    
    
        float lgt_pos2[] = {-4,3,0, 1.0f};
        float lgt_pos2DIR[] = {-3,-1,0};

    
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
            
            glTranslatef(carposx2,-0.5,carposz2);
            glRotatef(carrot2*(-57.24),0,1,0);
            
            glColor3ub(200,100,100);//body
            glPushMatrix();
                glScalef(3,0.8,2);
                glutSolidCube(1);
            glPopMatrix();
            
            glColor3ub(150,100,150);//cab
            glPushMatrix();
                glTranslatef(0.2,0.5,0);
                glScalef(1.5,1,1.8);
                glutSolidCube(1);
            glPopMatrix();
            
            glColor3ub(135,206,250);//windscreen
            glPushMatrix();
                glTranslatef(-0.55,0.65,0);
                glScalef(0.1,0.5,1.7);
                glutSolidCube(1);
            glPopMatrix();
            
            glColor3ub(0,0,0);//wheels
            glPushMatrix();
                
                glTranslatef(0.9,-0.25,0.95);
                glRotatef((carposx2+carposz2)*-15,0,0,1);
                glScalef(0.06,0.06,0.06);
                glutSolidTorus(3, 3,8, 8);
            glPopMatrix();
            
            glPushMatrix();
                
                glTranslatef(-0.9,-0.25,0.95);
                glRotatef((carposx2+carposz2)*-15,0,0,1);
                glScalef(0.06,0.06,0.06);
                glutSolidTorus(3, 3,8, 8);
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(0.9,-0.25,-0.95);
                glRotatef((carposx2+carposz2)*-15,0,0,1);
                glScalef(0.06,0.06,0.06);
                glutSolidTorus(3, 3,8, 8);
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(-0.9,-0.25,-0.95);
                glRotatef((carposx2+carposz2)*-15,0,0,1);
                glScalef(0.06,0.06,0.06);
                glutSolidTorus(3, 3,8, 8);
            glPopMatrix();
            glDisable(GL_LIGHTING);
            glColor3ub(250,250,220);//headlights
            glPushMatrix();
                glTranslatef(-1.5,0,0.7);
                glScalef(0.1,0.25,0.5);
                glutSolidCube(1);
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(-1.5,0,-0.7);
                glScalef(0.1,0.25,0.5);
                glutSolidCube(1);
            glPopMatrix();
            glEnable(GL_LIGHTING);

        glLightfv(GL_LIGHT1,GL_POSITION, lgt_pos2);   //light position
        glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION, lgt_pos2DIR);   //light position
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 35.0);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,2);
            
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
}


void normal(float x1, float y1, float z1, 
            float x2, float y2, float z2,
		      float x3, float y3, float z3 )
{
	  float nx, ny, nz;
	  nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
	  ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
	  nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}
//--------------------------------------------------------------------------------

void spongeBobsHouse(){
	float wx[N], wy[N], wz[N]; 
	float angleStep = -0.174532922;  //Rotate 10 degrees
	glColor4f (1.0, 0.75, 0.5, 1.0); 
    glPushMatrix();
    
    glRotatef(180,1,0,0);
    glScalef(3,3,3);
    glTranslatef(8,-3,8);
    
    glDisable(GL_TEXTURE_2D);
    glColor3ub(20,150,20);
    
        glPushMatrix();
            glRotatef(45,1,0,0);
            glScalef(0.8,2,0.7);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glRotatef(-30,1,0,0);
            glScalef(0.30,2.5,0.5);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glRotatef(-50,0,0,1);
            glScalef(0.4,3,0.2);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glRotatef(26,1,0,1);
            glScalef(0.5,4,0.4);
            glutSolidCube(1);
        glPopMatrix();
        
        
    glEnable(GL_TEXTURE_2D);
        glColor4f (1.0, 0.75, 0.5, 1.0); 
	for (int j = 0;j< 36;j++){
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < N; i++)
		{
			wx[i] = vx[i] * cos(angleStep) +  vz[i] * sin(angleStep);
			wy[i] = vy[i];
			wz[i] = -vx[i] * sin(angleStep) + vz[i] * cos(angleStep);
			if(i > 0) normal(wx[i-1], wy[i-1], wz[i-1], 
							vx[i-1], vy[i-1], vz[i-1],
							vx[i], vy[i], vz[i]);
			glTexCoord2f((float)j/36.0,(float)i/N); glVertex3f(vx[i], vy[i], vz[i]);
			if(i > 0) normal(wx[i-1], wy[i-1], wz[i-1],
							vx[i], vy[i], vz[i],
							wx[i], wy[i], wz[i] );
			glTexCoord2f((float)(j+1)/36.0,(float)i/N);glVertex3f(wx[i], wy[i], wz[i]);
		}
        glEnd();		

		for (int i = 0; i<N; i++){
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
		}
	}
    glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}	
//--------------------------------------------------------------------------------
void streetlight(){
    glColor3ub(100,100,100);
    glPushMatrix();
    float lgt_pos2[] = {0,5,2, 1.0f};
    float lgt_pos2DIR[] = {0,-1,0};
    glDisable(GL_TEXTURE_2D);
    

    glTranslatef(0,0.5,-1);
    glScalef(1,1.3,1);
    glRotatef(180,0,1,0);
    
    glPushMatrix();//pole
        glScalef(0.1,5,0.1);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();//top
        glTranslatef(0,2.5,0.5);
        glScalef(0.5,0.2,1.5);
        glutSolidCube(1);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glColor3ub(250,250,220);
    glPushMatrix();//light
        glTranslatef(0,2.4,0.8);
        glScalef(0.4,0.2,0.8);
        glutSolidCube(1);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    
        glLightfv(GL_LIGHT0,GL_POSITION, lgt_pos2);  
        glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, lgt_pos2DIR);  
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 35.0);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT,10);
        
    glEnable(GL_TEXTURE_2D);
glPopMatrix();    
}
void initialise(){ 
    
	loadTexture();
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
	glClearColor (0.0, 0.0, 0.0, 0.0);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    //gluPerspective(70.0, 1.0, 100.0, 5000.0);   //Perspective projection
}
//--------------------------------------------------------------------------------
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45., 1.77, 1., 5000.);

	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();
	gluLookAt(eye_x, eye_y, eye_z,  look_x, 0, look_z,   0, 1, 0);	
    
        if (carspeed > 0.005){
            carspeed -= 0.005;
        }

    
	    if (camerastate == 2){
        //eye_z = -4;
        //eye_x = carposx + 7;
        eye_y = 1.6;
        angle = -1.6;
        
        look_x = carposx;
        look_z = carposz;
        eye_x = carposx + (cos(carrot))*15;
        eye_z = carposz + (sin(carrot))*15;    
        
        if (keyStates['w'] == true){
            if (carspeed < 3){
                carspeed += 0.02;
            }
        }else{
            if (carspeed > 0.005){
            carspeed -= 0.005;
        }
        }
        if (keyStates['s'] == true){
            if (carspeed > -1){
            if (carspeed > 0){
                carspeed -= 0.08;
            } else {
                carspeed -= 0.01;
            }
        }
        }else{
            if (carspeed < -0.005){
            carspeed += 0.005;
        }
        }
        if (keyStates['a'] == true){
                carRotobj = carrot - 0.05;
        }
        if (keyStates['d'] == true){
                carRotobj = carrot + 0.05;
        }        
        
        } else if (camerastate == 1){
            
        eye_y = 0;
            
        if(keyStates['a'] == true) {
            angle -= 0.05;  //Change direction
        
        }
        if(keyStates['d'] == true) {
                angle += 0.05;
            
        }
        if(keyStates['s'] == true)
        {  //Move backward
            eye_x -= 0.2*sin(angle);
            eye_z += 0.2*cos(angle);
            
        }
         if(keyStates['w'] == true)
        { //Move forward
            eye_x += 0.3*sin(angle);
            eye_z -= 0.3*cos(angle);
        }  
        look_x = eye_x + 100*sin(angle);
        look_z = eye_z - 100*cos(angle);
        }
        
        if (keyStates['e'] == true){
            if (epressed == 0) {
                
                    if (camerastate == 1 && getCarDist() < 10){
                        camerastate = 2;
                    } else if (camerastate == 2) {
                        camerastate = 1;
                        eye_x = carposx;
                        eye_z = carposz;
                    }
            
             
             epressed = 1;
            }
        }
        
        if (keyStates['e'] == false){
            if (epressed == 1) {
             epressed = 0;
            }
        }
        
        
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL); 


    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    glPushMatrix();
        floor();
    glPopMatrix();
    glPushMatrix();
        road();
    glPopMatrix();
    glPushMatrix();
        squidwardsHouse();
    glPopMatrix();
    
    glPushMatrix();
        skybox();
    glPopMatrix();
    glPushMatrix();
        glScalef(0.2,0.2,0.2);
        glTranslatef(0,-6,-10);
        spongebob();
    glPopMatrix();
    glPushMatrix();
        drawbubbles();
    glPopMatrix();
    glPushMatrix();
        patrickshouse();
    glPopMatrix();
    treasurechest();
    glPushMatrix();
        garry();
    glPopMatrix();
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 10);
    glPushMatrix();
        car();
    glPopMatrix();
    glPushMatrix();
        car2();
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    
    glPushMatrix();
        streetlight();
    glPopMatrix();
    glPushMatrix();
        spongeBobsHouse();
    glPopMatrix();//the scred line
    
	glutSwapBuffers();
}
//--------------------------------------------------------------------------------



void keyPressed (unsigned char key, int x, int y) {
keyStates[key] = true;
} 
void keyUp (unsigned char key, int x, int y) { 
keyStates[key] = false;
}  
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
    glutInitWindowSize (1066, 600); 
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("Spongebob");
    initialise();
    
    glutTimerFunc(50, timer, 0);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp); 
    glutDisplayFunc(display); 
    glutSpecialFunc(special);
    
    glutMainLoop();
   return 0;
}
