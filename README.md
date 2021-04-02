# projet-c
petit projet de C

## Objectif

L'objectif de ce TP est de créer différentes versions de serveurs concurrents qui utilisent :
Les processus lourds avec les sockets Unix
Les processus légers avec les sockets Unix
Les processus lourds avec les socket IPv4
Les processus légers avec les socket IPv4
En conséquence, il y a aussi deux types de clients : le client Unix et le client IPv4
 

## Etapes

En cours, on a présenté le serveur itératif, qui prend en charge les requêtes les unes après les autres. Les grandes étapes du programme serveur sont :
 

Création de la structure sockaddr_un ou sockaddr_in
Appel de la fonction socket
Appel de la fonction bind
Appel de la fonction listen
Boucle dans laquelle on effectue les traitements suivants :
Attente sur la fonction accept
Lorsque le serveur reçoit une demande de connexion, la fonction accept génère une socket fille (on récupère le descripteur de fichier)
on utilise le descripteur pour dialoguer avec le client (grâce à des read et des write, en utilisant le descripteur de la socket fille)
quand on a fini le dialogue, on ferme la socket fille et on retourne sur la fonction accept
 

Dans le cas d'un serveur itératif, il faut modifier le code situé dans la boucle :
 

quand le serveur reçoit une demande de connexion, ça débloque la fonction accept qui renvoie le descripteur de la socket fille
on crée un nouveau processus grâce à fork (processus lourd) ou pthread_create (processus léger)
le processus père se remet en attente sur la fonction accept (il passe à l'itération suivante dans la boucle)
le processus fils dialogue avec le client, grâce à la socket fille
le processus fils ferme la socket fille
le processus fils s'arrête
 

 

 

Les clients et les serveurs doivent pouvoir s'échanger des messages de taille quelconque (voir le TP sur les tubes)
 

Il faut aussi gérer le CTRL-C au niveau du serveur afin de l'arrêter correctement (fermer la socket mère)
 

 
 

Le sujet de ce mail sera "TP-ICR" suivi des noms de famille des personnes appartenant au groupe de travail.
 

Les groupes de travail sont constitués d'au plus 3 personnes.
