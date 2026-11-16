#include "PaymentService.h"

#include "ParkingLot.h"
#include "VehicleHashTable.h"

#include <string>

// Cach tinh phi


bool calculateFee(std::time_t entryTime,
    std::time_t exitTime,
    long long& billedHours,
    long long& fee) {
    const double seconds = std::difftime(exitTime, entryTime);
    if (seconds <= 0) {
        return false;
    }

    const long long totalSeconds = static_cast<long long>(seconds);
    const long long hours = (totalSeconds + 3599) / 3600;  // lam tron len gio

    billedHours = hours;
    fee = hours * kFeePerStartedHourVND;
    return true;
}


// Nghiep vu tra xe


ReturnStatus returnVehicle(ParkingLot& lot,
    VehicleHashTable& table,
    const std::string& licensePlate,
    std::time_t exitTime,
    PaymentReceipt& receipt,
    std::string& message) {
    message.clear();

    // 1. Chuan hoa va kiem tra bien so
    const std::string plate = normalizeLicensePlate(licensePlate);
    if (plate.empty()) {
        message = "Bien so xe khong duoc de trong.";
        return ReturnStatus::INVALID_PLATE;
    }

    // 2. Tim ho so xe
    VehicleRecord* record = table.findVehicle(plate);
    if (record == nullptr) {
        message = "Khong tim thay xe co bien so " + plate + " trong bai.";
        return ReturnStatus::VEHICLE_NOT_FOUND;
    }

    // 3. Ho so xe phai tro toi mot cho dang co dung xe nay
    SpotNode* spot = record->spot;
    if (spot == nullptr || !spot->occupied || spot->licensePlate != plate) {
        message = "Ho so xe " + plate + " khong khop voi du lieu cho do.";
        return ReturnStatus::INCONSISTENT_DATA;
    }

    // Sao chep du lieu can thiet truoc khi record bi xoa
    const std::time_t entryTime = record->entryTime;
    const char row = spot->row;
    const int spotNumber = spot->number;
    const std::string spotName = std::string(1, row) + std::to_string(spotNumber);

    // 4. Tinh phi
    long long billedHours = 0;
    long long fee = 0;
    if (!calculateFee(entryTime, exitTime, billedHours, fee)) {
        message = "Thoi diem tra xe phai sau thoi diem gui xe.";
        return ReturnStatus::INVALID_EXIT_TIME;
    }

    // 5. Giai phong cho do (truoc khi xoa ho so vi can con tro spot)
    if (!lot.releaseSpot(spot)) {
        message = "Khong the giai phong cho " + spotName + ". Xe van o trong bai.";
        return ReturnStatus::INTERNAL_ERROR;
    }

    // 6. Xoa ho so xe (record khong con hop le sau dong nay)
    if (!table.removeVehicle(plate)) {
        lot.occupySpot(spot, plate);  // hoan tac de du lieu van nhat quan
        message = "Khong the xoa ho so xe " + plate + ". Xe van o trong bai.";
        return ReturnStatus::INTERNAL_ERROR;
    }

    // 7. Ghi hoa don (chi khi moi buoc da thanh cong)
    receipt.licensePlate = plate;
    receipt.row = row;
    receipt.spotNumber = spotNumber;
    receipt.entryTime = entryTime;
    receipt.exitTime = exitTime;
    receipt.billedHours = billedHours;
    receipt.totalFeeVND = fee;

    message = "Xe " + plate + " da roi bai, cho " + spotName + " da duoc giai phong.";
    return ReturnStatus::SUCCESS;
}
