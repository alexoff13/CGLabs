#include <OpenGL/glu.h>
#include <GLUT/glut.h>

GLfloat xRotated = 1, yRotated = 1, zRotated = 1;
GLdouble innerRadius = 0.5;
GLdouble outerRadius = 1;
GLint sides = 20;
GLint rings = 20;


void display() {
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTranslatef(0.0, 0.0, -4.5);
    glColor3f(0.3, 0.5, 0.1);
    glRotatef(xRotated, 1.0, 0.0, 0.0);
    glRotatef(yRotated, 0.0, 1.0, 0.0);
    glRotatef(zRotated, 0.0, 0.0, 1.0);
    glScalef(1.0, 1.0, 1.0);

    glShadeModel(GL_FLAT);
    glTranslatef(0.0, 0.0, -2.25);
    glutWireSphere(innerRadius, sides, rings);

    glTranslatef(0.0, 0.0, 1.5);
    glutSolidSphere(innerRadius, sides, rings);

    glShadeModel(GL_SMOOTH);
    glTranslatef(0.0, 0.0, 1.5);
    glutSolidSphere(innerRadius, sides, rings);

    glTranslatef(0.0, 0.0, 1.5);
    glutSolidSphere(innerRadius, sides*3, rings*3);

    glPopMatrix();
    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-1.5, 1.5, -1.5 * (GLfloat) h / (GLfloat) w,
                1.5 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-1.5 * (GLfloat) w / (GLfloat) h,
                1.5 * (GLfloat) w / (GLfloat) h, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    gluPerspective(90.0, (GLdouble) w / (GLdouble) h, 0.5, 20.0);
    glLoadIdentity();
}

void idle() {
    yRotated -= 1;
    glutPostRedisplay();
}

void keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT and rings > 1) {
        --sides;
        --rings;
    } else if (key == GLUT_KEY_RIGHT and rings < 200) {
        ++sides;
        ++rings;
    }
}

void init() {
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutSpecialFunc(keyboard);

    glutMainLoop();
    return 0;
}