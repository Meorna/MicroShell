/*!
  \file main.c
  \author Romane Sallio <sallioroma@eisti.eu>
  \version 1
  \date  29 / 03 / 2020
  \brief TP noté Programmation Système et réseau exercice 1
*/

#include "microshell.h"

int main(int argc, char const *argv[]){
    bool cmdUtilise = false;
    char cmd[TAILLE_CMD];
    char *fichierEntree;
    char *fichierSortie;
    int compteurNombreArgument;
    int i;
    int signal;
    pid_t m_pid;   
    struct sigaction action; 
    struct timespec start;
    struct timespec end;

    // Permet d'ignorer l'action faite par le signal SIGTERM
    action.sa_handler = SIG_IGN;
    sigaction(SIGTERM, &action, NULL);

    affichageBienvenue();
    
    // Permet le retour au prompt eistish % tant que l'utilisateur n'a pas rentré la commande exit
    while (1){
        affichagePrompt(cmdUtilise, signal, start, end);
        //Lecture de la commande simple (sans argument) saisie par l'utilisateur
        //scanf("%s", cmd);
        //Lecture d'une commande complexe (avec arguments) saisie par l'utilisateur
        fgets(cmd, TAILLE_CMD, stdin);
        //Récupère le temps de l'horloge CPU pour processus au début de l'exécution
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        // Supprimer le retour charriot (avant-dernier caractère de cmd) conservé par fgets
        cmd[strlen(cmd) - 1] = '\0';
        // Si la commande saisie est exit
        if(strcmp("exit", cmd) == 0){
            // Alors on quitte le programme
            exit(0);
        }
        // Exécution d'une commande simple (sans argument)
        //system(cmd);
        // On met le compteur à 0
        compteurNombreArgument = 0;
        // On parcourt la chaine de caractère corrsepondant à la commande de l'utilisateur
        for (i = 0; i < strlen(cmd); i++){
            //A chaque espace que l'on trouve on incrémente le compteur
            if(cmd[i] == ' ') {
                ++compteurNombreArgument;
            }
        }
        // On créer un tableau de chaines de caractères pour accueillir les différents arguments
        char *tabArguments[compteurNombreArgument + 2];
        //Permet d'extraire le premier élément syntaxique de la commande
        char *elt = strtok(cmd, " ");
        //On se place au début du tableau
        i = 0;
        // On parcourt élément tant qu'il n'est pas vide
        while (elt != NULL){
            //On met l'élément syntaxique dans le tableau
            tabArguments[i] = strdup(elt);
            // Permet de passer à l'élément suivant du tableau
            i++;
            // Permet d'extraire l'élément syntaxique suivant
            elt = strtok(NULL, " ");
        }
        // La dernière case du tableau prend la valeur NULL
        tabArguments[i] = NULL;
        // On vide la variable fichierEntree des potentiels valeurs précédentes
        fichierEntree = NULL;
        i = 0;
        //On parcourt les différents arguments de la commande de l'utilisateur
        while (tabArguments[i] != NULL){
            // Si la commande contient <
            if(strcmp(tabArguments[i], "<") == 0){
                // Alors l'argument suivant correspond on nom du fichier d'entrée
                fichierEntree = tabArguments[i + 1];
                // On supprime l'argument < et l'argument suivant du tableau
                free(tabArguments[i]);
                while (tabArguments[i + 2] != NULL){
                    tabArguments[i] = tabArguments[i + 2];
                    ++i;
                }
                tabArguments[i] = NULL;
            }
            ++i;
        }
        // On vide la variable fichierSortie des potentiels valeurs précédentes
        fichierSortie = NULL;
        i = 0;
        //On parcourt les différents arguments de la commande de l'utilisateur
        while (tabArguments[i] != NULL){
            // Si la commande contient >
            if(strcmp(tabArguments[i], ">") == 0){
                // Alors l'argument suivant correspond on nom du fichier d'entrée
                fichierSortie = tabArguments[i + 1];
                // On supprime l'argument > et l'argument suivant du tableau
                free(tabArguments[i]);
                while (tabArguments[i + 2] != NULL){
                    tabArguments[i] = tabArguments[i + 2];
                    ++i;
                }
                tabArguments[i] = NULL;
            }
            ++i;
        }    
        //On créé un processus fils
        m_pid = fork();
        switch(m_pid){
            //Erreur de la fonction fork
            case -1 :
                printf("Erreur: echec de fork \n");
            // Numéro du processus fils
            case 0 :
                //Si l'utilisateur souhaite lire un fichier
                if (fichierEntree != NULL){
                    //Ouverture du fichier donné par l'utilisateur 
                    //Si l'ouverture du fichier échoue
                    if(freopen(fichierEntree, "r", stdin) == NULL){
                        //Alors on envoie un signal de terminaison au processus père
                        if (kill(getppid(), SIGTERM) == -1){
                            printf("Erreur SIGTERM\n");
                        }
                        //On met fin au processus en spécifiant le signal de terminaison SIGTERM
                        exit(SIGTERM);
                    }
                }
                //Si l'utilisateur souhaite écrire dans un fichier
                if (fichierSortie != NULL){
                    //Ouverture du fichier donné par l'utilisateur
                    //Si l'ouverture du fichier échoue
                    if(freopen(fichierSortie, "w", stdout) == NULL){
                        // On envoie un signal de terminaison au processus père
                        if (kill(getppid(), SIGTERM) == -1){
                            printf("Erreur SIGTERM\n");
                        }
                        //On met fin au processus en spécifiant le signal de terminaison SIGTERM
                        exit(SIGTERM);
                    }
                }
                
                //Permet un appel système c'est à dire l'exécution d'une commande ainsi que la gestion d'erreur
                if (execvp(tabArguments[0], tabArguments) == -1){
                    // On envoie un signal de terminaison au processus père
                    if (kill(getppid(), SIGTERM) == -1){
                        printf("Erreur SIGTERM\n");
                    }
                    //On met fin au processus en spécifiant le signal de terminaison SIGTERM
                    exit(SIGTERM);
                }
                //On met fin au processus en spécifiant qui le processus fils c'est bien déroulé
                exit(0);
            // Numéro du processus père    
            default :
                // Attend la fin du fils
                wait(&signal);
                
        }
        //On libère la mémoire allouée
        i = 0;
        while(tabArguments[i] != NULL) {
            free(tabArguments[i]);
            i++;
        }
        free(elt);
        // Indique qu'on a exécuté au moins une fois une commande
        cmdUtilise = true;
        //Récupère le temps de l'horloge CPU pour processus à la fin de l'exécution
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        
    }   
    return 0;
}
