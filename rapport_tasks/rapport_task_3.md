# Rapport Task 3 : Assertions et exceptions

## Objectif 1 - Crash des avions

1) Je n'ai jamais eu de problème sur le crash d'un avion.
   

2) Introduisez un compteur qui est incrémenté chaque fois qu'un avion s'écrase. Choisissez une touche du clavier qui n'a pas encore été utilisée (m par exemple ?) et affichez ce nombre dans la console lorsque l'utilisateur appuie dessus.
   
La touche utiliser sera `l` car j'ai déjà utilisée la touche `m`.

```c++

void TowerSimulation::nb_of_crashs()
{
    std::cout << "Il y a eu: " << crash_cpt << " crash d'avion" << std::endl;
}

void TowerSimulation::increment_crash_cpt()
{
    crash_cpt++;
}
```

J'ai rajouter ça dans le fichier tower_sim.cpp ainsi qu'un crash_cpt dans TowerSimulation en privé.

Je voudrais pouvoir utiliser la fonction increment_crash_cpt() dans aircraft.cpp mais je ne suis pas sur que ce soit la bonne manière de faire. J'ai mis donc la fonction increment_crash_cpt en public et nb_of_crashs en privé.


