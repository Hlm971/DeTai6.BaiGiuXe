#define _CRT_SECURE_NO_WARNINGS 
//máy tui thêm này mới chạy dc máy mn test có thể bỏ xem chạy dc k =)) 
#include "PaymentService.h"
#include "ParkingLot.h"
#include "VehicleHashTable.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>

using namespace std;

PaymentService::PaymentService(
    ParkingLot* lot,
    VehicleHashTable* table
) {
    parkingLot = lot;
    vehicleTable = table;
}

// Tính thời gian gửi xe theo phút
int PaymentService::calculateParkingMinutes(
    time_t startTime,
    time_t endTime
) {
    if (endTime < startTime) {
        return -1;
    }

    double seconds = difftime(endTime, startTime);

    return static_cast<int>(ceil(seconds / 60.0));
}

// Tính phí theo số giờ bắt đầu
long long PaymentService::calculateFee(
    int minutes,
    const string& vehicleType
) {
    if (minutes < 0) {
        return -1;
    }

    if (minutes == 0) {
        return 0;
    }

    long long rate;

    if (vehicleType == "motorbike") {
        rate = MOTORBIKE_RATE;
    }
    else if (vehicleType == "car") {
        rate = CAR_RATE;
    }
    else {
        return -1;
    }

    int hours = (minutes + 59) / 60;

    return static_cast<long long>(hours) * rate;
}

// Hàm tính tiền công khai
long long PaymentService::calculatePayment(
    time_t startTime,
    time_t endTime,
    const string& vehicleType
) {
    int minutes = calculateParkingMinutes(startTime, endTime);

    if (minutes < 0) {
        return -1;
    }

    return calculateFee(minutes, vehicleType);
}

// In hóa đơn
void PaymentService::printReceipt(
    const string& licensePlate,
    time_t startTime,
    time_t endTime,
    int minutes,
    long long fee
) {
    cout << "\n========== HOA DON GUI XE ==========\n";

    cout << "Bien so xe: " << licensePlate << endl;

    cout << "Thoi gian gui: "
        << ctime(&startTime);

    cout << "Thoi gian tra: "
        << ctime(&endTime);

    cout << "Tong thoi gian: "
        << minutes << " phut\n";

    cout << "Tong tien: "
        << fixed << fee << " VND\n";

    cout << "====================================\n";
}

// Thực hiện trả xe
void PaymentService::returnVehicle(
    const string& licensePlate
) {
    /*
       Phần này cần kết nối với cấu trúc thực tế
       của VehicleHashTable và ParkingLot.

       Cần thực hiện:
       1. Tìm xe bằng biển số.
       2. Lấy thời gian gửi và loại xe.
       3. Tính tiền.
       4. In hóa đơn.
       5. Giải phóng vị trí đỗ.
       6. Xóa hồ sơ xe.
    */

    cout << "Chuc nang tra xe: "
        << licensePlate << endl;

    cout << "Can ket noi voi cac ham cua "
        << "VehicleHashTable va ParkingLot.\n";
}
