#include "polygon.h"
#include "utils.h"

Polygon *POLY_init()
{
    Polygon *poly = malloc(sizeof(Polygon));
    poly->first = NULL;
    poly->last = NULL;
    poly->type = OPENED;
    poly->size = 0;
    return poly;
}

Vertex *POLY_at(Polygon *poly, size_t index)
{
    Vertex *current = poly->first;
    size_t i = 0;
    while (current != NULL && i < index)
    {
        current = current->next;
        i++;
    }
    return current;
}

void POLY_appendVertex(Polygon *poly, Vec2 pos)
{
    printf("new vertex at (%f,%f)\n", pos.x, pos.y);
    Vertex *new = (Vertex *)(malloc(sizeof(Vertex)));
    ;
    new->position = pos;

    // if first vertex in poly, make first and last point
    // to the new vertex, but new has no ->next value
    if (poly->first == NULL)
    {
        new->next = NULL;
        poly->first = new;
        poly->last = new;
    }
    // in general case, update the poly->last pointer
    else
    {
        new->next = poly->type == CLOSED ? poly->first : NULL;
        poly->last->next = new;
        poly->last = new;
    }
    poly->size++;
}

void POLY_insertVertex(Polygon *poly, Vec2 pos, size_t index)
{
    Vertex *current = POLY_at(poly, index);
    Vertex *new = (Vertex *)(malloc(sizeof(Vertex)));
    new->next = current->next;
    new->position = pos;
    if (current == poly->last)
    {
        poly->last = new;
    }
    current->next = new;
    poly->size++;
}

void POLY_removeVertex(Polygon *poly, size_t index)
{
    Vertex *vert = POLY_at(poly, index);

    // if the selected vertex is the first vertex
    if (vert == poly->first)
    {
        if (poly->size > 2)
        {
            poly->first = vert->next;
            if (poly->type == CLOSED)
            {
                poly->last->next = poly->first;
            }
        }
        if (poly->size == 2)
        {
            // poly->first & poly->last link to
            // the vertex remaining, without
            // link to one and other
            //(only one vertex -> nothing to draw)
            poly->first = poly->last;
            poly->first->next = NULL;
        }
        // only one vertex remaining
        else if (poly->size == 1)
        {
            poly->first = NULL;
            poly->last = NULL;
        }
    }
    else if (vert == poly->last)
    {
        Vertex *prev = poly->first;
        while (prev->next != vert)
        {
            prev = prev->next;
        }
        prev->next = NULL;
        poly->last = prev;
        if (poly->type == CLOSED)
        {
            poly->last->next = poly->first;
        }
    }
    else // is a vertex in the middle of the poly
    {
        Vertex *prev = poly->first;
        while (prev->next != vert)
        {
            prev = prev->next;
        }
        if (prev->next->next != NULL)
        {
            prev->next = prev->next->next;
        }
        else
        {
            prev->next = NULL;
        }
    }
    free(vert);
    poly->size--;
}

void POLY_openClose(Polygon *poly)
{
    if (poly->type == OPENED) // si ouvert -> fermer
    {
        poly->type = CLOSED;
        poly->last->next = poly->first;
    }
    else // si fermé -> ouvrir
    {
        poly->type = OPENED;
        poly->last->next = NULL;
    }
}

void POLY_draw(Polygon *poly, int fill, Image *img)
{
    size_t i = 0;
    if (poly->first != NULL)
    {
        POLY_drawVertex(poly, i, img);
        if (poly->first->next != NULL)
        {
            // for all vertices, draw a line on img
            Vertex *current = poly->first;
            do
            {
                i++;
                I_bresenham(img, current->position.x, current->position.y,
                            current->next->position.x, current->next->position.y);
                POLY_drawVertex(poly, i, img);
                current = current->next;
            // check if not comming back to first edge or reach end of the polygon
            } while (current != poly->first && current != NULL && current->next != NULL);
        }
    }
}

void POLY_drawVertex(Polygon *poly, size_t index, Image *img)
{
    Vertex *current = POLY_at(poly, index);
    if (current != NULL)
    {
        I_drawSquare(img, current->position, 3);
    }
}

