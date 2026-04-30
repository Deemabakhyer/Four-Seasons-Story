#include <windows.h>
#pragma comment(lib, "winmm.lib") 
#include <GL/freeglut.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

// Window dimensions
GLint win_width = 1920;
GLint win_height = 1080;

// Animation Variables
float boatX = 0.0f;
int textState = 0;

// Helper function to draw a circle 
void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// Helper function (text)
void renderText(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(font, *string++);
    }
}

// 1. DRAWING: BACKGROUND
void drawBackground() {
    // Sky
    glColor3f(0.6f, 0.85f, 0.95f);
    glRectf(0.0f, 350.0f, 800.0f, 600.0f);

    // Sand
    glColor3f(0.9f, 0.8f, 0.5f);
    glRectf(0.0f, 0.0f, 800.0f, 200.0f);
}

// 2. DRAWING: SUN
void drawSun() {
    glPushMatrix();

    // Sun Circle
    glColor3f(1.0f, 0.9f, 0.0f);
    drawCircle(400.0f, 350.0f, 85.0f, 40);

    // Sun Rays 
    glLineWidth(4.0f);
    glBegin(GL_LINES);

    // Top
    glVertex2f(400.0f, 445.0f); glVertex2f(400.0f, 485.0f);

    // Left and Right
    glVertex2f(305.0f, 350.0f); glVertex2f(265.0f, 350.0f);
    glVertex2f(495.0f, 350.0f); glVertex2f(535.0f, 350.0f);

    // Top Diagonals
    glVertex2f(335.0f, 415.0f); glVertex2f(305.0f, 445.0f); // Top-Left
    glVertex2f(465.0f, 415.0f); glVertex2f(495.0f, 445.0f); // Top-Right

    glEnd();

    glPopMatrix();
}

// 3. DRAWING: BOAT
void drawBoat() {
    glPushMatrix(); // Isolate boat's translation

    // Apply Translation
    glTranslatef(boatX, 0.0f, 0.0f);

    // Base of the boat
    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(380.0f, 260.0f);
    glVertex2f(460.0f, 260.0f);
    glVertex2f(480.0f, 290.0f);
    glVertex2f(360.0f, 290.0f);
    glEnd();

    // White Sail
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(430.0f, 290.0f);
    glVertex2f(470.0f, 290.0f);
    glVertex2f(430.0f, 340.0f);
    glEnd();

    glPopMatrix();
}

