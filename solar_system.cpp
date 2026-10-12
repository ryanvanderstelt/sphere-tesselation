//--------------------------------------------------------------------------
// Developer -- Ryan Vander Stelt
// Course ----- CS3233
// Project ---- In Conclusion
//
// In this project, I built an lighted, textured, and animated solar system.
//--------------------------------------------------------------------------

#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include "camera.h"
#include "sphere.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MERCURY_TEXTURE GL_TEXTURE0
#define VENUS_TEXTURE GL_TEXTURE1
#define EARTH_TEXTURE GL_TEXTURE2
#define MOON_TEXTURE GL_TEXTURE3
#define MARS_TEXTURE GL_TEXTURE4
#define JUPITER_TEXTURE GL_TEXTURE5
#define SATURN_TEXTURE GL_TEXTURE6
#define URANUS_TEXTURE GL_TEXTURE7
#define NEPTUNE_TEXTURE GL_TEXTURE8
#define RING_TEXTURE GL_TEXTURE9

const float pi = 3.14159;
float start;
float day_speed;
int view_idx = 0;

GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat black[3] = {0, 0, 0};

int res = 200;

GLuint textures[9];

Sphere sun(res, 2);
Sphere mercury(res, 0.3);
Sphere venus(res, 0.3);
Sphere earth(res, 0.4);
Sphere moon(res, 0.1);
Sphere mars(res, 0.4);
Sphere jupiter(res, 0.7);
Sphere saturn(res, 0.55);
Sphere uranus(res, 0.25);
Sphere neptune(res, 0.25);

//------------------------------------------------------------------------------------
// drawOrbit()
//
// Given a radius and degree, this function draws a circle on the xz plane used for
// displaying orbits.
//------------------------------------------------------------------------------------
void drawOrbit(float radius, int degree)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < degree; i++)
    {
        glVertex3d(radius * sin(2 * pi * i / degree), 0, radius * cos(2 * pi * i / degree));
    }
    glVertex3d(0, 0, radius);

    glEnd();
}

