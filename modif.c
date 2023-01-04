/* il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>
#include <string.h>
#include <math.h>
// choisir la moyenne parmi ces 4pixels
// fonction moyenne
// fonction extreme
// fonction multiple

void enlever_pixels_0(Image *i)
{
  // Création d'un nouveau tableau de pixels
  GLubyte *pixels[i->sizeX * i->sizeY];
  int nb_pixels = 0;

  // Parcours tous les pixels de l'image
  for (int y = 0; y < i->sizeY; y++)
  {
    for (int x = 0; x < i->sizeX; x++)
    {
      GLubyte *pixel = i->data + 3 * (y * i->sizeX + x);
      // Si le pixel n'est pas de valeur 0, ajoute le pixel au nouveau tableau de pixels
      if (*pixel != 0)
      {
        pixels[nb_pixels++] = pixel;
      }
    }
  }

  // Alloue de la mémoire pour un nouveau tableau de données de l'image
  GLubyte *new_data = malloc(sizeof(GLubyte) * nb_pixels * 3);
  // Copie les pixels du nouveau tableau de pixels dans le nouveau tableau de données de l'image
  memcpy(new_data, pixels, sizeof(GLubyte) * nb_pixels * 3);
  // Libère l'ancien tableau de données de l'image
  free(i->data);

  // Remplace l'ancien tableau de données de l'image par le nouveau
  i->data = new_data;
  // Met à jour les champs sizeX et sizeY de l'image
  int racine = sqrt(nb_pixels);
  i->sizeX = racine;
  i->sizeY = racine;
}

int calculer_moyenne_bloc(Image *i, int j, int k, int n)
{
  int vr = 0, vg = 0, vb = 0;
  // Parcours des pixels du bloc
  for (int y = 0; y < n; y++)
  {
    for (int x = 0; x < n; x++)
    {
      GLubyte *imr = i->data + 3 * (j * i->sizeX + k + y * i->sizeX + x);
      GLubyte *img = imr + 1;
      GLubyte *imb = imr + 2;
      vr += (int)*imr;
      vg += (int)*img;
      vb += (int)*imb;
    }
  }
  // Calcul de la valeur moyenne
  int v = (vr + vg + vb) / (3 * n * n);
  return v;
}
int calculer_couleur_dominante_bloc(Image *i, int j, int k, int n)
{
  int compteur_rouge[256] = {0};
  int compteur_vert[256] = {0};
  int compteur_bleu[256] = {0};
  // Parcours des pixels du bloc
  for (int y = 0; y < n; y++)
  {
    for (int x = 0; x < n; x++)
    {
      GLubyte *imr = i->data + 3 * (j * i->sizeX + k + y * i->sizeX + x);
      GLubyte *img = imr + 1;
      GLubyte *imb = imr + 2;
      compteur_rouge[(int)*imr]++;
      compteur_vert[(int)*img]++;
      compteur_bleu[(int)*imb]++;
    }
  }
  // Recherche de la couleur la plus présente
  int couleur_dominante = 0;
  int compteur_max = 0;
  for (int c = 0; c < 256; c++)
  {
    if (compteur_rouge[c] > compteur_max)
    {
      couleur_dominante = c;
      compteur_max = compteur_rouge[c];
    }
    if (compteur_vert[c] > compteur_max)
    {
      couleur_dominante = c;
      compteur_max = compteur_vert[c];
    }
    if (compteur_bleu[c] > compteur_max)
    {
      couleur_dominante = c;
      compteur_max = compteur_bleu[c];
    }
  }
  printf("couleur dominante %d", couleur_dominante);
  return couleur_dominante;
}
int calculer_couleur_extreme_bloc(Image *i, int j, int k, int n)
{
  int somme_rouge = 0;
  int somme_vert = 0;
  int somme_bleu = 0;
  int nb_pixels = n * n;
  // Parcours des pixels du bloc
  for (int y = 0; y < n; y++)
  {
    for (int x = 0; x < n; x++)
    {
      GLubyte *imr = i->data + 3 * (j * i->sizeX + k + y * i->sizeX + x);
      GLubyte *img = imr + 1;
      GLubyte *imb = imr + 2;
      somme_rouge += (int)*imr;
      somme_vert += (int)*img;
      somme_bleu += (int)*imb;
    }
  }
  // Calcul des couleurs moyennes
  int moyenne_rouge = somme_rouge / nb_pixels;
  int moyenne_vert = somme_vert / nb_pixels;
  int moyenne_bleu = somme_bleu / nb_pixels;
  // Recherche de la couleur la plus extrême
  int couleur_extreme = 0;
  int distance_max = 0;
  for (int y = 0; y < n; y++)
  {
    for (int x = 0; x < n; x++)
    {
      GLubyte *imr = i->data + 3 * (j * i->sizeX + k + y * i->sizeX + x);
      GLubyte *img = imr + 1;
      GLubyte *imb = imr + 2;
      int rouge = (int)*imr;
      int vert = (int)*img;
      int bleu = (int)*imb;
      int distance = abs(rouge - moyenne_rouge) + abs(vert - moyenne_vert) + abs(bleu - moyenne_bleu);
      if (distance > distance_max)
      {
        couleur_extreme = ((int)*imr << 16) + ((int)*img << 8) + (int)*imb;
        distance_max = distance;
      }
    }
  }
  return couleur_extreme;
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
      if ((y == 0 || y == n - 1) && (x == 0 || x == n - 1))
      {
        // Garde seulement les 4 coins de la fenêtre
        int x2 = k * n + x;
        int y2 = j * n + y;
        if (x2 >= 0 && x2 < i->sizeX && y2 >= 0 && y2 < i->sizeY)
        {
          GLubyte *imr = i->data + 3 * (y2 * i->sizeX + x2);
          GLubyte *img = imr + 1;
          GLubyte *imb = imr + 2;
          *imr = val;
          *img = val;
          *imb = val;
        }
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
void compression(Image *i, int n)
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
  // enlever_pixels_0(i);
}
int nombre_pixels_differents(Image *i1, Image *i2)
{
  int nb_pixels_differents = 0;

  // Vérifie que les deux images ont la même taille
  if (i1->sizeX == i2->sizeX && i1->sizeY == i2->sizeY)
  {
    // Parcours des pixels des deux images
    for (int y = 0; y < i1->sizeY; y++)
    {
      for (int x = 0; x < i1->sizeX; x++)
      {
        GLubyte *p1 = i1->data + 3 * (y * i1->sizeX + x);
        GLubyte *p2 = i2->data + 3 * (y * i2->sizeX + x);
        if (*p1 != *p2 || *(p1 + 1) != *(p2 + 1) || *(p1 + 2) != *(p2 + 2))
        {
          // Incrémentation du compteur si les valeurs des pixels sont différentes
          nb_pixels_differents++;
        }
      }
    }
  }

  return nb_pixels_differents;
}

void décompression(Image *i)
{
  for (int y = 0; y < i->sizeY; y++)
  {
    for (int x = 0; x < i->sizeX; x++)
    {
      if (x > 0)
      {
        GLubyte *pixel = i->data + 3 * (y * i->sizeX + x);
        if (*pixel == 0)
        {
          GLubyte *voisin = pixel - 3;
          *pixel = *voisin;
        }
      }
    }
  }
}
