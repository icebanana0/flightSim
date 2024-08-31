#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
// Camera position and orientation
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 10.0f;
float cameraYaw = 0.0f, cameraPitch = 0.0f;

// Movement speed and key states
float speed = 0.1f;
bool keys[256] = {false}; // Array to track key states

void drawTerrain() {
    glColor3f(0.2f, 0.8f, 0.2f);

    // Draw terrain as a grid of quads
    for (float x = -50.0f; x < 50.0f; x += 1.0f) {
        glBegin(GL_TRIANGLE_STRIP);
        for (float z = -50.0f; z < 50.0f; z += 1.0f) {
            float height1 = sin(x * 0.1f) * cos(z * 0.1f) * 5.0f;
            float height2 = sin((x + 1.0f) * 0.1f) * cos(z * 0.1f) * 5.0f;
            float height3 = sin(x * 0.1f) * cos((z + 1.0f) * 0.1f) * 5.0f;
            float height4 = sin((x + 1.0f) * 0.1f) * cos((z + 1.0f) * 0.1f) * 5.0f;

            glVertex3f(x, height1 - 1.0f, z);
            glVertex3f(x + 1.0f, height2 - 1.0f, z);
            glVertex3f(x, height3 - 1.0f, z + 1.0f);
            glVertex3f(x + 1.0f, height4 - 1.0f, z + 1.0f);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply camera transformation
    glRotatef(cameraPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraYaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-cameraX, -cameraY, -cameraZ);

    // Draw terrain
    drawTerrain();

    // Draw reference cubes
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f); // Center cube
    glutWireCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5.0f, -1.0f, 0.0f); // Right cube
    glutWireCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5.0f, -1.0f, 0.0f); // Left cube
    glutWireCube(1.0f);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    keys[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void updateCamera() {
    float dx = sin(cameraYaw * M_PI / 180.0) * speed;
    float dz = -cos(cameraYaw * M_PI / 180.0) * speed;
    
    if (keys['w']) {
        cameraX += dx;
        cameraZ += dz;
    }
    if (keys['s']) {
        cameraX -= dx;
        cameraZ -= dz;
    }
    if (keys['a']) {
        cameraYaw -= 2.0f;
    }
    if (keys['d']) {
        cameraYaw += 2.0f;
    }
    if (keys['q']) {
        cameraPitch += 2.0f;
    }
    if (keys['e']) {
        cameraPitch -= 2.0f;
    }

    // Clamp pitch to prevent flipping
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;
}

void idle() {
    updateCamera();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Flight Simulator");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