//------------------------------------------------------------------------------------
// display()
//
// Handler for window-repaint event. Call back when the window first appears and
// whenever the window needs to be re-painted.
//------------------------------------------------------------------------------------
void display()
{
    float distance = 0;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, 0);

    cameraSetLimits(-20, 20, -20, 20, -20, 20);
    cameraApply();

    GLfloat light_pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    for (int i = 0; i < 10; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glDisable(GL_TEXTURE_2D);
    }

    // Sun
    {

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < sun.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glNormal3f(-sun.faces[i][j][0], -sun.faces[i][j][1], -sun.faces[i][j][2]);
                glVertex3fv(sun.faces[i][j]);
            }
        }
        glEnd();
    }

    // Mercury
    {
        distance += 4;
        drawOrbit(distance, 200);
        static double rot = 360.0 / 58.6 * start;
        static double orbit = 360.0 / 88.0 * start;

        rot += 360.0 / 60.0 / 58.6 * day_speed;
        orbit += 360.0 / 60.0 / 88.0 * day_speed;

        rot = fmod(rot, 360.0);
        orbit = fmod(orbit, 360.0);

        glPushMatrix();
        glRotated(orbit, 0, 1.0, 0);
        glTranslated(0, 0, distance);
        glRotated(rot, 0, 1.0, 0);

        glActiveTexture(MERCURY_TEXTURE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < mercury.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glMultiTexCoord2dv(MERCURY_TEXTURE, mercury.tex_coords[i][j]);
                glNormal3fv(mercury.faces[i][j]);
                glVertex3fv(mercury.faces[i][j]);
            }
        }
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    // Venus
    {
        distance += 2;
        drawOrbit(distance, 200);
        static double rot = 360.0 / 243.0 * start;
        static double orbit = 360.0 / 224.7 * start;

        rot -= 360.0 / 60.0 / 243.0 * day_speed;
        orbit += 360.0 / 60.0 / 224.7 * day_speed;

        rot = fmod(rot, 360.0);
        orbit = fmod(orbit, 360.0);

        glPushMatrix();
        glRotated(orbit, 0, 1.0, 0);
        glTranslated(0, 0, distance);
        glRotated(rot, 0, 1.0, 0);

        glActiveTexture(VENUS_TEXTURE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, black);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < venus.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glMultiTexCoord2dv(VENUS_TEXTURE, venus.tex_coords[i][j]);
                glNormal3fv(venus.faces[i][j]);
                glVertex3fv(venus.faces[i][j]);
            }
        }
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    // Earth
    {
        distance += 2;
        drawOrbit(distance, 200);
        static double rot = 360.0 / 1.0 * start;
        static double orbit = 360.0 / 365.25 * start;

        rot += 360.0 / 60.0 / 1.0 * day_speed;
        orbit += 360.0 / 60.0 / 365.25 * day_speed;

        rot = fmod(rot, 360.0);
        orbit = fmod(orbit, 360.0);

        glPushMatrix();
        glRotated(orbit, 0, 1.0, 0);
        glTranslated(0, 0, distance);
        glPushMatrix();
        glRotated(rot, 0, 1.0, 0);

        glActiveTexture(EARTH_TEXTURE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[2]);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < earth.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glMultiTexCoord2dv(EARTH_TEXTURE, earth.tex_coords[i][j]);
                glNormal3fv(earth.faces[i][j]);
                glVertex3fv(earth.faces[i][j]);
            }
        }
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    // Moon
    {
        static double rot = 360.0 / 27.3 * start;
        static double orbit = 360.0 / 27.3 * start;

        orbit += 360.0 / 60.0 / 27.3 * day_speed;

        rot = fmod(rot, 360.0);
        orbit = fmod(orbit, 360.0);

        glPushMatrix();
        glRotated(orbit, 0, 1.0, 0);
        glTranslated(0, 0, earth.radius * 1.5);
        glRotated(rot, 0, 1.0, 0);

        glActiveTexture(MOON_TEXTURE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[3]);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < moon.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glMultiTexCoord2dv(MOON_TEXTURE, moon.tex_coords[i][j]);
                glNormal3fv(moon.faces[i][j]);
                glVertex3fv(moon.faces[i][j]);
            }
        }
        glEnd();
        glPopMatrix();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    // Mars
    {
        distance += 2;
        drawOrbit(distance, 200);
        static double rot = 360.0 / 1.02595 * start;
        static double orbit = 360.0 / 687.0 * start;

        rot += 360.0 / 60.0 / 1.02595 * day_speed;
        orbit += 360.0 / 60.0 / 687.0 * day_speed;

        rot = fmod(rot, 360.0);
        orbit = fmod(orbit, 360.0);

        glPushMatrix();
        glRotated(orbit, 0, 1.0, 0);
        glTranslated(0, 0, distance);
        glRotated(rot, 0, 1.0, 0);

        glActiveTexture(MARS_TEXTURE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[4]);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < mars.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glMultiTexCoord2dv(MARS_TEXTURE, mars.tex_coords[i][j]);
                glNormal3fv(mars.faces[i][j]);
                glVertex3fv(mars.faces[i][j]);
            }
        }
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    // Jupiter
    {
        distance += 2;
        drawOrbit(distance, 200);
        static double rot = 360.0 / 0.41 * start;
        static double orbit = 360.0 / 4333 * start;

        rot += 360.0 / 60.0 / 0.41 * day_speed;
        orbit += 360.0 / 60.0 / 4333 * day_speed;

        rot = fmod(rot, 360.0);
        orbit = fmod(orbit, 360.0);

        glPushMatrix();
        glRotated(orbit, 0, 1.0, 0);
        glTranslated(0, 0, distance);
        glRotated(rot, 0, 1.0, 0);

        glActiveTexture(JUPITER_TEXTURE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[5]);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < jupiter.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glMultiTexCoord2dv(JUPITER_TEXTURE, jupiter.tex_coords[i][j]);
                glNormal3fv(jupiter.faces[i][j]);
                glVertex3fv(jupiter.faces[i][j]);
            }
        }
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    // Saturn
    {
        distance += 2;
        drawOrbit(distance, 200);
        static double rot = 360.0 / 0.45 * start;
        static double orbit = 360.0 / 10756.0 * start;

        rot += 360.0 / 60.0 / 0.45 * day_speed;
        orbit += 360.0 / 60.0 / 10756.0 * day_speed;

        rot = fmod(rot, 360.0);
        orbit = fmod(orbit, 360.0);

        glPushMatrix();
        glRotated(orbit, 0, 1.0, 0);
        glTranslated(0, 0, distance);

        // Saturn's Rings
        GLfloat ring_color[3] = {0.87, 0.74, 0.57};

        glMaterialfv(GL_FRONT, GL_DIFFUSE, ring_color);
        glMaterialfv(GL_FRONT, GL_SPECULAR, ring_color);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ring_color);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        GLfloat vector[3] = {0.0, 1.0, 0.0};
        GLfloat inner_1[3];
        GLfloat inner_2[3];
        GLfloat outer_1[3];
        GLfloat outer_2[3];
        float i_rad = 0.6;
        float o_rad = 1;
        glBegin(GL_TRIANGLES);

        for (int i = 0; i < 200; i++)
        {
            inner_1[0] = i_rad * sin(2 * pi * i / 200);
            inner_1[1] = 0;
            inner_1[2] = i_rad * cos(2 * pi * i / 200);

            inner_2[0] = i_rad * sin(2 * pi * (i + 1) / 200);
            inner_2[1] = 0;
            inner_2[2] = i_rad * cos(2 * pi * (i + 1) / 200);

            outer_1[0] = o_rad * sin(2 * pi * i / 200);
            outer_1[1] = 0;
            outer_1[2] = o_rad * cos(2 * pi * i / 200);

            outer_2[0] = o_rad * sin(2 * pi * (i + 1) / 200);
            outer_2[1] = 0;
            outer_2[2] = o_rad * cos(2 * pi * (i + 1) / 200);

            glVertex3fv(inner_1);
            glVertex3fv(outer_1);
            glVertex3fv(outer_2);

            glVertex3fv(inner_1);
            glVertex3fv(outer_2);
            glVertex3fv(inner_2);
        }
        glEnd();

        glRotated(rot, 0, 1.0, 0);

        glActiveTexture(SATURN_TEXTURE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[6]);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < saturn.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glMultiTexCoord2dv(SATURN_TEXTURE, saturn.tex_coords[i][j]);
                glNormal3fv(saturn.faces[i][j]);
                glVertex3fv(saturn.faces[i][j]);
            }
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    // Uranus
    {
        distance += 2;
        drawOrbit(distance, 200);
        static double rot = 360.0 / 0.72 * start;
        static double orbit = 360.0 / 30687.0 * start;

        rot += 360.0 / 60.0 / 0.72 * day_speed;
        orbit += 360.0 / 60.0 / 30687.0 * day_speed;

        rot = fmod(rot, 360.0);
        orbit = fmod(orbit, 360.0);

        glPushMatrix();
        glRotated(orbit, 0, 1.0, 0);
        glTranslated(0, 0, distance);
        glRotated(rot, 0, 1.0, 0);

        glActiveTexture(URANUS_TEXTURE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[7]);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < uranus.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glMultiTexCoord2dv(URANUS_TEXTURE, uranus.tex_coords[i][j]);
                glNormal3fv(uranus.faces[i][j]);
                glVertex3fv(uranus.faces[i][j]);
            }
        }
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    // Neptune
    {
        distance += 2;
        drawOrbit(distance, 200);
        static double rot = 360.0 / 0.67 * start;
        static double orbit = 360.0 / 60190.0 * start;

        rot += 360.0 / 60.0 / 0.67 * day_speed;
        orbit += 360.0 / 60.0 / 60190.0 * day_speed;

        rot = fmod(rot, 360.0);
        orbit = fmod(orbit, 360.0);

        glPushMatrix();
        glRotated(orbit, 0, 1.0, 0);
        glTranslated(0, 0, distance);
        glRotated(rot, 0, 1.0, 0);

        glActiveTexture(URANUS_TEXTURE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[8]);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < uranus.face_count; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glMultiTexCoord2dv(URANUS_TEXTURE, uranus.tex_coords[i][j]);
                glNormal3fv(uranus.faces[i][j]);
                glVertex3fv(uranus.faces[i][j]);
            }
        }
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    glFlush();
}

