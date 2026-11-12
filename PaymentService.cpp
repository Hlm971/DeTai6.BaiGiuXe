#include "PaymentService.h"
#include "ParkingLot.h"
#include "VehicleHashTable.h"

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

static string formatTime(time_t t) {
    char buf[32];
#ifdef _WIN32
    ctime_s(buf, sizeof(buf), &t);
#else
    ctime_r(&t, buf);
#endif
    string s(buf);
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r'))
        s.pop_back();
    return s;
}

PaymentService::PaymentService(ParkingLot* lot, VehicleHashTable* table)
    : parkingLot(lot), vehicleTable(table) {
}

// Tinh so phut gui xe (lam tron len). Tra ve -1 neu thoi gian ra < thoi gian vao
int PaymentService::calculateParkingMinutes(time_t startTime, time_t endTime) {
    double seconds = difftime(endTime, startTime);
    if (seconds < 0)
        return -1;
    return static_cast<int>(ceil(seconds / 60.0));
}

// Phi = so phut * gia/gio / 60 (lam tron len). Duoi 3 phut (0.05 gio) mien phi
long long PaymentService::calculateFee(int minutes, const string& vehicleType) {
    if (minutes <= 3)
        return 0;

    long long rate = (vehicleType == "car") ? CAR_RATE : MOTORBIKE_RATE;
    return (static_cast<long long>(minutes) * rate + 59) / 60;
}

long long PaymentService::calculatePayment(time_t startTime,
    time_t endTime,
    const string& vehicleType) {
    int minutes = calculateParkingMinutes(startTime, endTime);
    if (minutes < 0)
        return -1;
    return calculateFee(minutes, vehicleType);
}

// In hoa don
void PaymentService::printReceipt(const string& licensePlate,
    time_t startTime,
    time_t endTime,
    int minutes,
    long long fee) {
    VehicleRecord* record = vehicleTable->findVehicle(licensePlate);

    cout << "\n================ HOA DON THANH TOAN & TRA XE ================\n";
    cout << "Bien so xe       : " << normalizeLicensePlate(licensePlate) << endl;

    if (record != nullptr) {
        cout << "Loai xe          : "
            << (record->vehicleType == "car" ? "O to" : "Xe may") << endl;

        if (record->spot != nullptr)
            cout << "Vi tri do        : " << record->spot->row
            << record->spot->number << endl;
        else
            cout << "Vi tri do        : Chua gan vi tri\n";
    }

    cout << "Thoi gian vao    : " << formatTime(startTime) << endl;
    cout << "Thoi gian ra     : " << formatTime(endTime) << endl;
    cout << "Tong thoi gian   : " << minutes / 60 << " gio "
        << minutes % 60 << " phut\n";
    cout << "So tien thanh toan: " << fee << " VND\n";
    cout << "=============================================================\n";
}

// Thuc hien tra xe: tim xe -> tinh tien -> in hoa don -> xac nhan
// -> giai phong cho do -> xoa ho so xe
void PaymentService::returnVehicle(const string& licensePlate) {
    // 1. Tim xe bang bien so
    VehicleRecord* record = vehicleTable->findVehicle(licensePlate);
    if (record == nullptr) {
        cout << "\n[Loi] Khong tim thay xe co bien so: "
            << licensePlate << " trong bai xe!\n";
        return;
    }

    // 2. Lay thoi gian gui va loai xe
    time_t startTime = record->entryTime;
    time_t endTime = time(nullptr);
    string vehicleType = record->vehicleType;

    int minutes = calculateParkingMinutes(startTime, endTime);
    if (minutes < 0) {
        cout << "\n[Loi] Thoi gian tra xe som hon thoi gian vao bai. "
            << "Vui long kiem tra lai!\n";
        return;
    }

    // 3. Tinh tien
    long long fee = calculateFee(minutes, vehicleType);

    // 4. In hoa don
    printReceipt(licensePlate, startTime, endTime, minutes, fee);

    // Xac nhan thanh toan
    char confirm;
    cout << "Xac nhan thanh toan va hoan tat tra xe? (y/n): ";
    cin >> confirm;

    if (confirm != 'y' && confirm != 'Y') {
        cout << "[Huy] Giao dich da bi huy. Xe van giu nguyen trong bai.\n";
        return;
    }

    // 5. Giai phong vi tri do (phai lam truoc khi xoa ho so vi can record->spot)
    if (record->spot != nullptr && parkingLot != nullptr) {
        if (!parkingLot->releaseSpot(record->spot))
            cout << "[Canh bao] Khong the giai phong vi tri do!\n";
    }

    // 6. Xoa ho so xe khoi bang bam (record bi delete sau dong nay)
    vehicleTable->removeVehicle(licensePlate);

    cout << "[Thanh cong] Giao dich hoan tat. Cho do da duoc giai phong, "
        << "xe da roi khoi bai!\n";
}
