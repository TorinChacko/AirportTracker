#ifndef FM_H_
#define FM_H_

#include "provided.h"
#include <map>
#include "bstset.h" // Include BSTSet header

class FlightManager : public FlightManagerBase {
private:
    std::map<std::string, std::vector<FlightSegment>> flightsBySource;
    std::map<std::string, BSTSet<int>> departureTimesBySource; // BSTSet for departure times

public:
    bool load_flight_data(std::string filename);
    std::vector<FlightSegment> find_flights(std::string source_airport, int start_time, int end_time) const;
};

#endif // FM_H_
