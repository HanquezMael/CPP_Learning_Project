#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

TowerSimulation::TowerSimulation(int argc, char** argv) :
    context_initializer { argc, argv },
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{

    /*MediaPath::initialize(argv[0]);*/

    /*std::srand(static_cast<unsigned int>(std::time(nullptr)));*/
    /*GL::init_gl(argc, argv, "Airport Tower Simulation"); */

    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}
/*
void TowerSimulation::create_aircraft(const AircraftType& type) const
{
    assert(airport); // make sure the airport is initialized before creating aircraft

    const std::string flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();

    Aircraft* aircraft = new Aircraft { type, flight_number, start, direction, airport->get_tower() };
    aircraft_manager->create_aircraft(); // new
    //GL::display_queue.emplace_back(aircraft);
    // GL::move_queue.emplace(aircraft);
}
*/
void TowerSimulation::create_random_aircraft()
{
    aircraft_manager.add_aircraft(aircraft_factory.create_random_aircraft(airport->get_tower()));
    // aircraft_manager.add_aircraft(*(aircraft_types[rand() % 3]), airport->get_tower());
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { create_random_aircraft(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace(' ', []() { GL::pause(); });
    GL::keystrokes.emplace('p', []() { GL::increment_framerate(); });
    GL::keystrokes.emplace('m', []() { GL::decrement_framerate(); });
    GL::keystrokes.emplace('l', [this]() { nb_of_crashs(); }); // Task3 O1 2
    GL::keystrokes.emplace('0', [this]() { nb_of_aircraft_by_airline('0'); });
    GL::keystrokes.emplace('1', [this]() { nb_of_aircraft_by_airline('1'); });
    GL::keystrokes.emplace('2', [this]() { nb_of_aircraft_by_airline('2'); });
    GL::keystrokes.emplace('3', [this]() { nb_of_aircraft_by_airline('3'); });
    GL::keystrokes.emplace('4', [this]() { nb_of_aircraft_by_airline('4'); });
    GL::keystrokes.emplace('5', [this]() { nb_of_aircraft_by_airline('5'); });
    GL::keystrokes.emplace('6', [this]() { nb_of_aircraft_by_airline('6'); });
    GL::keystrokes.emplace('7', [this]() { nb_of_aircraft_by_airline('7'); });
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    // sans structured binding
    /*
        for (const auto& ks_pair : GL::keystrokes)
        {
            std::cout << ks_pair.first << ' ';
        }
    */
    // Avec structured binding
    for (const auto& [value, key] : GL::keystrokes)
    {
        std::cout << value << ' ';
    }
    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport =
        new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                      new img::Image { one_lane_airport_sprite_path.get_full_path() }, aircraft_manager };

    GL::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    // init_aircraft_types();

    GL::loop();
}

void TowerSimulation::nb_of_aircraft_by_airline(const char c)
{
    aircraft_manager.find_nb_aircraft_by_airline(c);
}

void TowerSimulation::nb_of_crashs()
{
    std::cout << "Il y a eu: " << crash_cpt << " crash d'avion" << std::endl;
}

void TowerSimulation::increment_crash_cpt()
{
    crash_cpt++;
}