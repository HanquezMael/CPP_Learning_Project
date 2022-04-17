# Rapport Task 1 - Gestion mémoire

## Analyse de la gestion des avions
```Si à un moment quelconque du programme, vous souhaitiez accéder à l'avion ayant le numéro de vol "AF1250", que devriez-vous faire ?```
On parcour la display_queue. Donc ce n'est pas optimal.

## Objectif 1 - Référencement des avions

### A - Choisir l'architecture

### B - Déterminer le propriétaire de chaque avion
```Qui est responsable de détruire les avions du programme ? (si vous ne trouvez pas, faites/continuez la question 4 dans TASK_0)```
la fonction timer dans opengl_interface.cpp
```Quelles autres structures contiennent une référence sur un avion au moment où il doit être détruit ?```
displayable
```Comment fait-on pour supprimer la référence sur un avion qui va être détruit dans ces structures ?```
on dois utiliser le destructeur.

```Pourquoi n'est-il pas très judicieux d'essayer d'appliquer la même chose pour votre AircraftManager ?```
?

### C - C'est parti !

Ajoutez un attribut aircrafts dans le gestionnaire d'avions. Choisissez un type qui met bien en avant le fait que AircraftManager est propriétaire des avions.

```
std::unordered_map<std::string, Aircraft> aircrafts ou bien 
std::unordered_map<std::string, std::unique_ptr<Aircraft>> aircrafts
std::unordered_set<Aircraft> (pas hashable) 
std::set<Aircraft> (Aircraft sont pas comparable)
std::unordered_set<std::unique_ptr<Aircraft>>
std::set<std::unique_ptr<Aircraft>> aussi possible
new ---> make_unique
delete --> ne plus necessaire
```

