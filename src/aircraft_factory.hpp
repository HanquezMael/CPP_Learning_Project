#pragma once

#include "aircraft.hpp"
#include "aircraft_types.hpp"
#include "config.hpp"

#include <array>
#include <string>
class AircraftFactory
{

private:
    std::unordered_set<std::string> name_already_attribued {}; // Comme ca 0(1)
    static constexpr size_t NUM_AIRCRAFT_TYPES = 3;
    std::array<AircraftType, NUM_AIRCRAFT_TYPES> aircraft_types {
        AircraftType { .02f, .05f, .02f, MediaPath { "l1011_48px.png" } },
        AircraftType { .02f, .05f, .02f, MediaPath { "b707_jat.png" } },
        AircraftType { .02f, .07f, .03f, MediaPath { "concorde_af.png" } }
    };

public:
    const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
    std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type, Tower& t);
    std::unique_ptr<Aircraft> create_random_aircraft(Tower& t); // peut etre non obliger
};