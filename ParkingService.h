#ifndef PARKING_SERVICE_H
#define PARKING_SERVICE_H

#include <string>
#include <ctime>
#include "ParkingLot.h"
#include "VehicleHashTable.h"

// Định nghĩa trạng thái gửi xe theo đặc tả của nhóm
enum class ParkStatus {
    SUCCESS,
    INVALID_PLATE,
    DUPLICATE_PLATE,
    LOT_FULL,
    INTERNAL_ERROR
};

// Xử lý nghiệp vụ gửi xe vào bãi.
// Hàm được Menu.cpp và tests.cpp gọi trực tiếp.
ParkStatus parkVehicle(
    ParkingLot& lot,
    VehicleHashTable& table,
    const std::string& plate,
    std::time_t entryTime,
    std::string& message
);

#endif // PARKING_SERVICE_H
