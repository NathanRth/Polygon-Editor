#ifndef POLYGON_H
#define POLYGON

#include <stdlib.h>

#include "vec2.h"
#include "Image.h"

enum PolyType
{
    CLOSED,
    OPENED
};

typedef struct s_vertex
{
    struct s_vertex *next;
    Vec2 position;
} Vertex;

typedef struct s_polygon
{
    struct s_vertex *first;
    struct s_vertex *last;
    enum PolyType type;
    size_t size;
} Polygon;

// create empty polygon
Polygon *POLY_init();

// return a pointer to the vertex at the index position in polygon list
Vertex *POLY_at(Polygon *poly, size_t index);

// append vertex at the end of the poly chain
void POLY_appendVertex(Polygon *poly, Vec2 pos);

// insert new vertex from position after vertex in poly at index
void POLY_insertVertex(Polygon *poly, Vec2 pos, size_t index);

// remove specified vertex from poly chain
void POLY_removeVertex(Polygon *poly, size_t index);

// draw the polygon
void POLY_draw(Polygon *poly, int fill, Image *img);

// draw a small point at vertex position
void POLY_drawVertex(Polygon *poly, size_t index, Image *img);

// draw a square around selected vertex
void POLY_drawSelectedVertex(Polygon *poly, size_t index, Image *img);

// draw the selected edge in red
void POLY_drawSelectedEdge(Polygon *poly, size_t index, Image *img);

// close or open the poly
void POLY_openClose(Polygon *poly);

// return the index of the closest vertex
size_t POLY_closestVertex(Polygon *poly, Vec2 mousePos);

// return the index of the first vertex of the closest edge
size_t POLY_closestEdge(Polygon *poly, Vec2 mousePos);

// find the dimensions of a given poly
void POLY_findDimensions(Polygon *poly, int *highest, int *lowest);

// fill the polygon using scanline algorithm
void POLY_fill(Polygon *poly, Image *img);

#endif