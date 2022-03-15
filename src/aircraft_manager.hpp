#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"
#include "aircraft_manager.hpp"

#include <memory>

class AircraftManager : public GL::DynamicObject
{

private:
    std::unordered_map<std::string, std::unique_ptr<Aircraft>> aircrafts;

public:
    AircraftManager() { GL::move_queue.insert(this); };

    bool move() override;
    void add_aircraft(std::unique_ptr<Aircraft> avion); // std::move
};