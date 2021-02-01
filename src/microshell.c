/*!
  \file microshell.c
  \author Romane Sallio <sallioroma@eisti.eu>
  \version 1
  \date  29 / 03 / 2020
  \brief TP noté Programmation Système et réseau exercice 1
*/

#include "microshell.h"

void affichageBienvenue() {
    printf("Bienvenue dans le Micro Shell eistish\n");
    printf("Commandes :\n");
    printf("    exit : quitter\n");
    printf("\n");
}

void affichagePrompt(bool cmdUtilise, int signal, struct timespec start, struct timespec end) {
    double time;
    printf("eistish");
    // Si on a exécuté au moins une fois une commande
    if(cmdUtilise){
        // Alors on affiche le code de retour (ou signal) de la commande précédente dans le prompt
        printf(" [");
        if(signal == 0) {
            printf("exit");
        } else {
            printf("sign");
        }
        //Calcule le temps d'exécution de la commande précédente
        time = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_nsec - start.tv_nsec) / 1000000.0);
        //Affiche le code retour du signal et le temps d'exécution
        printf(":%d|%.2fms]", WEXITSTATUS(signal),time);
    }
    printf(" %% ");
}
