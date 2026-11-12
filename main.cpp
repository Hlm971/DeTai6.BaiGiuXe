#include "ParkingLot.h"
#include "VehicleHashTable.h"
#include "Menu.h"

#include <climits>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

namespace {

bool readInteger(const std::string& prompt, int& value) {
    std::string input;

    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, input)) {
            return false;
        }

        std::istringstream stream(input);
        int parsedValue = 0;

        if (stream >> parsedValue) {
            stream >> std::ws;
            if (stream.eof()) {
                value = parsedValue;
                return true;
            }
        }

        std::cout << "Du lieu khong hop le. Vui long nhap mot so nguyen.\n";
    }
}

} // namespace

int main() {
    int rowCount = 0;
    int spotsPerRow = 0;

    std::cout << "========================================\n";
    std::cout << "       KHOI TAO HE THONG BAI GIU XE\n";
    std::cout << "========================================\n";

    if (!readInteger("Nhap so hang (1-26): ", rowCount)) {
        std::cerr << "Khong doc duoc so hang.\n";
        return 1;
    }

    if (rowCount < 1 || rowCount > 26) {
        std::cerr << "So hang phai nam trong khoang tu 1 den 26.\n";
        return 1;
    }

    if (!readInteger("Nhap so vi tri moi hang: ", spotsPerRow)) {
        std::cerr << "Khong doc duoc so vi tri moi hang.\n";
        return 1;
    }

    if (spotsPerRow < 1) {
        std::cerr << "So vi tri moi hang phai lon hon 0.\n";
        return 1;
    }

    // ParkingLot luu tong so vi tri bang int, can tranh tran so khi nhan.
    if (spotsPerRow > INT_MAX / rowCount) {
        std::cerr << "Tong so vi tri vuot qua gioi han luu tru cua chuong trinh.\n";
        return 1;
    }

    try {
        // Constructor ParkingLot hien tai tu goi initializeLot() de tao Linked list.
        ParkingLot parkingLot(rowCount, spotsPerRow);
        VehicleHashTable vehicleTable;

        std::cout << "\nKhoi tao bai xe thanh cong.\n";
        std::cout << "Tong so vi tri: " << parkingLot.getTotalSpots() << '\n';

        Menu menu(parkingLot, vehicleTable);
        menu.run();
    }
    catch (const std::exception& error) {
        std::cerr << "Khong the khoi tao hoac van hanh he thong: "
                  << error.what() << '\n';
        return 1;
    }

    // ParkingLot va VehicleHashTable tu giai phong bo nho trong destructor.
    return 0;
}
