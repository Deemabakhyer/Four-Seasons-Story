//  Headers   //

#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>      // handle the window-managing operations
#else
#include <GL/glew.h>
#include <GL/freeglut.h>    // handle the window-managing operations
#endif
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib") // Links the multimedia library
#pragma comment(linker, "/subsystem:\"console\" /entry:\"mainCRTStartup\"")

using namespace std;

// Global variables //

GLint win_width = 1920, win_hight = 1080;

GLdouble PI = 3.14159265359;

float transValueX;
float transValueY;
float transValueZ;
float zNear;
float zFar;

int textIndex = 0;
string messages[] = { "Hello! I'm Deema", "It's cold today...", "But i love the snow!", "Look at the snow man i made :>" };
int totalMessages = 4;

// Basic shapes //

void DrawTriangle()
{
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();
}

void DrawSquare()
{
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glEnd();
}

void DrawRectangular()
{
    glBegin(GL_POLYGON);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();
}

void DrawLine()
{
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glEnd();
}

void DrawStripedLine() {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, 0.7f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void MyPoint()
{
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
}

/*
 * Function that handles the drawing of a circle using the triangle fan
 * method. This will create a filled circle.
 *
 * Params:
 *    x (GLFloat) - the x position of the center point of the circle
 *    y (GLFloat) - the y position of the center point of the circle
 *    radius (GLFloat) - the radius that the painted circle will have
 *    function Source: https://gist.github.com/linusthe3rd/803118
 */
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount;i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

/*
 * Function that handles the drawing of a circle using the line loop
 * method. This will create a hollow circle.
 *
 * Params:
 *    x (GLFloat) - the x position of the center point of the circle
 *    y (GLFloat) - the y position of the center point of the circle
 *    radius (GLFloat) - the radius that the painted circle will have
 *    function Source: https://gist.github.com/linusthe3rd/803118
 */
void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int lineAmount = 100; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_LINE_LOOP);
    for (i = 0; i <= lineAmount;i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / lineAmount)),
            y + (radius * sin(i * twicePi / lineAmount))
        );
    }
    glEnd();
}

// Complex shapes //

void DrawSnow()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, -2.5f, 0.0f);
    glScalef(10.0f, 2.0f, 1.0f);
    DrawRectangular();
    glPopMatrix();
}

void DrawSnowMan()
{
    // Body
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.0f, -1.25f, 0.45f); // Bottom
    drawFilledCircle(0.0f, -0.6f, 0.35f);  // Middle
    drawFilledCircle(0.0f, -0.05f, 0.25f); // Head

    // Eyes
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(-0.08f, 0.0f, 0.03f); // Left Eye
    drawFilledCircle(0.08f, 0.0f, 0.03f);  // Right Eye

    // Nose
    glColor3f(1.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, -0.05f, 0.1f);
    glScalef(0.1f, 0.1f, 1.0f);
    DrawTriangle();
    glPopMatrix();

    // Hat
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 0.30f, 0.0f); 
    glScalef(0.35f, 0.35f, 1.0f);    
    DrawTriangle();
    glPopMatrix();

    // Arms
    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(0.3f, -0.6f, 0.0f); // Right Arm
    glScalef(0.6f, 0.6f, 1.0f);
    DrawLine();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3f, -0.6f, 0.0f); // Left Arm
    glScalef(-0.6f, 0.6f, 1.0f);
    DrawLine();
    glPopMatrix();
}

