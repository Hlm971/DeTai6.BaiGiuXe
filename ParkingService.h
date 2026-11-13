#ifndef PARKING_SERVICE_H
#define PARKING_SERVICE_H

#include <string>
#include "VehicleHashTable.h"
#include "ParkingLot.h"

using namespace std;

class ParkingService {
private:
    VehicleHashTable* hashTable;
    ParkingLot* parkingLot;

public:
    // Hàm khởi tạo nhận vào con trỏ quản lý Hash Table và Bãi xe
    ParkingService(VehicleHashTable* ht, ParkingLot* pl);

    // Hàm thực hiện nghiệp vụ gửi xe
    bool parkVehicle(string licensePlate, string vehicleType);
};

#endif