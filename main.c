
/*===============================================================*\

    Vendredi 25 septembre 2013

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <time.h>

#include "Image.h"
#include "polygon.h"
#include "utils.h"

Image *img;

// save the program current mode
enum Mode
{
	INSERT,
	VERTEX,
	EDGE
};
enum Mode prgmMode;

Polygon *poly;

int fill = 0;

// index of the currently selected vertex
size_t selected_vertex;

Color color_default = {1, 1, 1};

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Color black = {0, 0, 0};
	Color red = {1, 0, 0};
	Color fill_color = {0.4, 0.4, 0.5};

	// clear the screen with background color (black)
	I_fill(img, black);

	// if the polygon is to be filled
	if (fill)
	{
		if (poly->size > 1)
		{
			I_changeColor(img, fill_color);
			POLY_fill(poly, img);
			I_changeColor(img, color_default);
		}
	}

	// draw the polygon
	POLY_draw(poly, 0, img);

	// if "vertex selection" mode
	if (prgmMode == VERTEX)
	{
		if (poly->size > 0)
		{
			// change the color to red and draw the selection square
			I_changeColor(img, red);
			POLY_drawSelectedVertex(poly, selected_vertex, img);
			I_changeColor(img, color_default);
		}
	}
	// if "edge selection" mode
	else if (prgmMode == EDGE)
	{
		if (poly->size > 1)
		{
			// draw the selected line in red
			I_changeColor(img, red);
			POLY_drawSelectedEdge(poly, selected_vertex, img);
			I_changeColor(img, color_default);
		}
	}

	// display the final image on screen
	I_draw(img);

	glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int state, int x, int y)
{
	// mouse position vector
	Vec2 pos = {x, y};

	// LEFT MOUSE BUTTON - vertex insertion
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		if (prgmMode == INSERT)
		{
			POLY_appendVertex(poly, pos);
		}
	}
	// MOUSE MIDDLE BUTTON - edge subdivision
	if ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN))
	{
		if (prgmMode == EDGE)
		{
			Vertex *first = POLY_at(poly, selected_vertex);
			if (first->next != NULL)
			{
				Vertex *second = first->next;
				Vec2 new = V2_divFac(V2_add(second->position, first->position), 2.0);
				POLY_insertVertex(poly, new, selected_vertex);
				printf("new vertex at (%f,%f)\n", new.x, new.y);
			}
		}
	}
	// MOUSE RIGHT BUTTON - selection
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		if (prgmMode == VERTEX)
		{
			selected_vertex = POLY_closestVertex(poly, pos);
		}
		else if (prgmMode == EDGE)
		{
			selected_vertex = POLY_closestEdge(poly, pos);
		}
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	// fprintf(stderr,"key=%d\n",key);
	switch (key)
	{
	case 27:
		exit(1);
	case 'i': //insert
		printf("insert mode\n");
		prgmMode = INSERT;
		break;
	case 'c': // open/close
		printf("%s poly\n", poly->type == OPENED ? "close" : "open");
		POLY_openClose(poly);
		break;
	case 'v': // vertex mode
		printf("vertex mode\n");
		prgmMode = VERTEX;
		break;
	case 'e': // edge mode
		printf("edge mode\n");
		prgmMode = EDGE;
		break;
	case 'f': // fill poly
		printf("fill polygon\n");
		fill = (fill + 1) % 2;
		break;
	case 's': // delete vertex
		if (poly->size > 0 && prgmMode == VERTEX)
		{
			printf("remove vertex\n");
			POLY_removeVertex(poly, selected_vertex);
			if (selected_vertex > 0)
			{
				selected_vertex--;
			}
			else
			{
				selected_vertex = 0;
			}
		}
		break;
	default:
		fprintf(stderr, "keyboard_CB : %d : unknown key.\n", key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche spéciale (F1, F2 ... F12, home, end, insert,
// haut, bas, droite, gauche etc).
//------------------------------------------------------------------

void special_CB(int key, int x, int y)
{

	int d = 10;

	switch (key)
	{
	case GLUT_KEY_PAGE_UP: // select next vertex
		if (selected_vertex >= poly->size - 1)
		{
			selected_vertex = 0;
		}
		else
		{
			selected_vertex++;
		}
		break;
	case GLUT_KEY_PAGE_DOWN: // select previous vertex
		if (selected_vertex <= 0)
		{
			selected_vertex = poly->size - 1;
		}
		else
		{
			selected_vertex--;
		}
		break;
	case GLUT_KEY_UP: // move vertex upward
		if (prgmMode == VERTEX)
		{
			POLY_at(poly, selected_vertex)->position.y--;
		}
		else if (prgmMode == EDGE)
		{
			Vertex *a = POLY_at(poly, selected_vertex);
			Vertex *b = a->next;
			if (b != NULL)
			{
				a->position.y--;
				b->position.y--;
			}
		}
		break;
	case GLUT_KEY_DOWN: // move vertex downward
		if (prgmMode == VERTEX)
		{
			POLY_at(poly, selected_vertex)->position.y++;
		}
		else if (prgmMode == EDGE)
		{
			Vertex *a = POLY_at(poly, selected_vertex);
			Vertex *b = a->next;
			if (b != NULL)
			{
				a->position.y++;
				b->position.y++;
			}
		}
		break;
	case GLUT_KEY_LEFT: // move vertex to the left
		if (prgmMode == VERTEX)
		{
			POLY_at(poly, selected_vertex)->position.x--;
		}
		else if (prgmMode == EDGE)
		{
			Vertex *a = POLY_at(poly, selected_vertex);
			Vertex *b = a->next;
			if (b != NULL)
			{
				a->position.x--;
				b->position.x--;
			}
		}
		break;
	case GLUT_KEY_RIGHT: // move vertex to the right
		if (prgmMode == VERTEX)
		{
			POLY_at(poly, selected_vertex)->position.x++;
		}
		else if (prgmMode == EDGE)
		{
			Vertex *a = POLY_at(poly, selected_vertex);
			Vertex *b = a->next;
			if (b != NULL)
			{
				a->position.x++;
				b->position.x++;
			}
		}
		break;
	default:
		fprintf(stderr, "special_CB : %d : unknown key.\n", key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	srand(time(NULL));
	if ((argc != 3) && (argc != 2))
	{
		fprintf(stderr, "\n\nUsage \t: %s <width> <height>\nou", argv[0]);
		fprintf(stderr, "\t: %s <ppmfilename> \n\n", argv[0]);
		exit(1);
	}
	else
	{
		int largeur, hauteur;
		if (argc == 2)
		{
			img = I_read(argv[1]);
			largeur = img->_width;
			hauteur = img->_height;
		}
		else
		{
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			img = I_new(largeur, hauteur);
			Color noir = C_new(0, 0, 0);
			I_fill(img, noir);
		}
		prgmMode = INSERT;
		selected_vertex = 0;
		poly = POLY_init();
		fill = 0;

		int windowPosX = 100, windowPosY = 100;

		glutInitWindowSize(largeur, hauteur);
		glutInitWindowPosition(windowPosX, windowPosY);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
		glutInit(&argc, argv);
		glutCreateWindow(argv[0]);

		glViewport(0, 0, largeur, hauteur);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glOrtho(0, largeur, hauteur, 0, -1, 1);

		glutDisplayFunc(display_CB);
		glutKeyboardFunc(keyboard_CB);
		glutSpecialFunc(special_CB);
		glutMouseFunc(mouse_CB);
		// glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();

		return 0;
	}
}
