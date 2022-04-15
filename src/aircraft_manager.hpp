#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"
#include "aircraft_manager.hpp"

#include <list>
#include <memory>
class AircraftManager : public GL::DynamicObject
{

private:
    std::list<std::unique_ptr<Aircraft>> aircrafts;
    // std::unordered_map<std::string, std::unique_ptr<Aircraft>> aircrafts2; //avant

public:
    AircraftManager() { GL::move_queue.insert(this); };

    bool move() override;
    void add_aircraft(std::unique_ptr<Aircraft> avion); // std::move
    void find_nb_aircraft_by_airline(const char c);
    int get_required_fuel();
};