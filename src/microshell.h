/*!
  \file microshell.h
  \author Romane Sallio <sallioroma@eisti.eu>
  \version 1
  \date  29 / 03 / 2020
  \brief TP noté Programmation Système et réseau exercice 1
*/

#ifndef __MICROSHELL_H_
#define __MICROSHELL_H_

/* inclusion des entetes de librairies */
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* inclusion des variables globales */
#define TAILLE_CMD 150

/* inclusion des entetes de fonctions */
void affichageBienvenue();
void affichagePrompt(bool cmdUtilise, int signal, struct timespec start, struct timespec end);

#endif