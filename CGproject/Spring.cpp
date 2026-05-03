#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>      // handle the window-managing operations
#else
#include <GL/glew.h>
#include <GL/freeglut.h>    // handle the window-managing operations
#endif
#include <cmath>
#include <stdlib.h>
#include <iostream>
// ================= VARIABLES =================

// Animation State Variables
using namespace std;
float flowerAngle = 0;
float girlX = 1.2;
float birdOffset = 0;
float grassOffset = 0;
// Dialogue & Timing System
int textStep = 0;
float textTimer = 0;
GLint win_width = 1920, win_hight = 1080;
bool soundPlayed = false;

//Texture Mapping Data
GLuint grassTexture;
char grassPath[] = "/Users/remasabokhalel/Desktop/level 9/Computer Graphics/TheـFourـSeasons/TheـFourـSeasons/grass.bmp";

// ================= TEXT =================
void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}


// ================= CIRCLE =================
void drawCircle(float cx, float cy, float r) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float theta = 2 * 3.1416 * i / 100;
        glVertex2f(cx + r*cos(theta), cy + r*sin(theta));
    }
    glEnd();
}



GLuint LoadTexture(const char* filename, int width, int height)
{
    GLuint texture;
    unsigned char* data;
    FILE* file;

    file = fopen(filename, "rb");

    if (file == NULL)
    {
        cout << "Unable to open the image file" << endl;
        exit(0);
    }
    fseek(file, 54, SEEK_SET); // skip BMP header
    data = (unsigned char*)malloc(width * height * 3);

    fread(data, width * height * 3, 1, file);
    fclose(file);

    // Convert BGR → RGB
    for (int i = 0; i < width * height; ++i)
    {
        int index = i * 3;
        unsigned char B = data[index];
        unsigned char R = data[index + 2];
        data[index] = R;
        data[index + 2] = B;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
    return texture;
}
// ================= SUN =================
void drawSun() {

    float cx = 0.7;
    float cy = 0.7;
    float r = 0.1;
    glColor3f(1,1,0);
    drawCircle(cx, cy, r);
    int rays = 12;
    for(int i = 0; i < rays; i++) {
        float angle = i * 2 * 3.1416 / rays;
        float x1 = cx + (r + 0.1) * cos(angle);
        float y1 = cy + (r + 0.1) * sin(angle);
        float x2 = cx + r * cos(angle - 0.2);
        float y2 = cy + r * sin(angle - 0.2);
        float x3 = cx + r * cos(angle + 0.2);
        float y3 = cy + r * sin(angle + 0.2);
        glBegin(GL_TRIANGLES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glVertex2f(x3, y3);
        glEnd();
    }
}

// ================= GIRL =================
void drawGirl() {
    glPushMatrix();
    glTranslatef(girlX, 0, 0);
    
    // ================= hair=================
    glColor3f(0,0,0);
    
    glBegin(GL_QUADS);
    glVertex2f(-0.10,0.22);
    glVertex2f(0.10,0.22);
    glVertex2f(0.10,0.32);
    glVertex2f(-0.10,0.32);
    glEnd();
    drawCircle(0,0.32,0.10);
    
    // ================= haed=================
    glColor3f(1,0.8,0.6);
    drawCircle(0,0.3,0.07);
    
    
    // ================= Neck=================
    glColor3f(1,0.8,0.6);
    glBegin(GL_QUADS);
    glVertex2f(-0.02,0.23);
    glVertex2f(0.02,0.23);
    glVertex2f(0.02,0.20);
    glVertex2f(-0.02,0.20);
    glEnd();
    
    // ================= eyes=================
    glColor3f(1,1,1);
    drawCircle(-0.02,0.309,0.015);
    drawCircle(0.02,0.309,0.015);
    
    glColor3f(0,0,0);
    drawCircle(-0.02,0.309,0.007);
    drawCircle(0.02,0.309,0.007);
    
    // ================= Blush =================
    glColor3f(1,0.75,0.8);
    drawCircle(-0.04,0.280,0.013);
    drawCircle(0.04,0.280,0.013);
    
    // ================= Mouth =================
    glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
    
    glVertex2f(-0.025, 0.265);
    glVertex2f(-0.01, 0.255);
    glVertex2f(0.01, 0.255);
    glVertex2f(0.025, 0.265);
    
    glEnd();
    
    // ================= body =================
    glColor3f(0.6,0.8,0.4);
    glBegin(GL_QUADS);
    glVertex2f(-0.12,0.2);
    glVertex2f(0.12,0.2);
    glVertex2f(0.12,-0.05);
    glVertex2f(-0.12,-0.05);
    glEnd();
    // ================= skirt =================
    glColor3f(1.0f, 0.9f, 0.4f);
    glBegin(GL_QUADS);
    glVertex2f(-0.12f, -0.05f);
    glVertex2f( 0.12f, -0.05f);
    glVertex2f( 0.12f, -0.35f);
    glVertex2f(-0.12f, -0.35f);
    glEnd();
    // ================= Legs =================
    glColor3f(1,0.8,0.6);
    glBegin(GL_QUADS);
    glVertex2f(-0.04,-0.35);
    glVertex2f(-0.01,-0.35);
    glVertex2f(-0.01,-0.55);
    glVertex2f(-0.04,-0.55);
    
    glVertex2f(0.01,-0.35);
    glVertex2f(0.04,-0.35);
    glVertex2f(0.04,-0.55);
    glVertex2f(0.01,-0.55);
    glEnd();
    // ================= Arms =================
    glColor3f(1,0.8,0.6);
    glBegin(GL_QUADS);
    glVertex2f(-0.13,0.15);
    glVertex2f(-0.11,0.15);
    glVertex2f(-0.18,0.02);
    glVertex2f(-0.20,0.02);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex2f(0.11,0.15);
    glVertex2f(0.13,0.15);
    glVertex2f(0.20,0.02);
    glVertex2f(0.18,0.02);
    glEnd();
    
    // ================= Speech bubble =================
  

    if (girlX <= 0.05) {
 
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(0.35, 0.5, 0.18);

        glBegin(GL_TRIANGLES);
            glVertex2f(0.18, 0.35);
            glVertex2f(0.30, 0.45);
            glVertex2f(0.20, 0.45);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f); // Set text color to black for
        if (textStep == 1) {
            drawText(0.24, 0.52, "Hello! I am Farah.");
            drawText(0.22, 0.48, "Spring is finally here!");
        }
        else if (textStep == 2) {
            drawText(0.22, 0.52, "Flowers are blooming,");
            drawText(0.22, 0.48, "and birds are singing!");
        }
        else if (textStep == 3) {
            drawText(0.21, 0.52, "I love the fresh air");
            drawText(0.23, 0.48, "of this beautiful season!");
        }
    }
}

// ================= FLOWER =================
void drawFlowerSimple(float x, float y){
    glColor3f(0,0.6,0);
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x, y+0.13);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(x, y+0.08);
        glVertex2f(x+0.03, y+0.1);
        glVertex2f(x, y+0.1);

        glVertex2f(x, y+0.08);
        glVertex2f(x-0.03, y+0.1);
        glVertex2f(x, y+0.1);
    glEnd();
    
    glColor3f(1,0.3,0.6);

    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.035, y+0.13);
        glVertex2f(x, y+0.18);
        glVertex2f(x+0.035, y+0.13);
    glEnd();
}

