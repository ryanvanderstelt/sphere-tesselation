//------------------------------------------------------------------------------------
// sphere.cpp
//
// Developed by: Ryan Vander Stelt
//
// This is my implementation of generating a sphere from scratch.
// It can be rendered by feeding vertices into GL_TRIANGLES.
//------------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <cmath>
#include <GL/glut.h>
#include "sphere.h"

// Sphere class constructor
Sphere::Sphere(int deg, double rad)
{
    degree = deg;
    radius = rad;
    if (degree < 4)
    {
        degree = 4;
    }
    else if (degree % 2)
    {
        degree++;
    }
    if (degree % 4)
    {
        base = 6;
    }
    else
    {
        base = 4;
    }

    getCounts();

    unique_vertices = new GLfloat *[unique_vertex_count];
    unique_tex_coords = new GLdouble *[unique_vertex_count];
    for (int i = 0; i < unique_vertex_count; i++)
    {
        unique_vertices[i] = new GLfloat[3];
        unique_tex_coords[i] = new GLdouble[2];
    }

    faces = new GLfloat **[face_count];
    tex_coords = new GLdouble **[face_count];
    for (int i = 0; i < face_count; i++)
    {
        faces[i] = new GLfloat *[3];
        tex_coords[i] = new GLdouble *[3];
        for (int j = 0; j < 3; j++)
        {
            faces[i][j] = new GLfloat[3];
            tex_coords[i][j] = new GLdouble[2];
        }
    }

    generateVertices();
    generateSphere();
}

// Sphere class destructor
Sphere::~Sphere()
{
    for (int i = 0; i < unique_vertex_count; i++)
    {
        delete[] unique_tex_coords[i];
        delete[] unique_vertices[i];
    }
    delete[] unique_tex_coords;
    delete[] unique_vertices;

    for (int i = 0; i < face_count; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            delete[] faces[i][j];
            delete[] tex_coords[i][j];
        }
        delete[] faces[i];
        delete[] tex_coords[i];
    }
    delete[] faces;
    delete[] tex_coords;
}

// Calculate the number of unique vertices and faces
void Sphere::getCounts()
{
    int count = 2;

    for (int i = degree; i >= base; i -= 4)
    {
        count += i * 2;
    }

    if (degree % 4 == 0)
    {
        count -= degree;
    }

    unique_vertex_count = count;
    face_count = (count - 2) * 2;
    total_vertex_count = face_count * 3;
}

// Calculate all unique vertices
void Sphere::generateVertices()
{
    bool offset = degree % 4;

    // Set first/last vertex to top/bottom of sphere
    unique_vertices[0][0] = 0;
    unique_vertices[0][1] = radius;
    unique_vertices[0][2] = 0;
    unique_tex_coords[0][0] = 0.5;
    unique_tex_coords[0][1] = 0;

    unique_vertices[unique_vertex_count - 1][0] = 0;
    unique_vertices[unique_vertex_count - 1][1] = -radius;
    unique_vertices[unique_vertex_count - 1][2] = 0;
    unique_tex_coords[unique_vertex_count - 1][0] = 0.5;
    unique_tex_coords[unique_vertex_count - 1][1] = 1;

    // Generate the rest of the vertices
    double xy_angle = 2 * pi / degree;
    int v_index = 1;
    int half_middle_layer_count = degree / 4;

    for (int layer = 0; layer < half_middle_layer_count; layer++)
    {
        int v_per_l = base + layer * 4;
        double xz_angle = 2 * pi / v_per_l;
        double l_offset;
        if (offset)
        {
            l_offset = xz_angle / 2;
        }
        else
        {
            l_offset = 0;
        }
        int start = v_index;
        for (int v = 0; v < v_per_l; v++)
        {
            unique_vertices[v_index][0] = radius * sin(xy_angle * (layer + 1)) * sin(xz_angle * v);
            unique_vertices[v_index][1] = radius * cos(xy_angle * (layer + 1));
            unique_vertices[v_index][2] = radius * sin(xy_angle * (layer + 1)) * cos(xz_angle * v);
            unique_tex_coords[v_index][0] = (double)v / v_per_l;
            unique_tex_coords[v_index][1] = ((double)layer + 1) / (degree / 2);

            if (layer + 1 < half_middle_layer_count || offset)
            {
                unique_vertices[unique_vertex_count - start - v_per_l + v][0] = radius * sin(xy_angle * (layer + 1)) * sin(xz_angle * v - l_offset);
                unique_vertices[unique_vertex_count - start - v_per_l + v][1] = -radius * cos(xy_angle * (layer + 1));
                unique_vertices[unique_vertex_count - start - v_per_l + v][2] = radius * sin(xy_angle * (layer + 1)) * cos(xz_angle * v - l_offset);
                unique_tex_coords[unique_vertex_count - start - v_per_l + v][0] = (double)v / v_per_l - l_offset / (2 * pi);
                unique_tex_coords[unique_vertex_count - start - v_per_l + v][1] = 1.0 - ((double)layer + 1) / (degree / 2);
            }
            v_index++;
        }
    }

    unique_tex_coords[0][0] = 0.5;
    unique_tex_coords[0][1] = 0.0;
    unique_tex_coords[unique_vertex_count - 1][0] = 0.5;
    unique_tex_coords[unique_vertex_count - 1][1] = 1.0;
    for (int i = 1; i < unique_vertex_count - 2; i++)
    {
        double rel_x = unique_vertices[i][0] / radius;
        double rel_y = unique_vertices[i][1] / radius;
        double rel_z = unique_vertices[i][2] / radius;

        unique_tex_coords[i][0] = atan2(rel_x, rel_z) / (2 * pi);
        unique_tex_coords[i][1] = acos(rel_y) / pi;

        if (unique_tex_coords[i][0] < 0)
        {
            unique_tex_coords[i][0] += 1;
        }
    }
}

