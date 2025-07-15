#include "fm.h"
#include <fstream>
#include <sstream>
#include <algorithm>

bool FlightManager::load_flight_data(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        std::perror("Error: ");
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string airline, flightNumber, source, destination;
        int departureTime, arrivalTime, duration;

        std::getline(iss, airline, ',');
        std::getline(iss, flightNumber, ',');
        std::getline(iss, source, ',');
        std::getline(iss, destination, ',');
        iss >> departureTime;
        iss.ignore(1, ',');
        iss >> arrivalTime;
        iss.ignore(1, ',');
        iss >> duration;

        FlightSegment segment;
        segment.airline = airline;
        segment.flight_no = std::stoi(flightNumber);
        segment.source_airport = source;
        segment.destination_airport = destination;
        segment.departure_time = departureTime;
        segment.duration_sec = duration;

        flightsBySource[source].push_back(segment);
        departureTimesBySource[source].insert(departureTime); // Insert into BSTSet
    }

    file.close();
    return true;
}

std::vector<FlightSegment> FlightManager::find_flights(const std::string source_airport, int start_time, int end_time) const {
    std::vector<FlightSegment> result;

    // Find flights from the source airport
    std::map<std::string, std::vector<FlightSegment>>::const_iterator it = flightsBySource.find(source_airport);
    if (it != flightsBySource.end()) {
        std::map<std::string, BSTSet<int>>::const_iterator departureTimesIt = departureTimesBySource.find(source_airport);
        if (departureTimesIt != departureTimesBySource.end()) {
            // Use BSTSet to find departure times within the range
            BSTSet<int>::SetIterator endIter = departureTimesIt->second.find_first_not_smaller(end_time);
            if (endIter.get_and_advance() == nullptr) {
                endIter = departureTimesIt->second.find(end_time);
            }

            // Iterate through flights with departure times in the range using iterators
            for (std::vector<FlightSegment>::const_iterator segmentIt = it->second.begin(); segmentIt != it->second.end(); ++segmentIt) {
                if (segmentIt->departure_time >= start_time && segmentIt->departure_time < end_time) {
                    result.push_back(*segmentIt);
                }
            }
        }
    }

    // Sort flights by departure time
    std::sort(result.begin(), result.end(), [](const FlightSegment& a, const FlightSegment& b) {
        return a.departure_time < b.departure_time;
        });

    return result;
}
