#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"
#include "geometry.hpp"

#include <cassert>

class Terminal : public GL::DynamicObject
{
private:
    unsigned int service_progress = SERVICE_CYCLES;
    const Point3D pos;

    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

public:
    Terminal(const Point3D& pos_) : pos { pos_ } {}
    Aircraft* current_aircraft = nullptr; // no more const for d5 and no more private for D6
    bool in_use() const { return current_aircraft != nullptr; }
    bool is_servicing() const { return service_progress < SERVICE_CYCLES; }
    void assign_craft(Aircraft& aircraft) { current_aircraft = &aircraft; } // no more const aswell

    void start_service(const Aircraft& aircraft)
    {
        assert(aircraft.distance_to(pos) < DISTANCE_THRESHOLD);
        std::cout << "now servicing " << aircraft.get_flight_num() << "...\n";
        service_progress = 0;
    }

    void finish_service()
    {

        if (!is_servicing())
        {
            std::cout << "done servicing " << current_aircraft->get_flight_num() << '\n';
            current_aircraft = nullptr;
        }
    }

    bool move() override
    {
        if (in_use() && is_servicing() && !current_aircraft->is_low_on_fuel())
        {
            ++service_progress;
        }
        return true;
    }
    // Task2Obj2D5 il faut changer current_aircraft pour qu'il ne soit pas const sinon on ne pas le refill
    void refill_aircraft_if_needed(int& fuel_stock)
    {

        if (current_aircraft->is_low_on_fuel())
        {
            current_aircraft->refill(fuel_stock);
        }
    }
};