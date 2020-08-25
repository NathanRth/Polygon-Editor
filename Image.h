/*====================================================*\
  Vendredi 8 novembre 2013
  Arash HABIBI
  Image.h
\*====================================================*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "Ppm.h"

#include "vec2.h"

//-----------------------------------------------------

typedef struct
{
	float _red, _green, _blue;
} Color;

Color C_new(float red, float green, float blue);
void C_check(Color c, char *message);

//-----------------------------------------------------

typedef struct
{
	int _width, _height;
	int _xzoom, _yzoom;
	int _xoffset, _yoffset;
	double _zoom;
	Color _current_color;
	Color **_buffer;
} Image;

// cree une image noire de taille width x height
Image* I_new(int _width, int _height);
// cree une image à partir du fichier imagefilename
Image* I_read(char *ppmfilename);

// A partir d’une image allouee, cette fonction colorie tous les pixels en c
void I_fill       (Image *img, Color c);
// Sur une image allouee, cette fonction dessine un damier avec des cases de
// largeur step pixels et de couleurs c l’autre couleur étant la couleur
// d’origine (par défaut noir)
void I_checker    (Image *img, Color c1, Color c2, int step);

// Fait de c la couleur courante
void I_changeColor(Image *img, Color c);
// Colorie le pixel (x,y) de la couleur courante
void I_plot       (Image *img, int x, int y);
// Colorie le pixel (x,y) de la couleur c sans changer la couleur courante
void I_plotColor  (Image *img, int x, int y, Color c);

void I_focusPoint (Image *img, int x, int y);
void I_zoomInit   (Image *img);
void I_zoom       (Image *img, double zoom_coef);
void I_move       (Image *img, int x, int y);

// Affiche l’image img à l’écran
void I_draw       (Image *img);

// -------------------------------------------------------

void I_drawSquare (Image *img, Vec2 pos, size_t size);
void I_drawHLine  (Image *img, int y, int minX, int maxX);
//void I_bresenhamOrigin(Image* img, int x, int y);

// draw a strainght line with the bresenham algorithm
void I_bresenham(Image* img, int xA, int yA, int xB, int yB);

void z2_to1erOct(int xA, int yA, int xB, int yB,int* xA_1O,int* yA_1O,int* xB_1O,int* yB_1O);
void premierOct_to_Z2(int xA,int yA,int xB,int yB, int x_10,int y_10,int *x,int *y);


//void I_remplissage4(Image* img_in, Image* img_out, int x_germe, int y_germe);
//void germePileExplicite(int germe_x, int germe_y, couleur coul_cible, couleur coul_nouv);

#endif



