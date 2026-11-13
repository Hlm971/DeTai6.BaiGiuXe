#ifndef AVAILABILITY_SERVICE_H
#define AVAILABILITY_SERVICE_H

#include <ostream>
#include "ParkingLot.h"

// Liet ke cac vi tri con trong theo thu tu A1, A2, ..., B1, B2, ...
void displayAvailableSpots(const ParkingLot& lot, std::ostream& out);

// In bang thong ke: tong so vi tri, da su dung, con trong
void displayParkingSummary(const ParkingLot& lot, std::ostream& out);

#endif
