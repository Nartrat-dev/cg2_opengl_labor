#include <iostream>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <linmath.h>

#define PI 3.1415926535897932384626433832795

/* -- 2.6 Translation mit Tastatur-- */
float tx{ 0.0F };
float ty{ 0.0F };
float tz{ 0.0F };

/* -- 2.7 Rotation mit Tastatur-- */
float rx{ 0.0F };
float ry{ 0.0F };
float rz{ 0.0F };


/* -- 2.1 Stoppuhr -- */
void drawClock(float &rotationAngle) {

    //gr. Zeiger
    glLoadIdentity();
    glRotatef(rotationAngle, 0, 0, -1);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glColor3f(1.0f, .5f, .5f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.8f, 0.0f);
    glEnd();

    //kleiner Zeiger
    glLoadIdentity();
    glRotatef(rotationAngle / 60, 0, 0, -1);
    glBegin(GL_LINES);
    glColor3f(0.5f, .5f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.6f, 0.0f);
    glEnd();
}

/* -- 2.2 Rotierenders Koordinatensystem -- */ 
void drawCoordinateSystem() {

    // Starte Zeichenprozess
    glBegin(GL_LINES);      

    // X-Achse (rot)
    glColor3f(255.0F, 0.0F, 0.0F);
    glVertex3f(0.0F, 0.0F, 0.0F);
    glVertex3f(1.0F, 0.0F, 0.0F);

    // Y-Achse (grün)
    glColor3f(0.0F, 255.0F, 0.0F);
    glVertex3f(0.0F, 0.0F, 0.0F);
    glVertex3f(0.0F, 1.0F, 0.0F);

    // Z-Achse (blau)
    glColor3f(0.0F, 0.0F, 255.0F);
    glVertex3f(0.0F, 0.0F, 0.0F);
    glVertex3f(0.0F, 0.0F, 1.0F);

    // beende Zeichenprozess
    glEnd();                
}

/* -- 2.3 Quadratisches Gitter -- */
void drawGrid(float size, int numLines) {

    // Ermittle Abstand, der zwischen den Linien sein muss, damit das Grid gezeichnet werden kann
    float distance_between_lines = size / static_cast<float>(numLines-1);

    // Wir fangen oben Links an zu zeichnen, daher start = current
    float offset = size / 2;
    float start_position = offset;
    float cur_position = offset;

    glBegin(GL_LINES);
    glColor3f(255.f, 255.f, 255.f);

    int i = 0;
    while (i < numLines) {

        glVertex3f(cur_position, start_position, 0.f);
        glVertex3f(cur_position, start_position - size, 0.f);

        glVertex3f(start_position, cur_position, 0.f);
        glVertex3f(start_position - size, cur_position, 0.f);

        // Gehe zur nächstesn Position, wo eine Linie gezeichnet werden soll
        cur_position -= distance_between_lines;

        i++;
    }
    glEnd();
}

/* -- 2.4 Kugel -- */
void drawSphere(float radius, int numLines) {

    // Dynamisches 3D-Array anlegen
    float*** pointsSphere = new float** [numLines];
    for (int i = 0; i < numLines; i++)
    {
        pointsSphere[i] = new float* [numLines];
    }
    for (int i = 0; i < numLines; i++)
        for (int j = 0; j < numLines; j++)
        {
            pointsSphere[i][j] = new float[3]; // Jede Zelle [i][j] speichert einen Punkt, bestehed aus drei Zahlen
        }

    // Array mit Punkten für Kugel füllen -> i: Azimut, j: Elevation, 0,1,2: Koordinate
    for (int i = 0; i < numLines; i++) {
        float phi = 2.0F * PI * i / numLines;

        for (int j = 0; j < numLines; j++) {
            float theta = PI * j / (numLines - 1);

            pointsSphere[i][j][0] = radius * sin(theta) * cos(phi); // x
            pointsSphere[i][j][1] = radius * sin(theta) * sin(phi); // y
            pointsSphere[i][j][2] = radius * cos(theta);            // z
        }
    }

    // Punkte zeichnen
    glBegin(GL_POINTS);
    glPointSize(5.0f);
    glColor3f(255.f, 255.f, 255.f);

    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < numLines; j++) {
            glVertex3f(pointsSphere[i][j][0], pointsSphere[i][j][1], pointsSphere[i][j][2]);
        }
    }

    glEnd();

    // Azimut / Breitengrade zeichnen
    for (int j = 0; j < numLines; j++) {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numLines; i++)
            glVertex3f(pointsSphere[i][j][0], pointsSphere[i][j][1], pointsSphere[i][j][2]);
        glEnd();
    }

    // Elevation / Längengrade zeichnen
    for (int i = 0; i < numLines; i++) {
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < numLines; j++)
            glVertex3f(pointsSphere[i][j][0], pointsSphere[i][j][1], pointsSphere[i][j][2]);
        glEnd();
    }


    // Array freigeben
    for (int i = 0; i < numLines; i++)
        for (int j = 0; j < numLines; j++)
        {
            delete pointsSphere[i][j];
        }
    for (int i = 0; i < numLines; i++)
    {
        delete pointsSphere[i];
    }
}

