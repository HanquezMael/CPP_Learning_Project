#include "aircraft_factory.hpp"

std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(const AircraftType& type, Tower& t)
{
    const std::string flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();
    // 1. créer l'avion avec make_unique

    // 2. rajouter l'avion dans la liste des avions avec aircrafts.emplace

    return std::make_unique<Aircraft>(type, flight_number, start, direction, t); // cela compile... hurrah :)
}

std::unique_ptr<Aircraft> AircraftFactory::create_random_aircraft(Tower& t)
{

    return create_aircraft(aircraft_types[rand() % 3], t);
}