void drawRose(float x, float y){
    glColor3f(0,0.6,0);
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x, y+0.14);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y+0.08);
        glVertex2f(x+0.04, y+0.1);
        glVertex2f(x, y+0.1);

        glVertex2f(x, y+0.08);
        glVertex2f(x-0.04, y+0.1);
        glVertex2f(x, y+0.1);
    glEnd();

    glColor3f(0.8, 0, 0);
    drawCircle(x, y+0.16, 0.03);

    glColor3f(1, 0, 0);
    drawCircle(x, y+0.16, 0.02);

    glColor3f(1, 0.3, 0.3);
    drawCircle(x, y+0.16, 0.01);
}

// ================= TREE =================
void drawTree(float x, float y) {
    // 🌲 Trunk
    glColor3f(0.2f, 0.1f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(x - 0.04f, y);
        glVertex2f(x + 0.04f, y);
        glVertex2f(x + 0.04f, y + 0.5f);
        glVertex2f(x - 0.04f, y + 0.5f);
    glEnd();

    glColor3f(0.3f, 0.8f, 0.5f);
    drawCircle(x, y + 0.65f, 0.2f);

    glColor3f(0.2f, 0.7f, 0.4f);
    drawCircle(x - 0.12f, y + 0.55f, 0.18f);
    drawCircle(x + 0.12f, y + 0.55f, 0.18f);
    //Small flowers on the tree
    glColor3f(1.0f, 0.7f, 0.7f);
    drawCircle(x - 0.08f, y + 0.65f, 0.03f);
    drawCircle(x + 0.08f, y + 0.6f, 0.03f);
}
// ================= BIRD =================
void drawBird(float x,float y){
    glPushMatrix();
    glTranslatef(birdOffset, 0, 0); // movemint
    glColor3f(1,1,0);
    drawCircle(x,y,0.03);
    glColor3f(0,0,0);
    drawCircle(x+0.015,y+0.01,0.005);
    glColor3f(1,0.8,0);
    glBegin(GL_TRIANGLES);
        glVertex2f(x,y);
        glVertex2f(x-0.04,y+0.02);
        glVertex2f(x-0.02,y-0.02);
    glEnd();
    glPopMatrix();
}

// ================= LAKE =================
void drawLake(){
    glColor3f(0.2,0.6,1);
    drawCircle(-0.3,-0.7,0.2);
}

// ================= GRASS =================
void drawGrass(){
    glColor3f(0,0.8,0);
    for(float i=-1;i<1;i+=0.1){
        glBegin(GL_LINES);
            glVertex2f(i,-0.5);
            glVertex2f(i + 0.02*sin(grassOffset + i*10), -0.4); // shakes
        glEnd();
    }
}

// ================= DISPLAY =================
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    // ================= GROUND WITH TEXTURE =================
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex2f(-1, -1);
            glTexCoord2f(3.0, 0.0); glVertex2f( 1, -1);
            glTexCoord2f(3.0, 1.0); glVertex2f( 1,  0);
            glTexCoord2f(0.0, 1.0); glVertex2f(-1,  0);
        glEnd();

        glDisable(GL_TEXTURE_2D);

    drawSun();
    drawTree(-0.65f, -0.5f);
    drawTree(0.7f, -0.5f);
    drawBird(-0.2,0.7);
    drawBird(0.1,0.75);
    drawFlowerSimple(-0.5, -0.6);
    drawFlowerSimple(-0.2, -0.55);
    drawRose(0.2, -0.6);
    drawRose(0.5, -0.55);
    drawLake();
    drawGrass();
    drawGirl();

    glutSwapBuffers();
}

