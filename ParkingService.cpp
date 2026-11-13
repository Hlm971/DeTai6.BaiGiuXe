#include "ParkingService.h"
#include <algorithm>

ParkStatus ParkingService::parkVehicle(
    ParkingLot& lot,
    VehicleHashTable& table,
    const std::string& plate,
    std::time_t entryTime,
    std::string& message
) {
    // 1. Kiểm tra biển số rỗng
    if (plate.empty()) {
        message = "Loi: Bien so xe khong duoc de trong!";
        return ParkStatus::INVALID_PLATE;
    }

    // Chuẩn hóa biển số bằng hàm có sẵn trong VehicleHashTable.h
    std::string normalizedPlate = normalizeLicensePlate(plate);
    if (normalizedPlate.empty()) {
        message = "Loi: Bien so xe sau khi chuan hoa khong hop le!";
        return ParkStatus::INVALID_PLATE;
    }

    // 2. Kiểm tra xe đã có trong bãi (bảng băm) chưa
    // Lưu ý: findVehicle trả về VehicleRecord (hoặc con trỏ tùy cấu trúc, ở đây so sánh với nullptr nếu trả về con trỏ)
    // Nếu nhóm định nghĩa findVehicle trả về con trỏ VehicleRecord*:
    if (table.findVehicle(normalizedPlate) != nullptr) {
        message = "Gui xe that bai: Xe co bien so " + normalizedPlate + " da ton tai trong bai!";
        return ParkStatus::DUPLICATE_PLATE;
    }

    // 3. Kiểm tra xem bãi xe còn chỗ trống không
    if (lot.getAvailableCount() <= 0) {
        message = "Gui xe that bai: Bai giu xe da day!";
        return ParkStatus::LOT_FULL;
    }
    s
    // 4. Tìm vị trí trống đầu tiên trong bãi xe
    SpotNode* freeSpot = lot.findFirstFreeSpot();
    if (freeSpot == nullptr) {
        message = "Gui xe that bai: Khong tim thay vi tri trong trong bai!";
        return ParkStatus::LOT_FULL;
    }

    // 5. Tiến hành chiếm vị trí trong bãi (truyền con trỏ SpotNode* và biển số)
    if (!lot.occupySpot(freeSpot, normalizedPlate)) {
        message = "Gui xe that bai: Khong the danh dau vi tri trong!";
        return ParkStatus::INTERNAL_ERROR;
    }

    // 6. Thêm thông tin xe vào Hash table
    bool insertSuccess = table.insertVehicle(normalizedPlate, entryTime, freeSpot);
    if (!insertSuccess) {
        lot.releaseSpot(freeSpot);
        message = "Gui xe that bai: Loi he thong khi luu vao bang bam!";
        return ParkStatus::INTERNAL_ERROR;
    }

    // 7. Trả kết quả thành công kèm thông tin chi tiết qua message
    message = "Gui xe thanh cong cho bien so " + normalizedPlate +
        " tai vi tri: Hang " + std::to_string(freeSpot->row) +
        ", Cot " + std::to_string(freeSpot->number);

    return ParkStatus::SUCCESS;
}