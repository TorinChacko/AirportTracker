#include <iostream>
#include "tp.h"
#include <queue>
#include <unordered_set>
#include <algorithm>

TravelPlanner::TravelPlanner(const FlightManagerBase& flight_manager, const AirportDB& airport_db)
    : TravelPlannerBase(flight_manager, airport_db) {}

void TravelPlanner::add_preferred_airline(std::string airline) {
    preferredAirlines.insert(airline);
}

bool TravelPlanner::plan_travel(std::string source_airport, std::string destination_airport, int start_time, Itinerary& itinerary) const {
    std::queue<std::pair<Itinerary, int>> queue;
    Itinerary initialItinerary;
    initialItinerary.source_airport = source_airport;
    initialItinerary.destination_airport = destination_airport;
    queue.push({ initialItinerary, start_time });

    std::unordered_set<std::string> visited;

    while (!queue.empty()) {
        std::pair<Itinerary, int> current = queue.front();
        queue.pop();

        if (current.first.flights.empty()) {
            // Initial flight search
            std::vector<FlightSegment> flights = get_flight_manager().find_flights(current.first.source_airport, start_time, start_time + get_max_layover());

            // Prioritize preferred airlines
            std::vector<FlightSegment> preferredFlights;
            for (FlightSegment& flight : flights) {
                if (preferredAirlines.find(flight.airline) != preferredAirlines.end()) {
                    preferredFlights.push_back(flight);
                }
            }
            flights.insert(flights.end(), preferredFlights.begin(), preferredFlights.end());

            // Sort flights by departure time for deterministic behavior
            std::sort(flights.begin(), flights.end(), [](const FlightSegment& a, const FlightSegment& b) {
                return a.departure_time < b.departure_time;
                });

            for (FlightSegment& flight : flights) {
                Itinerary newItinerary = current.first;
                newItinerary.flights.push_back(flight);
                newItinerary.total_duration = flight.duration_sec + (flight.departure_time - start_time);
                queue.push({ newItinerary, flight.departure_time + flight.duration_sec });
            }
        }
        else {
            // Subsequent flights
            FlightSegment lastFlight = current.first.flights.back();
            std::vector<FlightSegment> flights = get_flight_manager().find_flights(lastFlight.destination_airport, current.second + get_min_connection_time(), current.second + get_max_layover());

            // Prioritize preferred airlines
            std::vector<FlightSegment> preferredFlights;
            for (FlightSegment& flight : flights) {
                if (preferredAirlines.find(flight.airline) != preferredAirlines.end()) {
                    preferredFlights.push_back(flight);
                }
            }
            flights.insert(flights.end(), preferredFlights.begin(), preferredFlights.end());

            // Sort flights by departure time for deterministic behavior
            std::sort(flights.begin(), flights.end(), [](const FlightSegment& a, const FlightSegment& b) {
                return a.departure_time < b.departure_time;
                });

            for (FlightSegment& flight : flights) {
                if (visited.find(flight.destination_airport) == visited.end()) {
                    Itinerary newItinerary = current.first;
                    newItinerary.flights.push_back(flight);
                    newItinerary.total_duration += flight.duration_sec + (flight.departure_time - current.second);
                    visited.insert(flight.destination_airport);

                    if (flight.destination_airport == destination_airport) {
                        // Found a valid itinerary
                        itinerary = newItinerary;
                        return true;
                    }

                    if (newItinerary.total_duration <= get_max_duration()) {
                        queue.push({ newItinerary, flight.departure_time + flight.duration_sec });
                    }
                }
            }
        }
    }

    return false; // No valid itinerary found
}
