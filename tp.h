#ifndef TP_H_
#define TP_H_

#include <set>
#include "provided.h"
#include "support.h"

class TravelPlanner : public TravelPlannerBase {
private:
    std::set<std::string> preferredAirlines;
    std::unordered_map<std::string, GeoLocation> airport_coords;

public:
    TravelPlanner(const FlightManagerBase& flight_manager, const AirportDB& airport_db);
    bool plan_travel(std::string source_airport, std::string destination_airport, int start_time, Itinerary& itinerary) const;
    void add_preferred_airline(std::string airline);
};

#endif // TP_H_
