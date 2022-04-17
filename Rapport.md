# Rapport HANQUEZ Mael

## Réponses aux questions TASK_#.md:

Les réponses au tasks ce trouve dans le dossier rapport_tasks.
J'ai rédiger un .md pour chacune des tasks.

## Les choix d'implémentation:

- J'ai eu parfois du mal à gérer l'implémentation d'une nouvelle fonction car j'avais peur de "casser" le projet en bidouillant l'implémentation de base du projet.
- J'ai eu un problème plus tard car j'utilisais un unordered_set pour stocker ma liste d'avions, mais après avoir avancer un peu plus dans le sujet je me suis aperçu que il vallait mieux utiliser une list qu'un unordered_set car la fonction que j'avais besoin n'était pas disponible sur un unordered_set. 
- J'ai essayer de faire le maximum du projet, au détriment de la qualité du code. Je voulais pouvoir avancer donc j'ai fais des choix d'implémentation "basique" pour simplement pouvoir tester le code rapidement. Je pense que j'aurais pu rendre plus de fonction "privé" que "public" et pour évité d'avoir à modifier plus tard si besoin je l'es mettaient en public.

## Les situations bloquantent:

- Dans la task2 j'ai eu du mal sur la partie Objectif 2 D6, où j'ai mis du temps à comprendre que D3 n'avait pas bien été implémenté. Donc je n'avais pas accès à mon AircraftManager, j'utilisais un autre AircraftManager, qui n'avait lui pas d'avions dans sa liste. Après avoir vérifié toutes les fonctions et plusieurs segfaults, je me suis rendu compte qu'il suffisait de rajouter dans TowerSimulation dans la création de l'airport. Il fallait lui fournir par référence le aircraft_manager car il à l'ownership du manager et de la factory.

- J'ai encore actuellement un problème (task2) pour trié correctement les avions lorsqu'ils reservent un terminal. J'arrive a ordonner via le fuel mais je n'arrive pas a gérer quand j'ai plusieur paramétre de trie et en m'être un qui prédomine l'autre.

- Je n'ai pas regarder la task4 car je n'ai pas eu le temps de finir la 3. Mais je pense qu'elle m'aurait posée problème.

## Ce que j'ai aimé/détesté dans le projet: 

C'est la premier fois qu'on travaille sur un projet qui est de base fonctionnel. Ce qui est très intéressant je trouve car c'est sans doute ce qu'on va tous devoir faire dans la vie professionelle. Travailler sur des projets déjà existant et/ou s'approprier un projet qu'on a pas conçu de A à Z.
C'est aussi très frustant car j'ai tirailler entre le code déjà fonctionnel d'un prof, et mon code pour implémenter une nouvelle feature du projet. Le problème c'est que j'avais peur de modifier le code du prof qui est propre et fonctionnel et mon code qui ne compilait pas et quand il compilait ne marchait pas. 
Donc j'essaye plusieurs modification et si le code ne marche pas après 3-4 modification je pars du principe que le code que je voulais implémenter n'est pas du tout correct. Alors même quand j'étais dans la bonne piste je faisais marche arrière. Contrairement à un code qu'on écrit entièrement on va ce dire plus facilement qu'on avait oublié des cas où qu'on avait pas penser à cette éventualité et donc c'est moins difficile de changer beaucoup de ligne de code, d'enlever un const, de changer une fonction privé et la passer public.

J'ai aimé autant que j'ai détesté mon incapacité à faire un choix entre le code déjà existant et fonctionnel et mon code. 

## Ce que j'ai appris:

Je trouve ça très intéressant de partir d'un projet déjà opérationnel, et même dommage que ce soit seulement la premier fois que ça arrive.
Pouvoir être confronté à un code que je n'ai pas écrit et devoir me balader de fichier en fichier pour trouver une info. Je pense que c'est le projet qui resemblera le plus à ce qu'on va devoir faire dans une société donc très bien de le voir avant.