// Use unique vertices to put together a sphere able to be rendered using GL_TRIANGLES
void Sphere::generateSphere()
{
    int insert_index = 0;
    bool add_a = true;
    bool inc_a = false;
    int stall_val = 0;
    int a_iter = 0;
    int b_iter = 0;
    int a_base;
    int b_base;

    int num_layers = degree / 2 + 1;
    int layers[num_layers];
    layers[0] = 1;
    layers[num_layers - 1] = 1;

    // Create array of vertices per layer
    if (base == 4)
    {
        for (int i = 1; i <= degree / 4; i++)
        {
            layers[i] = 4 * i;
            layers[num_layers - i - 1] = 4 * i;
        }
    }
    else
    {
        for (int i = 1; i <= degree / 4; i++)
        {
            layers[i] = 2 + 4 * i;
            layers[num_layers - i - 1] = 2 + 4 * i;
        }
    }

    // List vertices for GL_TRIANGLES between top two layers
    for (int i = 0; i < base; i++)
    {
        faces[insert_index][0][0] = unique_vertices[0][0];
        faces[insert_index][0][1] = unique_vertices[0][1];
        faces[insert_index][0][2] = unique_vertices[0][2];
        tex_coords[insert_index][0][0] = unique_tex_coords[0][0];
        tex_coords[insert_index][0][1] = unique_tex_coords[0][1];

        faces[insert_index][1][0] = unique_vertices[1 + i][0];
        faces[insert_index][1][1] = unique_vertices[1 + i][1];
        faces[insert_index][1][2] = unique_vertices[1 + i][2];
        tex_coords[insert_index][1][0] = unique_tex_coords[1 + i][0];
        tex_coords[insert_index][1][1] = unique_tex_coords[1 + i][1];

        faces[insert_index][2][0] = unique_vertices[1 + (1 + i) % base][0];
        faces[insert_index][2][1] = unique_vertices[1 + (1 + i) % base][1];
        faces[insert_index][2][2] = unique_vertices[1 + (1 + i) % base][2];
        tex_coords[insert_index][2][0] = unique_tex_coords[1 + (1 + i) % base][0];
        tex_coords[insert_index][2][1] = unique_tex_coords[1 + (1 + i) % base][1];
        insert_index++;
    }

    // List vertices for GL_TRIANGLES between layers i and i + 1
    b_base = 1;
    for (int i = 1; i < num_layers - 2; i++)
    {
        int triangles = layers[i] + layers[i + 1];
        a_base = b_base;
        b_base = a_base + layers[i];
        a_iter = 0;
        b_iter = 0;
        stall_val = 0;

        for (int j = 0; j < triangles; j++)
        {
            if (base == 4)
            {
                if (!(j % (triangles / 4)))
                {
                    if (layers[i] != layers[i + 1])
                    {
                        inc_a = layers[i + 1] < layers[i];
                    }
                }
            }
            else
            {
                if (j == stall_val)
                {
                    if (layers[i] != layers[i + 1])
                    {
                        inc_a = layers[i + 1] < layers[i];

                        if (add_a)
                        {
                            stall_val += layers[i] / 2;
                        }
                        else
                        {
                            stall_val += layers[i + 1] / 2;
                        }
                        add_a = !add_a;
                    }
                    else if (degree > 6)
                    {
                        inc_a = false;
                    }
                }
            }
            faces[insert_index][0][0] = unique_vertices[a_base + a_iter][0];
            faces[insert_index][0][1] = unique_vertices[a_base + a_iter][1];
            faces[insert_index][0][2] = unique_vertices[a_base + a_iter][2];
            tex_coords[insert_index][0][0] = unique_tex_coords[a_base + a_iter][0];
            tex_coords[insert_index][0][1] = unique_tex_coords[a_base + a_iter][1];

            faces[insert_index][1][0] = unique_vertices[b_base + b_iter][0];
            faces[insert_index][1][1] = unique_vertices[b_base + b_iter][1];
            faces[insert_index][1][2] = unique_vertices[b_base + b_iter][2];
            tex_coords[insert_index][1][0] = unique_tex_coords[b_base + b_iter][0];
            tex_coords[insert_index][1][1] = unique_tex_coords[b_base + b_iter][1];

            if (inc_a)
            {
                a_iter = (a_iter + 1) % layers[i];
                faces[insert_index][2][0] = unique_vertices[a_base + a_iter][0];
                faces[insert_index][2][1] = unique_vertices[a_base + a_iter][1];
                faces[insert_index][2][2] = unique_vertices[a_base + a_iter][2];
                tex_coords[insert_index][2][0] = unique_tex_coords[a_base + a_iter][0];
                tex_coords[insert_index][2][1] = unique_tex_coords[a_base + a_iter][1];
                insert_index++;
            }
            else
            {
                b_iter = (b_iter + 1) % layers[i + 1];

                faces[insert_index][2][0] = unique_vertices[b_base + b_iter][0];
                faces[insert_index][2][1] = unique_vertices[b_base + b_iter][1];
                faces[insert_index][2][2] = unique_vertices[b_base + b_iter][2];
                tex_coords[insert_index][2][0] = unique_tex_coords[b_base + b_iter][0];
                tex_coords[insert_index][2][1] = unique_tex_coords[b_base + b_iter][1];
                insert_index++;
            }
            inc_a = !inc_a;
        }
        if (base == 4)
        {
            tex_coords[insert_index - 1][2][0] += 1.0;
            tex_coords[insert_index - 2][2][0] += 1.0;
            if (i < num_layers / 2)
            {
                tex_coords[insert_index - 1][0][0] += 1.0;
            }
            else
            {
                tex_coords[insert_index - 1][1][0] += 1.0;
            }
        }
        else if (degree == 6)
        {
            tex_coords[insert_index - 1][2][0] += 1.0;
            tex_coords[insert_index - triangles][1][0] -= 1.0;
        }
        else
        {
            if (i < num_layers / 2 - 1)
            {
                tex_coords[insert_index - 1][1][0] -= 1.0;
                tex_coords[insert_index - 2][2][0] += 1.0;
            }
            else if (i > num_layers / 2 - 1)
            {
                tex_coords[insert_index - triangles][2][0] += 1.0;
                tex_coords[insert_index - triangles + 1][1][0] -= 1.0;
            }
            else
            {
                tex_coords[insert_index - 1][2][0] += 1.0;
                tex_coords[insert_index - triangles][1][0] -= 1.0;
            }
        }
    }
    // List vertices for GL_TRIANGLES between bottom two layers
    for (int i = 0; i < base; i++)
    {
        a_base = unique_vertex_count - base - 1;

        faces[insert_index][0][0] = unique_vertices[a_base + i][0];
        faces[insert_index][0][1] = unique_vertices[a_base + i][1];
        faces[insert_index][0][2] = unique_vertices[a_base + i][2];
        tex_coords[insert_index][0][0] = unique_tex_coords[unique_vertex_count - 1][0];
        tex_coords[insert_index][0][1] = unique_tex_coords[unique_vertex_count - 1][1];

        faces[insert_index][1][0] = unique_vertices[unique_vertex_count - 1][0];
        faces[insert_index][1][1] = unique_vertices[unique_vertex_count - 1][1];
        faces[insert_index][1][2] = unique_vertices[unique_vertex_count - 1][2];
        tex_coords[insert_index][1][0] = unique_tex_coords[a_base + i][0] + 1.0 / base;
        tex_coords[insert_index][1][1] = unique_tex_coords[unique_vertex_count - 1][1];

        faces[insert_index][2][0] = unique_vertices[a_base + (i + 1) % base][0];
        faces[insert_index][2][1] = unique_vertices[a_base + (i + 1) % base][1];
        faces[insert_index][2][2] = unique_vertices[a_base + (i + 1) % base][2];
        tex_coords[insert_index][2][0] = unique_tex_coords[a_base + (i + 1) % base][0];
        tex_coords[insert_index][2][1] = unique_tex_coords[a_base + (i + 1) % base][1];
        insert_index++;
    }
    // tex_coords[base - 1][2][0] += 1
}

void Sphere::resize(float rad)
{
    for (int i = 0; i < unique_vertex_count; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            unique_vertices[i][j] *= rad / radius;
        }
    }
    for (int i = 0; i < face_count; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                faces[i][j][k] *= rad / radius;
            }
        }
    }
}