// ================= ANIMATION =================
void update(int v){
    flowerAngle += 2;
    //
        if (girlX > 0.0) {
            girlX -= 0.008;
        }
        else {
            textTimer += 0.016;
            if (textTimer < 2.0) {
                textStep = 1;//Display the first message for the first 2 seconds.
            }
            else if (textTimer < 4.0) {
                textStep = 2; // Transition to the second message between 2 and 4 seconds.
            }
            else {
                textStep = 3; // Final message displayed after 4 seconds have elapsed.
            }
        }
    
    //Bird Flight Animation (Horizontal Translation)
    birdOffset += 0.004;
    if(birdOffset > 1.5) birdOffset = -1.5;
    
    //Grass Waving Animation
    grassOffset += 0.05;

    if(!soundPlayed){
        system("afplay \"/Users/remasabokhalel/Desktop/level 9/Computer Graphics/TheـFourـSeasons/TheـFourـSeasons/spring.wav\" &");
        soundPlayed = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16,update,0);
}

// ================= STOP SOUND =================
void stopSound() {
    system("killall afplay");
}

// ================= INIT =================
void init(){
    glClearColor(0.7,0.9,1,1);
    gluOrtho2D(-1,1,-1,1);
    grassTexture = LoadTexture(grassPath, 1024, 1024);
}

// ================= MAIN =================
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Spring Scene");
    glutFullScreen();

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0,update,0);

    atexit(stopSound);
    glutMainLoop();
}
