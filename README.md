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
 
### Mode itératif

- 1) Création de la structure sockaddr_un ou sockaddr_in
- 2) Appel de la fonction socket
- 3) Appel de la fonction bind
- 4) Appel de la fonction listen

- 5) Boucle dans laquelle on effectue les traitements suivants :
- 5.1) Attente sur la fonction accept
- 5.2) Lorsque le serveur reçoit une demande de connexion, la fonction accept génère une socket fille (on récupère le descripteur de fichier)
- 5.3) on utilise le descripteur pour dialoguer avec le client (grâce à des read et des write, en utilisant le descripteur de la socket fille)
- 6) quand on a fini le dialogue, on ferme la socket fille et on retourne sur la fonction accept
 
### Mode non-iteratif

Dans le cas d'un serveur non itératif, il faut modifier le code situé dans la boucle :
- 5.1) quand le serveur reçoit une demande de connexion, ça débloque la fonction accept qui renvoie le descripteur de la socket fille
- 5.2) on crée un nouveau processus grâce à fork (processus lourd) ou pthread_create (processus léger)
- le processus père se remet en attente sur la fonction accept (il passe à l'itération suivante dans la boucle)
- 5.3) le processus fils dialogue avec le client, grâce à la socket fille
- le processus fils ferme la socket fille
- 6) le processus fils s'arrête
 

 

## Notes

- Les clients et les serveurs doivent pouvoir s'échanger des messages de taille quelconque (voir le TP sur les tubes)
- Il faut aussi gérer le CTRL-C au niveau du serveur afin de l'arrêter correctement (fermer la socket mère)

## Rendu 
Le sujet de ce mail sera "TP-ICR" suivi des noms de famille des personnes appartenant au groupe de travail.
Les groupes de travail sont constitués d'au plus 3 personnes.