// 4. DRAWING: TREE & UMBRELLA
void drawTreeAndUmbrella() {
    // PALM TREE
    // Trunk
    glColor3f(0.5f, 0.3f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(130.0f, 50.0f);
    glVertex2f(170.0f, 50.0f);
    glVertex2f(160.0f, 250.0f);
    glVertex2f(140.0f, 250.0f);
    glEnd();

    // Coconuts
    glColor3f(0.3f, 0.15f, 0.0f);
    drawCircle(140.0f, 235.0f, 13.0f, 15);
    drawCircle(160.0f, 235.0f, 13.0f, 15);
    drawCircle(150.0f, 220.0f, 13.0f, 15);

    // Leaves
    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(150.0f, 240.0f); glVertex2f(50.0f, 220.0f);  glVertex2f(100.0f, 280.0f);
    glVertex2f(150.0f, 240.0f); glVertex2f(250.0f, 220.0f); glVertex2f(200.0f, 280.0f);
    glVertex2f(150.0f, 240.0f); glVertex2f(120.0f, 320.0f); glVertex2f(180.0f, 320.0f);
    glEnd();

    // UMBRELLA
    // Pole
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(355.0f, 60.0f);
    glVertex2f(365.0f, 60.0f);
    glVertex2f(385.0f, 130.0f);
    glVertex2f(375.0f, 130.0f);
    glEnd();

    // Stripes
    glColor3f(0.9f, 0.1f, 0.1f); // Red
    glBegin(GL_TRIANGLES);
    glVertex2f(400.0f, 200.0f); glVertex2f(300.0f, 140.0f); glVertex2f(360.0f, 125.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glBegin(GL_TRIANGLES);
    glVertex2f(400.0f, 200.0f); glVertex2f(360.0f, 125.0f); glVertex2f(420.0f, 110.0f);
    glEnd();
    glColor3f(0.9f, 0.1f, 0.1f); // Red
    glBegin(GL_TRIANGLES);
    glVertex2f(400.0f, 200.0f); glVertex2f(420.0f, 110.0f); glVertex2f(480.0f, 95.0f);
    glEnd();

    // TOWEL
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(200.0f, 30.0f); glVertex2f(240.0f, 30.0f); glVertex2f(280.0f, 90.0f); glVertex2f(240.0f, 90.0f);
    glEnd();
    glColor3f(0.8f, 0.7f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(240.0f, 30.0f); glVertex2f(280.0f, 30.0f); glVertex2f(320.0f, 90.0f); glVertex2f(280.0f, 90.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(280.0f, 30.0f); glVertex2f(320.0f, 30.0f); glVertex2f(360.0f, 90.0f); glVertex2f(320.0f, 90.0f);
    glEnd();
    glColor3f(0.8f, 0.7f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(320.0f, 30.0f); glVertex2f(360.0f, 30.0f); glVertex2f(400.0f, 90.0f); glVertex2f(360.0f, 90.0f);
    glEnd();
}

// 5. BOY & SPEECH BUBBLE
void drawBoyAndSpeech() {
    // Legs & Shoes
    glColor3f(0.9f, 0.8f, 0.7f);
    glRectf(610.0f, 20.0f, 620.0f, 40.0f);
    glRectf(640.0f, 20.0f, 650.0f, 40.0f);

    glColor3f(0.1f, 0.1f, 0.1f);
    glRectf(605.0f, 10.0f, 625.0f, 20.0f);
    glRectf(635.0f, 10.0f, 655.0f, 20.0f);

    // Shorts & Shirt
    glColor3f(0.3f, 0.2f, 0.6f);
    glRectf(600.0f, 40.0f, 660.0f, 80.0f);

    glColor3f(1.0f, 0.8f, 0.0f);
    glRectf(600.0f, 80.0f, 660.0f, 140.0f);

    // Arms
    glColor3f(0.9f, 0.8f, 0.7f);
    glRectf(580.0f, 90.0f, 600.0f, 130.0f);
    glRectf(660.0f, 90.0f, 675.0f, 130.0f);

    // Juice Cup & Straw
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(570.0f, 80.0f);
    glVertex2f(590.0f, 80.0f);
    glVertex2f(595.0f, 110.0f);
    glVertex2f(565.0f, 110.0f);
    glEnd();

    glColor3f(0.2f, 0.8f, 0.2f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex2f(575.0f, 110.0f);
    glVertex2f(565.0f, 130.0f);
    glEnd();

    // Hair & Head
    glColor3f(0.4f, 0.2f, 0.1f);
    drawCircle(630.0f, 165.0f, 28.0f, 20);

    glColor3f(0.9f, 0.8f, 0.7f);
    drawCircle(630.0f, 155.0f, 25.0f, 20);

    // Sunglasses
    glColor3f(0.1f, 0.1f, 0.1f);
    glRectf(612.0f, 152.0f, 628.0f, 168.0f);
    glRectf(632.0f, 152.0f, 648.0f, 168.0f);

    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(628.0f, 160.0f); glVertex2f(632.0f, 160.0f); // Bridge
    glVertex2f(612.0f, 160.0f); glVertex2f(605.0f, 160.0f); // Left arm
    glVertex2f(648.0f, 160.0f); glVertex2f(655.0f, 160.0f); // Right arm
    glEnd();

    // Smile
    glColor3f(0.8f, 0.1f, 0.1f);
    drawCircle(630.0f, 142.0f, 7.0f, 15);
    glColor3f(0.9f, 0.8f, 0.7f);
    glRectf(620.0f, 142.0f, 640.0f, 150.0f);

    // SPEECH BUBBLE
    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(490.0f, 220.0f, 730.0f, 270.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(610.0f, 220.0f);
    glVertex2f(630.0f, 195.0f);
    glVertex2f(640.0f, 220.0f);
    glEnd();

    // Display Text based on Spacebar state
    glColor3f(0.0f, 0.0f, 0.0f);
    if (textState == 0) {
        renderText(510.0f, 240.0f, GLUT_BITMAP_HELVETICA_18, "Hello!");
    }
    else if (textState == 1) {
        renderText(510.0f, 240.0f, GLUT_BITMAP_HELVETICA_18, "Phew, it is so hot today!");
    }
    else if (textState == 2) {
        renderText(510.0f, 240.0f, GLUT_BITMAP_HELVETICA_18, "But I love my cold orange juice.");
    }
    else if (textState == 3) {
        renderText(510.0f, 240.0f, GLUT_BITMAP_HELVETICA_18, "What season is it? Summer!");
    }
}

// DISPLAY FUNCTION 
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // 1. Background
    drawBackground();

    // 2. Sun
    drawSun();

    // 3. Ocean 
    glColor3f(0.3f, 0.6f, 0.9f);
    glRectf(0.0f, 200.0f, 800.0f, 350.0f);

    // 4.scene objects
    drawBoat();
    drawTreeAndUmbrella();
    drawBoyAndSpeech();

    glFlush();
    glutSwapBuffers();
}


// normal keys (Esc, Spacebar)
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: 
        exit(0);
        break;

    case ' ': // the Spacebar
        textState++;
        if (textState > 3) {
            textState = 0;
        }
        glutPostRedisplay();
        break;
    }
}

// special keys for boat movement
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_RIGHT:
        boatX += 3.0f; // Move right by 3 units
        if (boatX > 500.0f) boatX = -550.0f; // Loop back
        glutPostRedisplay();
        break;
    }
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(win_width, win_height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Summer Scene - Retaj Osama Mujahed");

    glutFullScreen();

    init();

    // Register all our functions with OpenGL
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys); 

    // sound
    PlaySound(TEXT("waves.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

    glutMainLoop();
    return 0;
}