void DrawTree()
{
    // Stem
    glColor3f(0.35f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(2.5f, -1.4f, 0.0f);
    glScalef(0.7f, 1.4f, 1.0f);
    DrawRectangular();
    glPopMatrix();

    // Leaves
    glColor3f(0.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(2.5f, -0.5f, 0.0f);
    glScalef(2.5f, 2.0f, 1.0f); 
    DrawTriangle();
    glTranslatef(0.0f, 0.45f, 0.0f);
    glScalef(0.85f, 0.95f, 1.0f); 
    DrawTriangle();
    glTranslatef(0.0f, 0.45f, 0.0f);
    glScalef(0.85f, 0.95f, 1.0f);
    DrawTriangle();
    glPopMatrix();
}

void DrawDeemu()
{
    float girlX = -2.5f; // Moved further to the left (from -1.2f)
    float girlY = -1.25f;

    // --- Hair (Dark Brown Curly Strands - Type 3A) ---
    glColor3f(0.22f, 0.12f, 0.04f);
    for (float i = 0.0f; i < 0.55f; i += 0.08f) {
        drawFilledCircle(girlX - 0.22f, (girlY + 1.35f) - i, 0.07f);
        drawFilledCircle(girlX + 0.22f, (girlY + 1.35f) - i, 0.07f);
        drawFilledCircle(girlX - 0.27f, (girlY + 1.30f) - i, 0.06f);
        drawFilledCircle(girlX + 0.27f, (girlY + 1.30f) - i, 0.06f);
    }
    drawFilledCircle(girlX, girlY + 1.42f, 0.22f);

    // --- Face (Wheatish Complexion) ---
    glColor3f(0.96f, 0.80f, 0.69f);
    drawFilledCircle(girlX, girlY + 1.25f, 0.20f);

    // --- Blush (Light Pink Cheeks) ---
    glColor3f(0.98f, 0.65f, 0.65f);
    drawFilledCircle(girlX - 0.11f, girlY + 1.20f, 0.035f);
    drawFilledCircle(girlX + 0.11f, girlY + 1.20f, 0.035f);

    // --- Smile (Half Hollow Circle) ---
    glColor3f(0.3f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    float smileRadius = 0.06f;
    float centerX = girlX;
    float centerY = girlY + 1.22f;
    for (int i = 180; i <= 360; i++) {
        float theta = i * PI / 180.0f;
        glVertex2f(centerX + smileRadius * cos(theta), centerY + (smileRadius * 0.7f) * sin(theta));
    }
    glEnd();

    // --- Legs (Drawn BEFORE coat so they sit behind it) ---
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(girlX - 0.12f, girlY + 0.45f, 0.0f); // Adjusted Y to hide top under coat
    glScalef(0.15f, -1.41f, 1.0f);
    DrawRectangular();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(girlX + 0.12f, girlY + 0.45f, 0.0f); // Adjusted Y to hide top under coat
    glScalef(0.15f, -1.41f, 1.0f);
    DrawRectangular();
    glPopMatrix();

    // --- Winter Clothes (Deep Blue Coat - Drawn AFTER legs) ---
    glColor3f(0.2f, 0.3f, 0.5f);
    glPushMatrix();
    glTranslatef(girlX, girlY + 0.75f, 0.0f);
    glScalef(0.6f, 0.8f, 1.0f);
    DrawRectangular();
    glPopMatrix();

    // --- Scarf (Red) ---
    glColor3f(0.8f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(girlX, girlY + 1.05f, 0.0f);
    glScalef(0.5f, 0.15f, 1.0f);
    DrawSquare();
    glPopMatrix();

    // --- Arms ---
    glColor3f(0.2f, 0.3f, 0.5f);
    glPushMatrix();
    glTranslatef(girlX + 0.3f, girlY + 0.8f, 0.0f);
    glRotatef(-45, 0, 0, 1);
    glScalef(0.15f, 0.5f, 1.0f);
    DrawRectangular();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(girlX - 0.3f, girlY + 0.8f, 0.0f);
    glRotatef(45, 0, 0, 1);
    glScalef(0.15f, 0.5f, 1.0f);
    DrawRectangular();
    glPopMatrix();

    // --- Eyes ---
    glColor3f(0.1f, 0.1f, 0.1f);
    drawFilledCircle(girlX - 0.07f, girlY + 1.28f, 0.025f);
    drawFilledCircle(girlX + 0.07f, girlY + 1.28f, 0.025f);
}

void DrawTextBox()
{
    float boxX = -2.0f;
    float boxY = 0.5f;

    // --- 1. Speech Bubble (Oval Shape) ---
    glColor3f(0.9f, 0.9f, 0.9f);
    glPushMatrix();
    glTranslatef(boxX + 0.6f, boxY + 0.3f, 0.0f); // Position of the bubble
    glScalef(1.8f, 0.8f, 1.0f);                  // Stretch X by 1.8, Squash Y by 0.8
    drawFilledCircle(0.0f, 0.0f, 0.5f);          // Draw circle at local origin
    glPopMatrix();

    // --- 2. Pointer Triangle (The "Tail") ---
    glPushMatrix();
    // Adjusted position slightly to meet the new oval edge
    glTranslatef(boxX + 0.15f, boxY - 0.05f, 0.0f);
    glRotatef(130, 0, 0, 1);
    glScalef(0.3f, 0.5f, 1.0f);
    DrawTriangle();
    glPopMatrix();

    // --- 3. Text Rendering (Simple and Centered) ---
    glColor3f(0.0f, 0.0f, 0.0f);

    // Adjust these offsets to center the text perfectly in the new oval
    float textOffsetX = -0.75f;
    float textOffsetY = 0.25f;
    glRasterPos2f(boxX + 0.6f + textOffsetX, boxY + textOffsetY);

    string currentText = messages[textIndex];
    for (int i = 0; i < currentText.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, currentText[i]);
    }
}

void DrawSnowFall()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(-4.0f, 2.0f, 0.03f);
    drawFilledCircle(-2.0f, 2.5f, 0.03f);
    drawFilledCircle(0.0f, 2.2f, 0.03f);
    drawFilledCircle(2.0f, 2.7f, 0.03f);
    drawFilledCircle(4.0f, 2.1f, 0.03f);
    drawFilledCircle(-3.0f, 1.5f, 0.03f);
    drawFilledCircle(1.5f, 1.2f, 0.03f);
    drawFilledCircle(3.5f, 1.8f, 0.03f);
}

//  Functions //

static void special(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:; 
        break;
    case GLUT_KEY_RIGHT:; 
        break;
    case GLUT_KEY_DOWN: transValueY -= .1; 
        break;
    }
    glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Cycle to the next message
        textIndex = (textIndex + 1) % totalMessages;

        // Refresh the display to show new text
        glutPostRedisplay();
    }
}