/* -- 2.5 Relative Positionierung -- */
void drawPiFighter(float radius) {
    // "Cockpit"
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    drawSphere(radius, 40);
    glPopMatrix();

    // Rechter "Flügel"
    glPushMatrix();
    glTranslatef(tx, ty, tz + radius);
    drawGrid(1.2F, 40);
    glPopMatrix();

    // Linker "Flügel"
    glPushMatrix();
    glTranslatef(tx, ty, tz - radius);
    drawGrid(1.2F, 40);
    glPopMatrix();

    // Rotiere nach Parmetern
    glLoadIdentity();   // setzt Rotation zuürck, bevor neuer Wert zugewiesen wird
    glRotatef(rx, 1.0F, 0.0F, 0.0F);
    glRotatef(ry, 0.0F, 1.0F, 0.0F);
    glRotatef(rz, 0.0F, 0.0F, 1.0F);
}


/* -- 2.6 & 2.7 Tastatureingaben -- */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // Bewegung / Transformieren der Koordinaten
    if (key == GLFW_KEY_UP)     { ty += 0.05F; }
    if (key == GLFW_KEY_DOWN)   { ty -= 0.05F; }
    if (key == GLFW_KEY_LEFT)   { tx -= 0.05F; }
    if (key == GLFW_KEY_RIGHT)  { tx += 0.05F; }

    // Drehung um X
    if (key == GLFW_KEY_X) { rx += 5.0F; }
    if (key == GLFW_KEY_S) { rx -= 5.0F; }

    // Drehung um Y
    if (key == GLFW_KEY_Y) { ry += 5.0F; }
    if (key == GLFW_KEY_A) { ry += 5.0F; }

    // Drehung um Z
    if (key == GLFW_KEY_Z) { rz += 5.0F; }
    if (key == GLFW_KEY_6) { rz += 5.0F; }

    // Reset:
    if (key == GLFW_KEY_ESCAPE) {
        tx = 0.0f;
        ty = 0.0f;
        tz = 0.0f;
        rx = 1.0f;
        ry = 1.0f;
        rx = 1.0f;
    }
}

/* -- 2.8 Drucken einer Matrix -- */
void printmatrix(float mat[16], const char* title, bool redraw)
{
    if (redraw)
        system("cls");
    fprintf(stdout, "%s\n", title);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fprintf(stdout, "\t%f ", mat[j * 4 + i]);
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

// Main
int main()
{
    float rotationAngle = 0;
    GLFWwindow* window;
    if (!glfwInit())
        exit(EXIT_FAILURE);
    else
        std::cout << "GLFW init successful!\n";

    window = glfwCreateWindow(1280, 1024, "Hello OpenGL World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    // Float-Array für auslesen der Matrix
    float modelview[16];

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);

        rotationAngle = glfwGetTime() * 360 / 60;   // Updated angle by seconds passed
        drawCoordinateSystem(); // Koordinatensystem zeichnen

        // Rotiere um Achse (1,1,1)
        //glRotatef(rotationAngle * 6, 1.0F, 1.0F, 1.0F);

        //drawClock(rotationAngle);
        drawPiFighter(0.3F); // 0.3 = Radius der Kugel

        // Matrix Zeichnen
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
        printmatrix(modelview, "Data", true);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

}

