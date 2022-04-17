# Rapport Task 2: Algorithmes


## Objectif 1 - Refactorisation de l'existant

### A - Structured Bindings
```TowerSimulation::display_help() est chargé de l'affichage des touches disponibles. Dans sa boucle, remplacez const auto& ks_pair par un structured binding adapté.```

```c++
    for (const auto& ks_pair : GL::keystrokes)
    {
        std::cout << ks_pair.first << ' ';
    }

    ///////// TO THIS //////////////

        for (const auto& [value, key] : GL::keystrokes)
    {
        std::cout << value << ' ';
    }
```

### B - Algorithmes divers

1. AircraftManager::move() supprime les avions de la move_queue dès qu'ils sont "hors jeux". En pratique, il y a des opportunités pour des pièges ici. Pour les éviter, <algorithm> met à disposition la fonction std::remove_if. Remplacez votre boucle avec un appel à std::remove_if.
Il a fallut modifier mon contenaire que était une unordered_map et la changer en vector pour pouvoir utiliser std::remove_if.

2. 

```
void AircraftManager::find_nb_aircraft_by_airline(const char c)
{

    int nb_aircrafts = std::count_if(
        aircrafts.begin(), aircrafts.end(),
        [c](auto& i)
        {
            return (i->get_flight_num().find(airlines[atoi(&c)]) == 0);
        }); // ici il faudrait que airlines soit l'element de aircraft_factory mais je n'arrive pas

    std::cout << "nombre d'avion pour airline " << airlines[atoi(&c)] << " : " << nb_aircrafts << std::endl;
}
```
Il a fallut crée dans tower_sim tout les keystrokes de 0 à 7, et crée une fonction dans aircraft_manager pour compter en utilisant la fonction <algorithm> count_if();

### C - - Relooking de Point3D 


La classe `Point3D` présente beaucoup d'opportunités d'appliquer des algorithmes.
Particulairement, des formulations de type `x() = ...; y() = ...; z() = ...;` se remplacent par un seul appel à la bonne fonction de la librairie standard.
Remplacez le tableau `Point3D::values` par un `std::array` et puis,
remplacez le code des fonctions suivantes en utilisant des fonctions de `<algorithm>` / `<numeric>`:

1. `Point3D::operator*=(const float scalar)`
2. `Point3D::operator+=(const Point3D& other)` et `Point3D::operator-=(const Point3D& other)`
3. `Point3D::length() const`

---
```c++

    Point3D& operator+=(const Point3D& other)
    {
        /*
        x() += other.x();
        y() += other.y();
        z() += other.z();
        */

        std::transform(other.values.begin(), other.values.end(), values.begin(), values.begin(),
                       [](auto& i1, auto& i2) { return i2 + i1; });
        return *this;
    }
    Point3D& operator-=(const Point3D& other)
    {
        /*
        x() -= other.x();
        y() -= other.y();
        z() -= other.z();
        */

        std::transform(other.values.begin(), other.values.end(), values.begin(), values.begin(),
                       [](auto& i1, auto& i2) { return i2 - i1; });
        return *this;
    }

        Point3D& operator*=(const float scalar)
    {
        /*
        x() *= scalar;
        y() *= scalar;
        z() *= scalar;
        */
        std::transform(values.begin(), values.end(), values.begin(),
                       [scalar](auto& i) { return i * scalar; });
        return *this;
    }

```

Exemple d'utilisation de std::transform


## Objectif 2 

### B - Un terminal s'il vous plaît

