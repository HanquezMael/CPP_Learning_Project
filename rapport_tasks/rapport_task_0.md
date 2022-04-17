# Rapport 
## Task 0

### A- Exécution

```Sur quelle touche faut-il appuyer pour ajouter un avion ? Comment faire pour quitter le programme ? A quoi sert la touche 'F' ?```

- Il faut utiliser la touche 'c' pour ajouter un avion
- on peux appuyer sur la touche 'q', 'x'
- La touche 'f' sert a mettre en full screen 

```Ajoutez un avion à la simulation et attendez. Que est le comportement de l'avion ? Quelles informations s'affichent dans la console ?```
L'avion arrive, il ce pose s'arrete au terminal, quand il a terminer il repart.
Exemple:
``` 
DL2094 is now landing...
now servicing DL2094...
done servicing DL2094
DL2094 lift off
```

```Ajoutez maintenant quatre avions d'un coup dans la simulation. Que fait chacun des avions ?```

Quand il y a + de 3 avions qui sont dans a terre alors, le 4 ème reste en l'air et tourne autour en attendant que l'un des 3 avion reparte.
La capacité maximal de l'aeroport est donc de 3 avions simultanés

### B- Analyse du code

```Listez les classes du programme à la racine du dossier src/. Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.```



<u>**class Aircraft :**</u>
Elle représente l'objet Avion, et donc les comportement qu'il peut avoir et ses caractéristiques.

<u>**class AirportType :**</u>
Elle représente un terminal, sa position et les chemins que doivent emprunter pour rejoindre la porte du terminal correspondant.

<u>**class Airport :**</u>
Elle représent l'aeroport donc tous les terminales.

<u>**class Terminal :**</u>
Elle représente l'endroit (une position) où l'avion doit charger/decharger les gens ou la marchandise


<u>**class TowerSimulation :**</u>
Permet de crée la simulation visuel du comportement de l'aeroport.

<u>**class Tower :**</u>
Elle représente la tour de contrôle qui informe les avions d'un terminal libre. Elle permet de mettre en attentent les autres avions dans une waiting queue en gros.


<u>**class Waypoint :**</u>
Représentation de l'emplacement des avions. Permet de savoir si un avion ce trouve au sol, à un terminal ou dans les airs.

---

```Pour les classes Tower, Aircaft, Airport et Terminal, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent. Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.```

<u>**class Tower :**</u>
```c++
public:
    // Le constructeur pour cree une tour de contrôle
    Tower(Airport& airport_) : airport { airport_ } {}


    // produce instructions for aircraft

    // Récupere la disponibilité d'un terminal, si un terminal est dispo l'avions qui demande en premier un terminal va le reserver, si le terminal n'est pas disponible alors il refait un tour (l'avions fait un tour complet avant de redemander si il peut atterrir).
    WaypointQueue get_instructions(Aircraft& aircraft);

    // Permet de s'assurer que l'avion est bien dans le terminal. Une fois qu'il est bien arrivé on peut lui demander de faire le service auquel il est rattacher. Par exemple : (Decharger de la marchandise, faire embarquer ou débarquer des personnes).
    void arrived_at_terminal(const Aircraft& aircraft);
```

<u>**class Aircraft :**</u>

```c++
public:
    //Constructeur 
    Aircraft(const AircraftType& type_, const std::string_view& flight_number_, const Point3D& pos_,
             const Point3D& speed_, Tower& control_) :
        GL::Displayable { pos_.x() + pos_.y() },
        type { type_ },
        flight_number { flight_number_ },
        pos { pos_ },
        speed { speed_ },
        control { control_ }
    {
        speed.cap_length(max_speed());
    }

    // Retourne le numéro de vol
    const std::string& get_flight_num() const { return flight_number; }

    // Retourne la distance entre 2 points
    float distance_to(const Point3D& p) const { return pos.distance_to(p); }

    // Rendu graphique en OpenGL
    void display() const override;

    // Permet de gerer le mouvement d'un avion, sa vitesse et sa direction.
    void move() override;
```
<u>**class Airport :**</u>

```c++
public:
    // Constructeur
    Airport(const AirportType& type_, const Point3D& pos_, const img::Image* image, const float z_ = 1.0f) :
        GL::Displayable { z_ },
        type { type_ },
        pos { pos_ },
        texture { image },
        terminals { type.create_terminals() },
        tower { *this }
    {}
    // Recupere les infos de la tour.
    Tower& get_tower() { return tower; }

    // Rendu graphique
    void display() const override { texture.draw(project_2D(pos), { 2.0f, 2.0f }); }

    // Permet d'exécuter le move de terminal, on delègue ici
    void move() override
    {
        for (auto& t : terminals)
        {
            t.move();
        }
    }
```

