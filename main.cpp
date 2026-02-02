#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
using namespace std;

int winW = 500, winH = 300;

// Flag dimensions
int rectLeft = 50, rectRight = 450;
int rectBottom = 50, rectTop = 250;

int cx = 250, cy = 150;   // Circle center
int radius = 60;

// Colors
float green[3] = {0.0f, 0.6f, 0.0f};
float red[3]   = {1.0f, 0.0f, 0.0f};
float white[3] = {1.0f, 1.0f, 1.0f};

// ---------------- DRAWING ----------------

void drawRectangle()
{
    glColor3fv(green);
    glBegin(GL_POLYGON);
        glVertex2i(rectLeft, rectBottom);
        glVertex2i(rectRight, rectBottom);
        glVertex2i(rectRight, rectTop);
        glVertex2i(rectLeft, rectTop);
    glEnd();
}

void drawCircle(int xc, int yc, int r)
{
    glColor3fv(red);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 360; i++)
    {
        float theta = i * 3.1416 / 180;
        glVertex2f(xc + r * cos(theta),
                   yc + r * sin(theta));
    }
    glEnd();
}

// ---------------- FLOOD FILL ----------------

void getPixelColor(int x, int y, float color[3])
{
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

void setPixelColor(int x, int y, float color[3])
{
    glColor3fv(color);
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
    glFlush();
}

bool sameColor(float c1[3], float c2[3])
{
    return (fabs(c1[0]-c2[0]) < 0.01 &&
            fabs(c1[1]-c2[1]) < 0.01 &&
            fabs(c1[2]-c2[2]) < 0.01);
}

void floodFill(int x, int y, float oldColor[3], float newColor[3])
{
    float currColor[3];
    getPixelColor(x, y, currColor);

    if(sameColor(currColor, oldColor))
    {
        setPixelColor(x, y, newColor);

        floodFill(x+1, y, oldColor, newColor);
        floodFill(x-1, y, oldColor, newColor);
        floodFill(x, y+1, oldColor, newColor);
        floodFill(x, y-1, oldColor, newColor);
    }
}

// ---------------- CLICK CHECK ----------------

bool insideCircle(int x, int y)
{
    int dx = x - cx;
    int dy = y - cy;
    return (dx*dx + dy*dy <= radius*radius);
}

bool insideRectangle(int x, int y)
{
    return (x >= rectLeft && x <= rectRight &&
            y >= rectBottom && y <= rectTop);
}

// ---------------- MOUSE ----------------

void mouse(int button, int state, int x, int y)
{
    if(button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;

    y = winH - y;  // convert mouse Y

    float clickedColor[3];
    getPixelColor(x, y, clickedColor);

    if(insideCircle(x, y))
    {
        if(sameColor(clickedColor, red))
            floodFill(x, y, red, white);
        else
            floodFill(x, y, white, red);
    }
    else if(insideRectangle(x, y))
    {
        if(sameColor(clickedColor, green))
            floodFill(x, y, green, white);
        else
            floodFill(x, y, white, green);
    }
}

// ---------------- DISPLAY ----------------

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawRectangle();
    drawCircle(cx, cy, radius);
    glFlush();
}

// ---------------- INIT ----------------

void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
}

// ---------------- MAIN ----------------

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Bangladesh Flag - Flood Fill");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
