#pragma once

#include "aircraft_factory.hpp"
#include "aircraft_manager.hpp"
class Airport;

struct AircraftType;
struct ContextInitializer
{

public:
    ContextInitializer(int argc, char** argv)
    {
        MediaPath::initialize(argv[0]);
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        GL::init_gl(argc, argv, "Airport Tower Simulation");
    }
};

class TowerSimulation
{
private:
    ContextInitializer context_initializer;
    bool help        = false;
    Airport* airport = nullptr;

    AircraftFactory aircraft_factory {}; // new
    AircraftManager aircraft_manager {}; // new
    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    /*void create_aircraft(const AircraftType& type)const; */
    void create_random_aircraft();

    void create_keystrokes();
    void display_help() const;

    void init_airport();

    void nb_of_aircraft_by_airline(const char c);

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
