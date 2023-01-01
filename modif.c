/* il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>
// prendre en grid un nombre n (ex20) check
// choisir 4 pixel dans cet fenetre
// choisir la moyenne parmi ces 4pixels
// fonction moyenne
// fonction extreme
// fonction multiple

void window(Image *i){

};

int calculer_moyenne_bloc(Image *i, int j, int k, int n)
{
  int v = 0;
  // Parcours des pixels du bloc
  for (int y = 0; y < n; y++)
  {
    for (int x = 0; x < n; x++)
    {
      GLubyte *imr = i->data + 3 * (j * i->sizeX + k + y * i->sizeX + x);
      GLubyte *img = imr + 1;
      GLubyte *imb = imr + 2;
      v += (int)*imr + 3 * (int)*img + 2 * (int)*imb;
    }
  }
  // Calcul de la valeur moyenne
  v /= 6 * n * n;
  return v;
}

/**
 * Met à jour le groupe de quatre pixels de chaque fenêtre de taille nxn de l'image en utilisant la valeur val.
 * Les autres pixels du bloc sont mis à jour avec la valeur 0.
 *
 * Paramètres :
i – Pointeur sur l'image à mettre à jour.
j – Indice de ligne du début du bloc.
k – Indice de colonne du début du bloc.
n – Taille du côté du bloc (nxn).
val – Valeur à utiliser pour mettre à jour les pixels sélectionnés.
 */
void update_bloc(Image *i, int j, int k, int n, GLubyte val)
{
  // Parcours des pixels du bloc
  for (int y = 0; y < n; y++)
  {
    for (int x = 0; x < n; x++)
    {
      if (y % 2 == 0 && x % 2 == 0)
      {
        // Garde seulement toutes les lignes et colonnes paires de pixels
        GLubyte *imr = i->data + 3 * (j * i->sizeX + k + y * i->sizeX + x);
        GLubyte *img = imr + 1;
        GLubyte *imb = imr + 2;
        *imr = val;
        *img = val;
        *imb = val;
      }
      else
      {
        // Mise à jour des autres pixels avec la valeur 0
        GLubyte *imr = i->data + 3 * (j * i->sizeX + k + y * i->sizeX + x);
        GLubyte *img = imr + 1;
        GLubyte *imb = imr + 2;
        *imr = 0;
        *img = 0;
        *imb = 0;
      }
    }
  }
}
/**
 *
 * Les autres pixels du bloc sont mis à jour avec la valeur 0.
 *
 * Paramètres :
i – Pointeur sur l'image.
j – Indice de ligne du début du bloc.
k – Indice de colonne du début du bloc.
n – Taille du côté du bloc (nxn).
val – Valeur à utiliser pour mettre à jour les pixels sélectionnés.
 */
void gris_uniforme(Image *i, int n)
{
  // Parcours des blocs de n*n pixels de l'image
  for (int j = 0; j < i->sizeY - n + 1; j += n)
  {
    for (int k = 0; k < i->sizeX - n + 1; k += n)
    {
      // Calcul de la valeur moyenne du bloc
      int v = calculer_moyenne_bloc(i, j, k, n);
      GLubyte val = (GLubyte)v;
      // Mise à jour des pixels du bloc avec la valeur moyenne
      update_bloc(i, j, k, n, val);
    }
  }
}

void imageTab(Image *i)
{
  GLubyte **tab = malloc(sizeof(GLubyte *));
  int size = i->sizeY * i->sizeX;
  int sizeX = i->sizeX;
  int sizeY = i->sizeY;
  GLubyte *im = i->data;
  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      tab[i][j] = *im++;
      printf("%d\n", tab[i][j]);
    }
  }
}