static void play(void) 
{ 
    glutPostRedisplay();
}

static void key(unsigned char keyPressed, int x, int y)
{
    switch (keyPressed) {
    case 'q': 
    case 27: 
        exit(0); 
        break;
    }
}

void init() {
    transValueX = 0.0;
    transValueY = 0.0;
    transValueZ = 6.0; 
    zNear = 0.1;
    zFar = 100.0;

    // Play background music
    // SND_FILENAME: path to your file
    // SND_ASYNC: plays in background so your graphics don't freeze
    // SND_LOOP: repeats the music infinitely
    PlaySound(TEXT("background_music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    glClearColor(0.01f, 0.01f, 0.15f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (1920.0 / 1080.0), zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
}

void reshapeFunc(GLint new_width, GLint new_hight)
{
    if (new_hight == 0) new_hight = 1;
    glViewport(0, 0, new_width, new_hight);
    win_width = new_width;
    win_hight = new_hight;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(transValueX, 0, transValueZ, 0, 0, 0, 0, 1, 0);

    DrawSnow();
    DrawTree();
    DrawSnowMan();
	DrawTextBox();
	DrawDeemu();
    
    /*
    The fmod Function(C++ Standard Library)
    The fmod function is part of the <cmath>(or <math.h>) library in C++.
    It computes the floating - point remainder of the division operation
    Reference: https://en.cppreference.com/w/cpp/numeric/math/fmod
    */
    float infiniteSnowY = fmod(transValueY, 4.0f);

    glPushMatrix();
    glTranslatef(0.0f, infiniteSnowY, 0.0f);
    DrawSnowFall();
    glTranslatef(0.0f, 4.0f, 0.0f);
    DrawSnowFall();
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Winter Scene");
    glutFullScreen();

    init();

    glutIdleFunc(play);
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeFunc);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(special);

    glutMainLoop();
    return 0;
}