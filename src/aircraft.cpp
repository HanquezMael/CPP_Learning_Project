#include "aircraft.hpp"

#include "GL/opengl_interface.hpp"
#include "tower_sim.hpp"

#include <cmath>

void Aircraft::turn_to_waypoint()
{
    if (!waypoints.empty())
    {
        Point3D target = waypoints[0];
        if (waypoints.size() > 1)
        {
            const float d   = (waypoints[0] - pos).length();
            const Point3D W = (waypoints[0] - waypoints[1]).normalize(d / 2.0f);
            target += W;
        }

        turn(target - pos - speed);
    }
}

void Aircraft::turn(Point3D direction)
{
    (speed += direction.cap_length(type.max_accel)).cap_length(max_speed());
}

unsigned int Aircraft::get_speed_octant() const
{
    const float speed_len = speed.length();
    if (speed_len > 0)
    {
        const Point3D norm_speed { speed * (1.0f / speed_len) };
        const float angle =
            (norm_speed.y() > 0) ? 2.0f * 3.141592f - std::acos(norm_speed.x()) : std::acos(norm_speed.x());
        // partition into NUM_AIRCRAFT_TILES equal pieces
        return (static_cast<int>(std::round((angle * NUM_AIRCRAFT_TILES) / (2.0f * 3.141592f))) + 1) %
               NUM_AIRCRAFT_TILES;
    }
    else
    {
        return 0;
    }
}

// when we arrive at a terminal, signal the tower
void Aircraft::arrive_at_terminal()
{
    // we arrived at a terminal, so start servicing
    control.arrived_at_terminal(*this);
    is_at_terminal = true;
}

// deploy and retract landing gear depending on next waypoints
void Aircraft::operate_landing_gear()
{
    if (waypoints.size() > 1u)
    {
        const auto it            = waypoints.begin();
        const bool ground_before = it->is_on_ground();
        const bool ground_after  = std::next(it)->is_on_ground();
        // deploy/retract landing gear when landing/lifting-off
        if (ground_before && !ground_after)
        {
            std::cout << flight_number << " lift off" << std::endl;
        }
        else if (!ground_before && ground_after)
        {
            std::cout << flight_number << " is now landing..." << std::endl;
            landing_gear_deployed = true;
        }
        else if (!ground_before && !ground_after)
        {
            landing_gear_deployed = false;
        }
    }
}

void Aircraft::add_waypoint(const Waypoint& wp, const bool front)
{
    if (front)
    {
        waypoints.push_front(wp);
    }
    else
    {
        waypoints.push_back(wp);
    }
}

bool Aircraft::move()
{
    if (!update())
    {

        if (must_be_deleted())
        {
            return false;
        }

        if (!waypoints.empty() && !reserved)
        {
            if (is_circling())
            {
                WaypointQueue res = control.reserve_terminal(*this);
                if (!res.empty())
                {
                    waypoints = res;
                    reserved  = true;
                }
            }
        }

        if (waypoints.empty())
        {
            waypoints = control.get_instructions(*this);
        }

        if (!is_at_terminal)
        {
            turn_to_waypoint();
            // move in the direction of the current speed
            pos += speed;

            // if we are close to our next waypoint, stike if off the list
            if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)
            {
                if (waypoints.front().is_at_terminal())
                {
                    arrive_at_terminal();
                    already_see_terminal = true;
                }
                else
                {
                    operate_landing_gear();
                }
                waypoints.pop_front();
            }

            if (is_on_ground())
            {
                is_not_on_sky = true;
                if (!landing_gear_deployed)
                {
                    using namespace std::string_literals;
                    // increment_crash_cpt(); //Fonctionne pas comme prévu Task3 2 cpt
                    throw AircraftCrash { flight_number + " crashed into the ground"s };
                }
            }
            else
            {
                // if we are in the air, but too slow, then we will sink!
                const float speed_len = speed.length();
                if (speed_len < SPEED_THRESHOLD)
                {
                    pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed_len);
                }
                is_not_on_sky = false;
            }

            // update the z-value of the displayable structure
            GL::Displayable::z = pos.x() + pos.y();
        }
        return true;
    }
    return false;
}

void Aircraft::display() const
{
    type.texture.draw(project_2D(pos), { PLANE_TEXTURE_DIM, PLANE_TEXTURE_DIM }, get_speed_octant());
}

bool Aircraft::must_be_deleted()
{
    return waypoints.empty() && already_see_terminal && !is_not_on_sky;
}

bool Aircraft::update()
{

    if (!is_not_on_sky)
    {
        if (fuel > 0)
        {
            fuel -= 1;
        }
    }
    if (fuel == 0)
    {
        std::cout << "L'avion " << flight_number << " n'a plus de fuel" << std::endl;

        return true;
    }
    return false;
}

bool Aircraft::has_terminal() const
{

    return waypoints.back().type == wp_terminal;
}

bool Aircraft::is_circling() const
{
    return !has_terminal() && !already_see_terminal && waypoints.back().type == wp_air;
}

bool Aircraft::is_low_on_fuel() const
{
    return fuel < 200;
}

bool Aircraft::is_stuck_at_terminal()
{

    return is_low_on_fuel() && is_at_terminal;
}

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