void POLY_drawSelectedVertex(Polygon *poly, size_t index, Image *img)
{
    Vertex *current = POLY_at(poly, index);
    if (current != NULL)
    {
        I_drawSquare(img, current->position, 10);
    }
}

void POLY_drawSelectedEdge(Polygon *poly, size_t index, Image *img)
{
    Vertex *current = POLY_at(poly, index);
    if (current != NULL && current->next != NULL)
    {
        I_bresenham(img, current->position.x, current->position.y,
                    current->next->position.x, current->next->position.y);
    }
}

size_t POLY_closestVertex(Polygon *poly, Vec2 mousePos)
{
    if (poly->first != NULL)
    {
        // initialize values on first vertex
        Vertex *current = poly->first;
        size_t index = 0;
        size_t indexMin = 0;
        float distMin = V2_distanceSq(current->position, mousePos);

        // the run the loop over the next vertices
        current = current->next;
        index++;

        while (current != NULL && index < poly->size)
        {
            // compute de squared distance to the vertex
            float d = V2_distanceSq(current->position, mousePos);
            // is it closer ?
            if (d < distMin)
            {
                // save the distance and the index
                distMin = d;
                indexMin = index;
            }
            current = current->next;
            index++;
        }
        return indexMin;
    }
    return 0;
}

// exactly the same algo as above
size_t POLY_closestEdge(Polygon *poly, Vec2 mousePos)
{
    if (poly->first != NULL && poly->first->next != NULL)
    {
        Vertex *current = poly->first;
        size_t index = 0;
        size_t indexMin = 0;
        float distMin = distanceFromSegment(current->position, current->next->position, mousePos);

        current = current->next;
        index++;

        while (current->next != NULL && current != poly->first)
        {
            float d = distanceFromSegment(current->position, current->next->position, mousePos);
            if (d < distMin)
            {
                distMin = d;
                indexMin = index;
            }
            current = current->next;
            index++;
        }
        return indexMin;
    }
    return 0;
}

// high and low are "inverted" because the (0,0)
// point is in the top left corner of the screen
void POLY_findDimensions(Polygon *poly, int *high, int *low)
{
    Vertex *current = poly->first;
    if (current != NULL)
    {
        // initialize on first vertex
        *high = current->position.y;
        *low = current->position.y;

        // the run the loop
        current = current->next;
        while (current != poly->first && current != NULL)
        {
            // if find a lower y value (higher point)
            if (current->position.y < *high)
            {
                *high = current->position.y;
            }
            // if find a higher y value (lower point)
            if (current->position.y > *low)
            {
                *low = current->position.y;
            }
            current = current->next;
        }
    }
}

void POLY_fill(Polygon *poly, Image *img)
{
    int high, low;

    // find upper and lower boudaries to optimize scanning
    POLY_findDimensions(poly, &high, &low);
    //printf("high:%d\tlow:%d\n",high, low);
    for (size_t y = high; y < low; y++)
    {
        Vertex *current;
        current = poly->first;
        int T[64];
        int S[64];
        size_t size = 0;

        // for all segments in poly
        do
        {
            Vec2 A = {0, y};
            Vec2 B = {800, y};
            Vec2 I;
            if (intersectPoint(A, B, current->position, current->next->position, &I))
            {
                // put x coordinate of intersection in array
                T[size] = I.x;
                // put the sign of the corresponding edge in array
                S[size] = current->next->position.y - current->position.y;
                size++;
            }
            current = current->next;
        // check if not comming back to first edge or reach end of the polygon
        } while (current != poly->first && current->next != NULL && current != NULL);

        // sort the intersection array and perform the same permutations on the sign array
        sort(T, S, size);
        
        if (size >= 2)
        {
            for (size_t i = 0; i < size - 1; i += 2)
            {
                // if both x coordinates are equals (is result of polygon vertex intersection)
                if(T[i]==T[i+1])
                {
                    // if signs are equals, skip 1 vertex
                    if(S[i]*S[i+1]>0)
                    {
                        i++;
                    }
                }
                Color r = {0.3, 0.3, 0.3};
                I_changeColor(img, r);
                I_drawHLine(img, y, T[i], T[i + 1]);
            }
        }
    }
}