<u>**class Terminal :**</u>

```c++
public:
    // Constructeur
    Terminal(const Point3D& pos_) : pos { pos_ } {}
    // Renvois si le terminal à actuellement un avion.
    bool in_use() const { return current_aircraft != nullptr; }

    // Tant que le service n'est pas 100% compliquer on continue 
    bool is_servicing() const { return service_progress < SERVICE_CYCLES; }

    // Permet  d'assigner à un terminal un avion qui va atterire et se mettre ici.
    void assign_craft(const Aircraft& aircraft) { current_aircraft = &aircraft; }

    // Permet de lancer le service associcé a l'avion et continuer le service tant qu'il n'est pas terminer
    void start_service(const Aircraft& aircraft)
    {
        assert(aircraft.distance_to(pos) < DISTANCE_THRESHOLD);
        std::cout << "now servicing " << aircraft.get_flight_num() << "...\n";
        service_progress = 0;
    }
    // Permet de renvoyer l'avion une fois qu'il a terminer sa tâche
    void finish_service()
    {
        if (!is_servicing())
        {
            std::cout << "done servicing " << current_aircraft->get_flight_num() << '\n';
            current_aircraft = nullptr;
        }
    }
    // Permet d'excuter le service auquel l'avion est rattacher, tant que le service n'est pas terminer l'avion ne pars pas et continue sa tâche. 
    void move() override
    {
        if (in_use() && is_servicing())
        {
            ++service_progress;
        }
    }

```
---
**TODO : Schema** :

---

```Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ? Quel conteneur de la librairie standard a été choisi pour représenter le chemin ? Expliquez les intérêts de ce choix.```

Il y a geometry.hpp
runway,
waypoint,
airport_type

```c++
// ------------------------------------------
// Airport_type

WaypointQueue air_to_terminal(const Point3D& offset, const size_t runway_num,const size_t terminal_num) const;

WaypointQueue terminal_to_air(const Point3D& offset, const size_t runway_num, const size_t terminal_num) const;

// ------------------------------------------

// runway.hpp -> Struct Runway 

// geometry.hpp -> Struct Point2D

// waypoint.hpp -> Struct Point3D

```
On utilise le conteneur <std::vector>, Vector est dynamique (a verifier)

---

### C- Bidouillons !

1. Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion. Le Concorde est censé pouvoir voler plus vite que les autres avions. Modifiez le programme pour tenir compte de cela.
   
```cpp 
inline void init_aircraft_types();
```
Dans le fichier aircraft_types, on peux voir le max speed sur le sol dans les air, et l'acceleration max init_aircraft_types() permet donc de modifier ça pour la concorde.

```cpp
inline void init_aircraft_types()
{
    aircraft_types[0] = new AircraftType { .02f, .05f, .02f, MediaPath { "l1011_48px.png" } };
    aircraft_types[1] = new AircraftType { .02f, .05f, .02f, MediaPath { "b707_jat.png" } };
    aircraft_types[2] = new AircraftType { .02f, .07f, .03f, MediaPath { "concorde_af.png" } };
}
```

2. Identifiez quelle variable contrôle le framerate de la simulation. Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur. Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?
Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.

La variable de contrôle **inline unsigned int ticks_per_sec**.
J'ai ajouté 2 fonctions ```void increment_framerate()``` et ```void decrement_framerate()``` dans opengl_interface qui permet de d'incrementer de 1 ou de decrementer de 1 le framerate en appuyant respectivement sur p et m
Pour le pause j'ai fais un bool paused qui permet de verifier si on est en pause, et dans la fonction timer() je verifie si on est pas en pause pour faire la boucle for each.
Il faut appuyer sur la barre d'espace pour mettre en pause et unpause.

3. Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.

```unsigned int service_progress``` est la variable de contrôle pour le temps de débarquement. On va dans config et on fait **2 * 20u** pour la constante ```SERVICE_CYCLES```

4. Lorsqu'un avion a décollé, il réattérit peu de temps après. Faites en sorte qu'à la place, il soit retiré du programme.
Indices :
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?
Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ? A quel endroit de la callstack pourriez-vous le faire à la place ?
Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?

- Aircraft::move
- for each cassé
- timer()
- changer signature de move pour retourner l'information
```c++
  timer()
    for(auto& item : move_queue)
        item->move();
```



5. ```c++
    Displayable(const float z_) : z { z_ }
    {
        // ???? Rajouter le Displayable qui vient d'être créé dans le displays_queue
        display_queue.emplace_back(this);
    }
    virtual ~Displayable()
    {
        // Enlever le Displayable qui sera detruit de la display_queue
        display_queue.erase(std::find(display_queue.begin(), display_queue.end(), this));
    }
    ```

    