4. Il faut définir un opérateur d'assignation dans WaypointsQueue pour pouvoir faire:
   ```c++
           if (is_circling())
        {
            WaypointQueue res = control.reserve_terminal(*this);
            if (!res.empty())
            {
                waypoints = res; // pour faire cette ligne il nous faut l'operateur d'assignation
            }
        }
    ```

    du coup :
    ```c++
    Waypoint& operator=(const Waypoint& other) // pour aircraft waypoints = res
    {
        if (this != &other)
        {

            values = other.values;
            type   = other.type;
        }
        return *this;
    }
    ```
    pour pouvoir faire ```type = other.type``` il faut enlever le const de type dans waypoint.hpp puis il faut rajouter:

    ```c++
    Waypoint(const Waypoint&) = default; // pour airport_type
    ```

    pour evité ce probleme dans aircraft_type: 
    ```
    error: implicitly-declared ‘constexpr Waypoint::Waypoint(const Waypoint&)’ is deprecated [-Werror=deprecated-copy]
    [build]    47 |         WaypointQueue result { before_in_air, runway_middle, runway_end, crossing };
    ```



### C - Minimiser les crashs
Modification dans aircraft_manager.cpp
```c++
bool sort_by_fuel(std::unique_ptr<Aircraft>& a, std::unique_ptr<Aircraft>& b)
{

    return a->fuel < b->fuel;
}
// Work not as expected
bool sort_by_reserved(std::unique_ptr<Aircraft>& a, std::unique_ptr<Aircraft>& b)
{
    if (a->reserved && !b->reserved)
    {
        return a->reserved > b->reserved;
    }
    if (!a->reserved && b->reserved)
    {
        return b->reserved > a->reserved;
    }
    if (a->reserved == b->reserved)
    {

        return sort_by_fuel(a, b);
    }

    return sort_by_fuel(a, b); // n'arrive jamais tant mieux
}
```
Ajouté dans la fonction move:
```c++
    // C minimiser les crashs
    aircrafts.sort(sort_by_reserved);
    // pour tester comment sont trier les avions.

    std::cout << "List" << std::endl;
    for (auto const& i : aircrafts)
    {
        std::cout << "reserved = " << i->reserved << " fuel = " << i->fuel << std::endl;
    }
```


### D - Réapprovisionnement
1) Afin de pouvoir repartir en toute sécurité, les avions avec moins de 200 unités d'essence doivent être réapprovisionnés par l'aéroport pendant qu'ils sont au terminal.

    Ajoutez une fonction bool Aircraft::is_low_on_fuel() const, qui renvoie true si l'avion dispose de moins de 200 unités d'essence.
    Modifiez le code de Terminal afin que les avions qui n'ont pas suffisamment d'essence restent bloqués.
    Testez votre programme pour vérifier que certains avions attendent bien indéfiniment au terminal. Si ce n'est pas le cas, essayez de faire varier la constante 200.

Modification dans aircraft.cpp 
```c++
bool Aircraft::is_low_on_fuel() const
{
    return fuel < 200;
}
```
Modification dans terminal.hpp
```c++
    bool move() override
    {
        if (in_use() && is_servicing() && !current_aircraft->is_low_on_fuel())
        {
            ++service_progress;
        }
        return true;
    }
```

   2) Dans AircraftManager, implémentez une fonction get_required_fuel, qui renvoie la somme de l'essence manquante (le plein, soit 3'000, moins la quantité courante d'essence) pour les avions vérifiant les conditions suivantes :
   - l'avion est bientôt à court d'essence
   - l'avion n'est pas déjà reparti de l'aéroport.

J'ai rajouter la fonction is_stuck_at_terminal() dans Aircraft pour pouvoir vérifier: `return is_low_on_fuel && is_at_terminal;`
```c++
// Task2 Objectif2 D2
int AircraftManager::get_required_fuel()
{
    for (auto& aircraft : aircrafts)
    {
        if (aircraft->is_stuck_at_terminal())
        {
            return 3000 - aircraft->fuel;
        }
    }
    return 0;
}
```

3) Ajoutez deux attributs fuel_stock et ordered_fuel dans la classe Airport, que vous initialiserez à 0.
Ajoutez également un attribut next_refill_time, aussi initialisé à 0.
Enfin, faites en sorte que la classe Airport ait accès à votre AircraftManager de manière à pouvoir l'interroger.

