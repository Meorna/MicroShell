# Micro shell eistish en C
## Objectif:
Créez un micro shell, que vous appellerez eistish, qui doit servir à lancer des commandes et à afficher les codes de sortie et les temps sur leur exécution.
Les fonctionnalités suivantes sont demandées : (l’utilisation des fonctions proposées est facultative)
1. Exécution de la commande saisie et retour au prompt :
a) Lecture de la commande saisie,
b) Exécution d’une commande simple (sans argument),
c) Retour au prompt eistish % et attente de la commande suivante.
2. Affichage du code de retour (ou du signal) de la commande précédente dans le prompt:
Exemple :
eistish % un_programme
eistish [exit:0] % un_autre_programme
eistish [sign:9] %
3. Mesure du temps d’exécution de la commande (clock_gettime):
Exemple :
eistish % un_programme
eistish [exit:0|10ms] % un_autre_programme
eistish [sign:9|5ms] %
4. Exécution d’une commande complexe (avec arguments) (strtok)
Exemple :
eisti % hostname -i
10.10.2.245
5. Gestion des redirections vers stdin et stdout avec ‘<’ et ‘>’ (dup ou dup2)
Exemple :
eistish % ls > filelist.txt
eistish [exit:0|1ms] % wc -l < filelist.txt 44
eistish [exit:0|4ms] %


## Instructions pour compiler et exécuter le code et utiliser le makefile : 
    Compiler : make
    Exécuter : ./exe
    Nettoyer : make clean
    Créer archive : make give

## Fonctionnement du code :
### main.c:
`int main(int argc, char const *argv[])` : Pour commencer, on ignore l'action faite par le signal SIGTERM pendant toute l'exécution du programme, on appelle la fonction affichageBienvenue, On fait une boucle infinie incluant le reste du programme, on appelle la fonction affichagePrompt, on récupère la commande saisie par l'utilisateur, on récupère l'heure qu'on stocke dans start,  vérifier si la commande saisie est exit alors on quitte le programme, on insère dans un tableaux tous les arguments de la commande de l'utilisateur, si l'utilisateur veut lire ou écrire dans un fichier alors on récupère le nom du fichier, on créé un processus fils en verifiant les erreurs.
Le processus fils va ouvrir les fichiers si donnés par l'utilisateur en écriture ou lecture selon le besoins, s'il y a une erreur lors de l'ouverture alors un signal de terminaison est envoyé au père et on quitte le processus fils, sinon on exécute la commande de l'utilisateur, si l'exécution échoue au envoie un signal de terminaison au processus père, si tout se passe bien on quitte le processus avec un signal succès.
Le processus père va attendre la fin du processus du fils et récupérer le signal renvoyé.
Pour terminer, on libère la mémoire allouée pour les pointeurs et on récupère l'heure de la fin de l'exécution.
        `sigaction(signal, &action, NULL)` : ignorer l'action d'un signal.
        `fgets(texte, taille, stdin)` : lecture d'une saisie utilisateur.
        `clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start)` : Récupère le temps de l'horloge CPU pour processus.
        `strlen(texte)` : Récupèrer la taille d'un texte.
        `strcmp(texte1, texte2)` : Comparer deux chaînes de caractères.
        `exit(signal)` : Terminer un processus.
        `strtok(texte, separateur)` : Extraire un à un les éléments syntaxique d'une chaine de caractères.
        `strdup(mot)` : Renvoyer un pointeur sur une nouvelle chaine de caractère.
        `free(tab)` : Libérer la mémoire.
        `fork()` : Créer un processus fils.
        `printf(texte`) : Ecrire un message dans la console.
        `freopen(fichier, modeOuverture, action)` : Ouvrir un fichier.
        `kill(id, signal)` : Envoyer un signal à un processus.
        `execvp(action, arguments)` ; Faire un apelle système.
        `wait(&status)` : Attendre la fin d'un processus fils.
###microshell.c :
`void affichageBienvenue()` : Cette fonction permet d'afficher un message à l'utilisateur l'informant qu'il se trouve dans le Micro Shell eistish
et la commande pour en sortir proprement.
    `printf(texte)` : Ecrire un message dans la console.
`void affichagePrompt(bool cmdUtilise, int signal, struct timespec start, struct timespec end)` : Cette fonction permet d'afficher le prompt, à chaque fois qu'une commande est faite par l'utilisateur (sauf exit qui quitte le programme) on calcule le temps d'exécution puis on affiche le code de retour (ou signal) de la commande précédente ainsi que son temps d'exécution.
    `printf(texte)` : Ecrire un message dans la console.
    `WEXITSTATUS(signal)` : Renvoie le code de sortie du fils (exit(signal)) grâce au wait(&signal).

## Décision de design :
Pour cet exercie, j'ai suivi à la lettre l'affichage que le professeur nous as donné dans l'énoncé de l'exercice. Pour la question 1 et 4 j'ai utilisé de nombreuses fonctions permettant de manipuler les chaînes de caractères dont strtok qui était conseillé dans l'énoncé, je me suis également servi du Chapitre 1 vu en cours sur les Multiprocessing.
Pour la question 2, je me suis servie du chapitre 2 sur les signaux vu en cours. Ensuite, pour la question 3, je me suis servie de la fonction donnée dans l'énoncé. Pour la question 5, je suis allée me renseigner sur le manuel des librairies.

## Résultat obtenu :
Bienvenue dans le Micro Shell eistish
Commandes :
    exit : quitter

eistish % ls
bin  doc  exe  filelist.txt  Makefile  src
eistish [exit:0|0.10ms] % toto
eistish [sign:15|0.12ms] % hostname -i
127.0.1.1
eistish [exit:0|0.09ms] % ls > filelist.txt
eistish [exit:0|0.10ms] % wc -l < filelist.txt
6
eistish [exit:0|0.10ms] % exit

## Section de références :
https://arel.eisti.fr/documents/68085/download/01-Multiprocessing.pdf
https://arel.eisti.fr/documents/68155/download/02-Signaux.pdf
https://arel.eisti.fr/documents/51966
http://manpagesfr.free.fr/consulter.html
https://koor.fr/C/Index.wp
