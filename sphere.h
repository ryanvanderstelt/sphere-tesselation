//------------------------------------------------------------------------------------
// sphere.h
//
// Developed by: Ryan Vander Stelt
//
// This is the definition of my sphere class.
//------------------------------------------------------------------------------------
#include <GL/glut.h>

#ifndef SPHERE_H
#define SPHERE_H

// This is a class to generate a sphere to be drawn with GL_TRIANGLES
class Sphere
{
private:
    int base;
    const double pi = 3.14159265;
    void getCounts();
    void generateVertices();
    void generateSphere();

public:
    int degree;
    double radius;
    int unique_vertex_count;
    int total_vertex_count;
    int face_count;
    GLfloat **unique_vertices;
    GLdouble **unique_tex_coords;
    GLfloat ***faces;
    GLdouble ***tex_coords;
    void resize(float radius);
    Sphere(int deg, double rad);
    ~Sphere();
};

#endif