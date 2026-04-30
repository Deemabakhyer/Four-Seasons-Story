#include <GL/glut.h>
#include <cmath>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;
//camera var and cloud movement
float cameraX = 0.0f;
float zoom = 1.0f;
float cloudMove = 0.0f;
//falling leaves
const int FALLING_LEAVES = 10;
bool leavesFalling = false;
bool leafActive[FALLING_LEAVES];
float leafX[FALLING_LEAVES];
float leafY[FALLING_LEAVES];
float leafRot[FALLING_LEAVES];
//controls dialogue text
int dialogState = 0;
//for drawing text with bitmap font
void drawText(float x, float y, string text) {
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}
//a circle
void circle(float x, float y, float r) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float a = i * 3.14159f / 180;
        glVertex2f(x + cos(a) * r, y + sin(a) * r);
    }
    glEnd();
}
//oval shape
void oval(float x, float y, float rx, float ry) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float a = i * 3.14159f / 180;
        glVertex2f(x + cos(a) * rx, y + sin(a) * ry);
    }
    glEnd();
}
//draws a cloud
void drawOneCloud(float x, float y, float s) {
    oval(x, y, 0.16f * s, 0.06f * s);
    circle(x - 0.10f * s, y + 0.02f * s, 0.06f * s);
    circle(x, y + 0.06f * s, 0.08f * s);
    circle(x + 0.10f * s, y + 0.02f * s, 0.06f * s);
}
//this draws the sun and moving cloud
void drawSunAndClouds() {
    glColor3f(1.0f, 0.72f, 0.18f);
    circle(0.72f, 0.78f, 0.12f);

    glColor3f(1.0f, 0.85f, 0.35f);
    circle(0.72f, 0.78f, 0.08f);

    float c = cloudMove; //the movement offset

    glColor3f(0.95f, 0.82f, 0.70f);
    drawOneCloud(-0.55f + c, 0.75f, 1.0f);

    glColor3f(0.96f, 0.78f, 0.62f);
    drawOneCloud(0.12f + c, 0.62f, 1.1f);

    glColor3f(0.97f, 0.84f, 0.72f);
    drawOneCloud(-1.15f + c, 0.55f, 0.85f);

    glColor3f(0.94f, 0.76f, 0.60f);
    drawOneCloud(1.20f + c, 0.52f, 0.9f);
}
//this draws the house
void drawHouse() {
    glColor3f(0.65f, 0.36f, 0.16f);
    glBegin(GL_QUADS);
    glVertex2f(-0.18f, -0.20f);
    glVertex2f(0.18f, -0.20f);
    glVertex2f(0.18f, 0.08f);
    glVertex2f(-0.18f, 0.08f);
    glEnd();

    glColor3f(0.35f, 0.08f, 0.04f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.25f, 0.08f);
    glVertex2f(0.25f, 0.08f);
    glVertex2f(0.0f, 0.28f);
    glEnd();

    glColor3f(0.18f, 0.09f, 0.04f);
    glBegin(GL_QUADS);
    glVertex2f(-0.045f, -0.20f);
    glVertex2f(0.045f, -0.20f);
    glVertex2f(0.045f, -0.05f);
    glVertex2f(-0.045f, -0.05f);
    glEnd();

    glColor3f(0.95f, 0.75f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(-0.14f, -0.03f);
    glVertex2f(-0.08f, -0.03f);
    glVertex2f(-0.08f, 0.03f);
    glVertex2f(-0.14f, 0.03f);

    glVertex2f(0.08f, -0.03f);
    glVertex2f(0.14f, -0.03f);
    glVertex2f(0.14f, 0.03f);
    glVertex2f(0.08f, 0.03f);
    glEnd();
}
//draws the shadow under the trees
void drawTreeShadow(float x, float y, float s) {
    glColor3f(0.16f, 0.10f, 0.05f);
    oval(x, y - 0.58f * s, 0.28f * s, 0.055f * s);
}
//this draws the road with the perspective
void drawRoad() {
    glColor3f(0.22f, 0.22f, 0.24f);
    glBegin(GL_QUADS);
    glVertex2f(-0.45f, -1.0f);
    glVertex2f(0.45f, -1.0f);
    glVertex2f(0.16f, -0.20f);
    glVertex2f(-0.16f, -0.20f);
    glEnd();

    glColor3f(0.32f, 0.32f, 0.34f);
    for (float y = -0.9f; y < -0.25f; y += 0.12f) {
        glBegin(GL_LINES);
        glVertex2f(-0.22f, y);
        glVertex2f(0.22f, y + 0.03f);
        glEnd();
    }

    glColor3f(0.9f, 0.7f, 0.2f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, -0.95f);
    glVertex2f(0.0f, -0.23f);
    glEnd();
}
//draws a tree
void drawTree(float x, float y, float s) {
    drawTreeShadow(x, y, s);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(s, s, 1);

    glColor3f(0.32f, 0.16f, 0.06f);
    glBegin(GL_QUADS);
    glVertex2f(-0.07f, -0.55f);
    glVertex2f(0.07f, -0.55f);
    glVertex2f(0.05f, 0.28f);
    glVertex2f(-0.05f, 0.28f);
    glEnd();

    glColor3f(0.85f, 0.25f, 0.04f);
    circle(0.0f, 0.55f, 0.32f);

    glColor3f(0.95f, 0.55f, 0.06f);
    circle(-0.22f, 0.38f, 0.23f);

    glColor3f(0.58f, 0.13f, 0.03f);
    circle(0.22f, 0.38f, 0.23f);

    glColor3f(0.9f, 0.45f, 0.03f);
    circle(0.02f, 0.32f, 0.18f);

    glPopMatrix();
}
// the shadow for the girl
void drawGirlShadow() {
    glColor3f(0.16f, 0.10f, 0.05f);
    oval(-0.45f, -0.84f, 0.25f, 0.05f);
}

// draws the girl
void drawGirl() {
    drawGirlShadow();

    glPushMatrix();
    glTranslatef(-0.45f, -0.35f, 0);

    glColor3f(0.16f, 0.07f, 0.025f);
    oval(0.0f, 0.40f, 0.20f, 0.28f);
    oval(-0.12f, 0.36f, 0.075f, 0.22f);
    oval(0.12f, 0.36f, 0.075f, 0.22f);

    glColor3f(0.12f, 0.10f, 0.09f);
    glBegin(GL_QUADS);
    glVertex2f(-0.09f, -0.45f);
    glVertex2f(-0.03f, -0.45f);
    glVertex2f(-0.03f, -0.08f);
    glVertex2f(-0.09f, -0.08f);

    glVertex2f(0.03f, -0.45f);
    glVertex2f(0.09f, -0.45f);
    glVertex2f(0.09f, -0.08f);
    glVertex2f(0.03f, -0.08f);
    glEnd();

    glColor3f(0.05f, 0.03f, 0.02f);
    glBegin(GL_QUADS);
    glVertex2f(-0.11f, -0.48f);
    glVertex2f(-0.02f, -0.48f);
    glVertex2f(-0.02f, -0.43f);
    glVertex2f(-0.11f, -0.43f);

    glVertex2f(0.02f, -0.48f);
    glVertex2f(0.11f, -0.48f);
    glVertex2f(0.11f, -0.43f);
    glVertex2f(0.02f, -0.43f);
    glEnd();

    glColor3f(0.56f, 0.30f, 0.10f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.23f, -0.12f);
    glVertex2f(0.23f, -0.12f);
    glVertex2f(0.15f, 0.36f);
    glVertex2f(-0.15f, 0.36f);
    glEnd();

    glColor3f(0.50f, 0.25f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(-0.22f, 0.28f);
    glVertex2f(-0.31f, 0.03f);
    glVertex2f(-0.25f, 0.00f);
    glVertex2f(-0.15f, 0.25f);

    glVertex2f(0.22f, 0.28f);
    glVertex2f(0.31f, 0.03f);
    glVertex2f(0.25f, 0.00f);
    glVertex2f(0.15f, 0.25f);
    glEnd();

    glColor3f(0.95f, 0.72f, 0.55f);
    circle(-0.29f, 0.00f, 0.035f);
    circle(0.29f, 0.00f, 0.035f);

    glColor3f(0.1f, 0.05f, 0.02f);
    circle(0.0f, 0.22f, 0.015f);
    circle(0.0f, 0.08f, 0.015f);
    circle(0.0f, -0.06f, 0.015f);

    glColor3f(0.16f, 0.07f, 0.025f);
    circle(-0.08f, 0.64f, 0.10f);
    circle(0.08f, 0.64f, 0.10f);
    circle(0.0f, 0.70f, 0.09f);

    glColor3f(0.95f, 0.72f, 0.55f);
    circle(0.0f, 0.55f, 0.145f);

    glColor3f(1.0f, 0.45f, 0.45f);
    circle(-0.075f, 0.535f, 0.025f);
    circle(0.075f, 0.535f, 0.025f);

    glColor3f(1, 1, 1);
    oval(-0.055f, 0.585f, 0.035f, 0.022f);
    oval(0.055f, 0.585f, 0.035f, 0.022f);

    glColor3f(0.18f, 0.08f, 0.03f);
    circle(-0.055f, 0.585f, 0.014f);
    circle(0.055f, 0.585f, 0.014f);

    glColor3f(0, 0, 0);
    circle(-0.055f, 0.585f, 0.007f);
    circle(0.055f, 0.585f, 0.007f);

    glColor3f(0.25f, 0.08f, 0.04f);
    glBegin(GL_LINE_STRIP);
    for (int i = 200; i <= 340; i++) {
        float a = i * 3.14159f / 180;
        glVertex2f(cos(a) * 0.045f, 0.515f + sin(a) * 0.025f);
    }
    glEnd();

    glPopMatrix();
}
// draws a single leaf
void drawLeaf(float x, float y, float rot) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rot, 0, 0, 1);

    glColor3f(0.9f, 0.35f, 0.02f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.08f);
    glVertex2f(0.06f, 0.0f);
    glVertex2f(0.0f, -0.08f);
    glVertex2f(-0.06f, 0.0f);
    glEnd();

    glColor3f(0.3f, 0.12f, 0.02f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.08f);
    glVertex2f(0.0f, -0.08f);
    glEnd();

    glPopMatrix();
}
// make the leaves fall from the tree
void startFallingLeaves() {
    leavesFalling = true;

    for (int i = 0; i < FALLING_LEAVES; i++) {
        leafActive[i] = true;

        if (i < 5) {
            leafX[i] = 1.05f + (i * 0.07f);
            leafY[i] = 0.15f + (i % 3) * 0.08f;
        }
        else {
            leafX[i] = 0.55f + ((i - 5) * 0.07f);
            leafY[i] = 0.05f + (i % 3) * 0.08f;
        }

        leafRot[i] = i * 35.0f;
    }
}
// draws the falling leaves
void drawFallingLeaves() {
    if (!leavesFalling) return;

    for (int i = 0; i < FALLING_LEAVES; i++) {
        if (leafActive[i]) {
            drawLeaf(leafX[i], leafY[i], leafRot[i]);
        }
    }
}
// this just add the leaves on the ground
void drawGroundLeaves() {
    drawLeaf(-0.8f, -0.75f, 20);
    drawLeaf(-0.2f, -0.82f, 70);
    drawLeaf(0.45f, -0.72f, 120);
    drawLeaf(0.85f, -0.85f, 40);
    drawLeaf(1.1f, -0.70f, 90);
    drawLeaf(-1.2f, -0.87f, 10);
    drawLeaf(-1.45f, -0.70f, 55);
    drawLeaf(1.45f, -0.82f, 135);
    drawLeaf(-0.55f, -0.92f, 80);
    drawLeaf(0.15f, -0.67f, 30);
    drawLeaf(1.65f, -0.62f, 160);
    drawLeaf(-1.65f, -0.62f, 100);
    drawLeaf(-0.05f, -0.90f, 25);
    drawLeaf(0.65f, -0.93f, 75);
    drawLeaf(-1.0f, -0.58f, 130);
    drawLeaf(1.25f, -0.55f, 45);
}
// this is the speech bubble
void drawSpeechBubble() {
    glColor3f(0.98f, 0.90f, 0.72f);
    glBegin(GL_QUADS);
    glVertex2f(-0.25f, 0.47f);
    glVertex2f(0.95f, 0.47f);
    glVertex2f(0.95f, 0.78f);
    glVertex2f(-0.25f, 0.78f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.18f, 0.47f);
    glVertex2f(-0.08f, 0.47f);
    glVertex2f(-0.36f, 0.16f);   
    glEnd();

    glColor3f(0.25f, 0.12f, 0.04f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.25f, 0.47f);
    glVertex2f(0.95f, 0.47f);
    glVertex2f(0.95f, 0.78f);
    glVertex2f(-0.25f, 0.78f);
    glEnd();

    glColor3f(0.1f, 0.06f, 0.03f);

    if (dialogState == 0) {
        drawText(-0.18f, 0.66f, "hello");
        drawText(-0.18f, 0.56f, "its autumn!");
    }
    else if (dialogState == 1) {
        drawText(-0.18f, 0.66f, "its a bit cold");
        drawText(-0.18f, 0.56f, "and the air is crisp");
    }
    else {
        drawText(-0.18f, 0.66f, "the leaves are beautifully golden and crunchy");
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
    glScalef(zoom, zoom, 1);
    glTranslatef(cameraX, 0, 0);

    glColor3f(0.90f, 0.55f, 0.28f);
    glBegin(GL_QUADS);
    glVertex2f(-2, -1);
    glVertex2f(2, -1);
    glVertex2f(2, 1);
    glVertex2f(-2, 1);
    glEnd();

    drawSunAndClouds();

    glColor3f(0.55f, 0.28f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(-2.0f, -0.35f);
    glVertex2f(2.0f, -0.35f);
    glVertex2f(2.0f, 0.15f);
    glVertex2f(-2.0f, 0.15f);
    glEnd();

    drawHouse();

    glColor3f(0.45f, 0.22f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(-2, -1);
    glVertex2f(2, -1);
    glVertex2f(2, -0.35f);
    glVertex2f(-2, -0.35f);
    glEnd();

    drawRoad();

    drawTree(1.25f, -0.25f, 1.0f);
    drawTree(-1.25f, -0.35f, 0.8f);
    drawTree(0.70f, -0.28f, 0.65f);
    drawTree(-0.90f, -0.25f, 0.55f);
    drawTree(1.75f, -0.35f, 0.7f);
    drawTree(-1.65f, -0.30f, 0.65f);
    drawTree(0.38f, -0.32f, 0.48f);
    drawTree(-0.55f, -0.32f, 0.50f);

    drawGirl();

    drawFallingLeaves();
    drawGroundLeaves();
    drawSpeechBubble();
    glPopMatrix();

    glutSwapBuffers();
}
//timer for the leaves
void timer(int value) {
    cloudMove += 0.0015f;
    if (cloudMove > 0.35f)
        cloudMove = -0.35f;

    if (leavesFalling) {
        bool anyLeafStillFalling = false;

        for (int i = 0; i < FALLING_LEAVES; i++) {
            if (leafActive[i]) {
                leafY[i] -= 0.012f;
                leafX[i] += 0.004f * sin(leafRot[i] * 0.05f);
                leafRot[i] += 4.0f;

                if (leafY[i] < -0.75f) {
                    leafActive[i] = false;
                }
                else {
                    anyLeafStillFalling = true;
                }
            }
        }

        if (!anyLeafStillFalling) {
            leavesFalling = false;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}
// the mouse interactions for the speech and the leaves
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int width = glutGet(GLUT_WINDOW_WIDTH);
        int height = glutGet(GLUT_WINDOW_HEIGHT);

        float mx = (float)x / width * 2.0f - 1.0f;
        float my = 1.0f - (float)y / height * 2.0f;

        if (mx >= -0.75f && mx <= -0.15f && my >= -0.85f && my <= 0.35f) {
            dialogState = (dialogState + 1) % 3;
        }
        //the sound for the falling leaves
        if (mx > 0.20f && my > -0.50f) {
            startFallingLeaves();

            PlaySound(TEXT("C:\\Users\\dan\\Desktop\\autumn\\ElevenLabs_Crisp_dry_leaves_crunching_underfoot,_autumn_walk.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }

        glutPostRedisplay();
    }
}
//keyboard for zooming and moving the camera
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
    case 'A':
        cameraX += 0.05f;
        break;

    case 'd':
    case 'D':
        cameraX -= 0.05f;
        break;

    case 'w':
    case 'W':
        zoom += 0.05f;
        break;

    case 's':
    case 'S':
        zoom -= 0.05f;
        if (zoom < 0.5f) zoom = 0.5f;
        break;

    case 'r':
    case 'R':
        cameraX = 0.0f;
        zoom = 1.0f;
        leavesFalling = false;
        dialogState = 0;
        for (int i = 0; i < FALLING_LEAVES; i++)
            leafActive[i] = false;
        break;

    case 27:
        exit(0);
    }

    glutPostRedisplay();
}

void init() {
    glClearColor(0.90f, 0.55f, 0.28f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.777f, 1.777f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Autumn");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}