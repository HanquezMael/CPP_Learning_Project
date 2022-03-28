#include "aircraft_manager.hpp"

#include "aircraft_factory.hpp"

#include <algorithm> //pour std::remove_if
#include <memory>

// const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

bool AircraftManager::move()

{
    // Passer sur un vector à la place d'un unordored map
    aircrafts.erase(
        std::remove_if(aircrafts.begin(), aircrafts.end(), [](auto& it) { return !(it->move()); }),
        aircrafts.end());
    return true;

    /*
      for (auto it = aircrafts.begin();
           it != aircrafts.end();) // Notez bien le manque de l'incremenet ++it ici car on va incrementer it
                                   // selon la réponse de move()
      {

          if (!(it->second)->move())
          {
              // ************* déjà fait TASK0 exo 5
              //          display_queue.erase(*it);   // n'oubliez pas d'enlever l'objet de la display_queue;
              //          (5) Faites en sort que ceci est fait automatiquement (par le destructeur)
              // *************

              // ***** gestion automatique par le destructeur de std::unqiue_ptr
              //          delete *it;                 // c'est pas bien, mais necessaire parce qu'on a créé
              //          l'avion via new.... cela change dès qu'on trouve un propre owner des avions (TASK1)
              it = aircrafts.erase(it); // ici, on enleve *it de aircrafts d'une facon safe
          }
          else
              ++it;
      }
      */
}

void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> avion)
{
    /*
    const std::string flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();
    // 1. créer l'avion avec make_unique

    // 2. rajouter l'avion dans la liste des avions avec aircrafts.emplace
    aircrafts.emplace(
        std::make_pair(flight_number, std::make_unique<Aircraft>(type, flight_number, start, direction,
                                                                 t))); // cela compile... hurrah :)
*/
    aircrafts.emplace_back(std::move(avion));
    // aircrafts2.emplace(avion->get_flight_num(), std::move(avion));
}