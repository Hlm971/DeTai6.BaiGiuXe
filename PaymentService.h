
#ifndef PAYMENT_SERVICE_H
#define PAYMENT_SERVICE_H

#include <string>
#include <ctime>

using namespace std;

class ParkingLot;
class VehicleHashTable;

class PaymentService {
private:
    ParkingLot* parkingLot;
    VehicleHashTable* vehicleTable;

    const long long MOTORBIKE_RATE = 5000;
    const long long CAR_RATE = 20000;

    long long calculateFee(int minutes, const string& vehicleType);

public:
    PaymentService(ParkingLot* lot, VehicleHashTable* table);

    // Tính số phút gửi xe
    int calculateParkingMinutes(time_t startTime, time_t endTime);

    // Tính tiền gửi xe
    long long calculatePayment(time_t startTime,
        time_t endTime,
        const string& vehicleType);

    // Thực hiện trả xe
    void returnVehicle(const string& licensePlate);

    // In hóa đơn
    void printReceipt(const string& licensePlate,
        time_t startTime,
        time_t endTime,
        int minutes,
        long long fee);
};

#endif
