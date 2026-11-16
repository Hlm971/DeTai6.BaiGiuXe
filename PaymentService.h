#ifndef PAYMENT_SERVICE_H
#define PAYMENT_SERVICE_H

#include <ctime>
#include <string>

class ParkingLot;
class VehicleHashTable;

// Visual Studio coi localtime la "unsafe" (C4996) -> tat canh bao cho cac file
// co include header nay (vd: Menu.cpp).
#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

// ---------------------------------------------------------------------------
// Bieu phi: tinh theo gio bat dau (gui qua 1 phut cua gio nao cung tinh tron gio do)
// ---------------------------------------------------------------------------
constexpr long long kFeePerStartedHourVND = 5000;

// ---------------------------------------------------------------------------
// Trang thai tra xe
// ---------------------------------------------------------------------------
enum class ReturnStatus {
    SUCCESS,            // Tra xe thanh cong
    INVALID_PLATE,      // Bien so rong / khong hop le
    VEHICLE_NOT_FOUND,  // Khong co xe nay trong bai
    INVALID_EXIT_TIME,  // Gio ra <= gio vao
    INCONSISTENT_DATA,  // Ho so xe va cho do khong khop
    INTERNAL_ERROR      // Loi he thong
};

// ---------------------------------------------------------------------------
// Hoa don
// ---------------------------------------------------------------------------
struct PaymentReceipt {
    std::string licensePlate;    // Bien so da chuan hoa
    char row = '\0';             // Hang cua cho do (A - Z)
    int spotNumber = 0;          // So cho do trong hang
    std::time_t entryTime = 0;   // Thoi diem gui
    std::time_t exitTime = 0;    // Thoi diem tra
    long long billedHours = 0;   // So gio tinh phi (gio bat dau)
    long long totalFeeVND = 0;   // Tong tien phai tra
};

// ---------------------------------------------------------------------------
// Cach tinh phi
//   billedHours = ceil((exitTime - entryTime) / 3600)
//   fee         = billedHours * kFeePerStartedHourVND
// Tra ve false (va khong thay doi hours, fee) neu exitTime <= entryTime.
// ---------------------------------------------------------------------------
bool calculateFee(std::time_t entryTime,
    std::time_t exitTime,
    long long& billedHours,
    long long& fee);

// ---------------------------------------------------------------------------
// Nghiep vu tra xe:
//   1. Chuan hoa + kiem tra bien so
//   2. Tim ho so xe trong bang bam
//   3. Kiem tra ho so va cho do khop nhau
//   4. Tinh phi
//   5. Giai phong cho do
//   6. Xoa ho so xe khoi bang bam
//   7. Ghi hoa don
// Neu that bai o bat ky buoc nao: xe van o trong bai, receipt khong bi thay doi.
// Ham khong doc/ghi console; Menu chiu trach nhiem hien thi.
// ---------------------------------------------------------------------------
ReturnStatus returnVehicle(ParkingLot& lot,
    VehicleHashTable& table,
    const std::string& licensePlate,
    std::time_t exitTime,
    PaymentReceipt& receipt,
    std::string& message);

#endif // PAYMENT_SERVICE_H