// Control Redisplay timing
void timer(int unused)
{
    glutPostRedisplay();
    glutTimerFunc(17, timer, 0);
}

// Initialize camera API, lighting, and textures
void init()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_ambient[] = {0, 0, 0, 0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);

    std::string dir = "./textures/";
    std::string filenames[10] = {
        "mercury.jpg",
        "venus.jpg",
        "earth.jpg",
        "moon.jpg",
        "mars.jpg",
        "jupiter.jpg",
        "saturn.jpg",
        "uranus.jpg",
        "neptune.jpg",
    };

    int imgWidth, imgHeight, bytesPerPixel;
    unsigned char *data;

    glGenTextures(9, textures);

    for (int i = 0; i < 9; i++)
    {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load((dir + filenames[i]).c_str(), &imgWidth, &imgHeight, &bytesPerPixel, 0);

        if (!data)
        {
            std::cout << "Failed to load texture: " << filenames[i] << std::endl;
            continue;
        }

        if (filenames[i].find(".png") > -1)
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, imgWidth, imgHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
        glDisable(GL_TEXTURE_2D);
    }

    glutMouseFunc(trackballMouseFunction);
    glutMotionFunc(trackballMotionFunction);
    return;
}

int main(int argc, char **argv)
{
    std::cout << "Enter # of days passed: ";
    std::cin >> start;
    std::cout << "Enter days per second: ";
    std::cin >> day_speed;
    glutInit(&argc, argv);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Dynamic Duo");
    glutDisplayFunc(display);
    init();
    timer(0);
    glutMainLoop();
    return 0;
}
