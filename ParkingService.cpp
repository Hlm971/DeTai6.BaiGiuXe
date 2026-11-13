#include "ParkingService.h"
#include <iostream>
#include <ctime>

ParkingService::ParkingService(VehicleHashTable* ht, ParkingLot* pl) {
    hashTable = ht;
    parkingLot = pl;
}

bool ParkingService::parkVehicle(const std::string& licensePlate, const std::string& vehicleType) {
    // 1. Kiểm tra biển số đầu vào không được để trống
    if (licensePlate.empty()) {
        std::cout << "Bien so xe khong duoc de trong!\n";
        return false;
    }

    // 2. Kiểm tra xem xe đã có trong bãi (Hash Table) chưa
    // Dùng hàm findVehicle() từ VehicleHashTable.h
    if (hashTable->findVehicle(licensePlate) != nullptr) {
        std::cout << "Xe co bien so " << licensePlate << " da ton tai trong bai!\n";
        return false;
    }

    // 3. Kiểm tra xem bãi xe còn chỗ trống không
    if (parkingLot->getAvailableCount() <= 0) {
        std::cout << "Bai giu xe da day!\n";
        return false;
    }

    // 4. Tìm vị trí trống đầu tiên trong bãi xe
    SpotNode* freeSpot = parkingLot->findFirstFreeSpot();
    if (freeSpot == nullptr) {
        std::cout << "Khong tim thay vi tri trong trong bai!\n";
        return false;
    }

    // 5. Tiến hành cập nhật bãi xe và bảng băm
    parkingLot->occupySpot(freeSpot, licensePlate);

    // Lấy thời gian hiện tại để đưa vào insertVehicle
    std::time_t currentTime = std::time(nullptr);
    hashTable->insertVehicle(licensePlate, currentTime, freeSpot);

    std::cout << "Gui xe thanh cong cho bien so " << licensePlate
        << " tai vi tri: " << freeSpot->row << freeSpot->number << "\n";
    return true;
}