Il faut ajouter au constructeur d'Airport un accès a la référence de aircraft_manager. Le ownership de aircraft_manager est dans tower_sim.

Donc on rajoute aircraft_manager dans init_airport() de TowerSimulation:

```c++
void TowerSimulation::init_airport()
{
    airport =
        new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                      new img::Image { one_lane_airport_sprite_path.get_full_path() }, aircraft_manager };

    GL::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
}
```

Et ensuite on crée un champ pour stocker la référence de aircraft_manager dans Airport.

```c++
...
    // Task2Obj2D3
    int fuel_stock       = 0;
    int ordered_fuel     = 0;
    int next_refill_time = 0;
    AircraftManager& am;
...
public:
    Airport(const AirportType& type_, const Point3D& pos_, const img::Image* image,
            AircraftManager& aircraft_manager, const float z_ = 1.0f) :
        GL::Displayable { z_ },
        type { type_ },
        pos { pos_ },
        texture { image },
        terminals { type.create_terminals() },
        tower { *this },
        am { aircraft_manager } // new d3
    {}

```

4)  Ajoutez une fonction refill à la classe Aircraft, prenant un paramètre fuel_stock par référence non-constante. Cette fonction rempliera le réservoir de l'avion en soustrayant ce dont il a besoin de fuel_stock. Bien entendu, fuel_stock ne peut pas devenir négatif.
Indiquez dans la console quel avion a été réapprovisionné ainsi que la quantité d'essence utilisée.
```c++ 
// task2obj2d4
void Aircraft::refill(int& fuel_stock)
{

    int cpt         = 0;
    int fuel_needed = 3000 - fuel;
    if (fuel_stock - fuel_needed > 0)
    {
        fuel += fuel_needed;
        cpt = fuel_needed;
        fuel_stock -= fuel_needed;
    }
    else
    {
        fuel += fuel_stock;
        cpt        = fuel_stock;
        fuel_stock = 0;
    }
    if (cpt > 0)
    {
        std::cout << get_flight_num() << " as refill: " << cpt << std::endl;
    }
}
```

5) Définissez maintenant une fonction refill_aircraft_if_needed dans la classe Terminal, prenant un paramètre fuel_stock par référence non-constante. Elle devra appeler la fonction refill sur l'avion actuellement au terminal, si celui-ci a vraiment besoin d'essence.

```c++
    // Task2Obj2D5 il faut changer current_aircraft pour qu'il ne soit pas const sinon on ne pas le refill
    void refill_aircraft_if_needed(int& fuel_stock)
    {

        if (current_aircraft->is_low_on_fuel())
        {
            current_aircraft->refill(fuel_stock);
        }
    }
    
```

6) Modifiez la fonction Airport::move, afin de mettre-en-oeuvre les étapes suivantes.
- Si next_refill_time vaut 0 :
* fuel_stock est incrémenté de la valeur de ordered_fuel.
* ordered_fuel est recalculé en utilisant le minimum entre AircraftManager::get_required_fuel() et 5'000 (il s'agit du volume du camion citerne qui livre le kérosène).
* next_refill_time est réinitialisé à 100.
* La quantité d'essence reçue, la quantité d'essence en stock et la nouvelle quantité d'essence commandée sont affichées dans la console.
- Sinon next_refill_time est décrémenté.
- Chaque terminal réapprovisionne son avion s'il doit l'être.

```c++

    bool move() override
    {

        for (auto& t : terminals)
        {
            if (next_refill_time == 0)
            {

                fuel_stock += ordered_fuel;
                ordered_fuel     = std::min(am.get_required_fuel(), 5000);
                next_refill_time = 100;
            }
            else
            {
                next_refill_time--;
            }

            if (t.current_aircraft != nullptr && t.current_aircraft->is_stuck_at_terminal())
            {

                t.refill_aircraft_if_needed(fuel_stock);
            }

            t.move();
        }
        return true;
    }
```


