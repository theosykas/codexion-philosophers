🧑‍💻 Architecture Multithreading : Le Problème des Codeurs à la Table Ronde

Ce document est un guide de conception pour résoudre le problème de synchronisation de type "Dîner des Philosophes", adapté avec des codeurs et des dongles USB.

1. Topologie : La Table Ronde et les Dongles

Le concept principal est la circularité. Les codeurs sont assis en cercle, et il y a un dongle entre chaque codeur.
Pour qu'un codeur puisse compiler (ou "manger"), il doit acquérir deux dongles : celui à sa gauche et celui à sa droite.

La règle du Modulo (%) : C'est l'outil mathématique parfait pour créer un cercle infini dans un tableau linéaire.

Le codeur i possède le dongle gauche i.

Son dongle droit est le (i + 1) % nombre_de_codeurs.

Exemple : S'il y a 10 codeurs (de 0 à 9), le codeur 9 prendra le dongle 9 à sa gauche, et le dongle (9 + 1) % 10 = 0 à sa droite. La boucle est bouclée !

Structure de base (Exemple conceptuel) :
Chaque codeur devrait posséder sa propre structure contenant ses informations et des pointeurs vers les dongles (qui agiront comme des verrous/mutex).

```bash
typedef struct s_coder {
    int             id;
    pthread_mutex_t *left_dongle;
    pthread_mutex_t *right_dongle;
	 // ... autres variables d'état
} t_coder;
```
2. Les Règles d'Attribution (Scheduling)

Quand plusieurs threads (codeurs) veulent la même ressource (dongle), il faut une logique pour éviter que certains meurent de faim ("burn out").

EDF (Earliest Deadline First) : L'algorithme donne la priorité au codeur qui est le plus proche de son délai fatidique (celui qui va mourir / faire un burn out en premier). C'est une approche dynamique et très optimisée.

FIFO (First In, First Out) : Approche classique (Premier arrivé, premier servi). On pourrait imaginer une liste d'attente pour un dongle (Pos 0 pour le premier, Pos 1 pour le second).

Note de conception : Dans la plupart des implémentations basiques de ce projet, un simple ordre de prise des dongles (ex: toujours prendre le gauche puis le droit) couplé à un léger décalage temporel pour les codeurs pairs/impairs suffit à éviter les blocages mortels (deadlocks).

3. Mémoire et Mutex (La Gestion des Concurrences)

Un Mutex (Mutual Exclusion) est comme un "bâton de parole". Il permet de dire : "Attention, je modifie ou je lis cette variable sensible, les autres, vous attendez que j'ai terminé".

A. Ce qui DOIT être protégé par un Mutex :

Toute variable qui est modifiée pendant l'exécution du programme et qui peut être lue ou écrite par plusieurs threads en même temps.

Exemples : L'heure de la dernière compilation, le statut du codeur (en vie ou mort), le nombre de compilations terminées.

B. Ce qui n'a PAS BESOIN d'être protégé par un Mutex :

Toute variable constante après l'initialisation (Read-Only). Puisqu'elles ne changent jamais, aucun thread ne risque d'écraser la lecture d'un autre.

Exemples : L'ID du codeur, le nombre total de codeurs, le temps requis pour compiler, le temps requis pour dormir ("cool down").

4. Affichage Propre (Print to Output)

Lorsque plusieurs threads utilisent printf en même temps, les messages peuvent se mélanger ou s'entrecouper dans le terminal.
La Solution : Tu dois créer un Mutex d'affichage global.

Avant chaque printf, le codeur verrouille ce mutex. Une fois le message affiché, il le déverrouille. Ainsi, l'output se fait toujours d'une seule traite, ligne par ligne.

5. Cycle de Vie des Threads (pthread_join)

Chaque codeur est un thread indépendant qui exécute sa propre boucle (son propre programme) sans se soucier des autres.

Le fil principal (Main Process) : Si la fonction main de ton programme se termine, elle tue instantanément tous les threads enfants.

Maintenir en vie avec pthread_join : Cette fonction permet au processus principal de dire "Je mets mon exécution en pause et j'attends que ce thread spécifique ait terminé son travail". En faisant une boucle de join sur tous tes threads à la fin de ton main, tu garantis que le programme ne quittera pas tant que les codeurs n'ont pas tous terminé.

6. Débogage : fsanitize et les Data Races

En multithreading, le pire ennemi est la Data Race (Course aux données) : c'est quand deux threads accèdent à la même variable mémoire en même temps, et qu'au moins l'un d'eux tente de la modifier. Le comportement devient alors totalement imprévisible.

Comment s'en protéger ? En utilisant les bonnes pratiques de Mutex (voir point 3).

Comment les détecter ? Lors de la compilation, on utilise un outil spécifique de GCC/Clang appelé ThreadSanitizer.

Exemple de flag de compilation : gcc -Wall -Wextra -Werror -fsanitize=thread -g ton_fichier.c

Attention : N'utilise jamais -fsanitize=thread en même temps que -fsanitize=address ou Valgrind, ils sont incompatibles entre eux.