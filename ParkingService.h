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

class ParkingService {
public:
    /**
     * Xử lý nghiệp vụ gửi xe vào bãi.
     * @param lot Tham chiếu đến đối tượng quản lý bãi đỗ xe.
     * @param table Tham chiếu đến bảng băm quản lý xe.
     * @param plate Biển số xe đầu vào.
     * @param entryTime Thời điểm gửi xe.
     * @param message Biến tham chiếu để lưu thông báo kết quả trả ra.
     * @return ParkStatus Trạng thái kết quả thực hiện.
     */
    static ParkStatus parkVehicle(
        ParkingLot& lot,
        VehicleHashTable& table,
        const std::string& plate,
        std::time_t entryTime,
        std::string& message
    );
};

#endif // PARKING_SERVICE_H