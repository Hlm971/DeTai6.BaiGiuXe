#include "Menu.h"

#include "ParkingLot.h"
#include "VehicleHashTable.h"
#include "ParkingService.h"
#include "PaymentService.h"
#include "AvailabilityService.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

bool parseInteger(const std::string& input, int& value) {
    std::istringstream stream(input);

    if (!(stream >> value)) {
        return false;
    }

    stream >> std::ws;
    return stream.eof();
}

std::string formatDateTime(std::time_t timestamp) {
    const std::tm* localTime = std::localtime(&timestamp);
    if (localTime == nullptr) {
        return "Khong xac dinh";
    }

    std::ostringstream output;
    output << std::put_time(localTime, "%d/%m/%Y %H:%M:%S");
    return output.str();
}

void printParkResult(ParkStatus status, const std::string& message) {
    switch (status) {
    case ParkStatus::SUCCESS:
        std::cout << "[THANH CONG] ";
        break;
    case ParkStatus::INVALID_PLATE:
        std::cout << "[BIEN SO KHONG HOP LE] ";
        break;
    case ParkStatus::DUPLICATE_PLATE:
        std::cout << "[XE DA CO TRONG BAI] ";
        break;
    case ParkStatus::LOT_FULL:
        std::cout << "[BAI XE DA DAY] ";
        break;
    case ParkStatus::INTERNAL_ERROR:
        std::cout << "[LOI HE THONG] ";
        break;
    }

    if (!message.empty()) {
        std::cout << message;
    }
    std::cout << '\n';
}

void printReturnResult(ReturnStatus status, const std::string& message) {
    switch (status) {
    case ReturnStatus::SUCCESS:
        std::cout << "[TRA XE THANH CONG] ";
        break;
    case ReturnStatus::INVALID_PLATE:
        std::cout << "[BIEN SO KHONG HOP LE] ";
        break;
    case ReturnStatus::VEHICLE_NOT_FOUND:
        std::cout << "[KHONG TIM THAY XE] ";
        break;
    case ReturnStatus::INVALID_EXIT_TIME:
        std::cout << "[THOI GIAN TRA KHONG HOP LE] ";
        break;
    case ReturnStatus::INCONSISTENT_DATA:
        std::cout << "[DU LIEU KHONG DONG NHAT] ";
        break;
    case ReturnStatus::INTERNAL_ERROR:
        std::cout << "[LOI HE THONG] ";
        break;
    }

    if (!message.empty()) {
        std::cout << message;
    }
    std::cout << '\n';
}

void printReceipt(const PaymentReceipt& receipt) {
    std::cout << "\n========== HOA DON GUI XE ==========\n";
    std::cout << "Bien so xe       : " << receipt.licensePlate << '\n';
    std::cout << "Vi tri dau       : " << receipt.row
              << receipt.spotNumber << '\n';
    std::cout << "Thoi diem gui    : " << formatDateTime(receipt.entryTime) << '\n';
    std::cout << "Thoi diem tra    : " << formatDateTime(receipt.exitTime) << '\n';
    std::cout << "So gio tinh phi  : " << receipt.billedHours << '\n';
    std::cout << "Tong tien        : " << receipt.totalFeeVND << " VND\n";
    std::cout << "====================================\n";
}

} // namespace

Menu::Menu(ParkingLot& lot, VehicleHashTable& table)
    : lot(lot), table(table) {
}

void Menu::showMenu() const {
    std::cout << "\n========================================\n";
    std::cout << "             HE THONG BAI GIU XE\n";
    std::cout << "========================================\n";
    std::cout << "1. Gui xe\n";
    std::cout << "2. Tra xe\n";
    std::cout << "3. Xem cho trong va thong ke bai xe\n";
    std::cout << "0. Thoat chuong trinh\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Nhap lua chon: ";
}

void Menu::handleParkVehicle() {
    std::string plate;
    std::cout << "Nhap bien so xe: ";

    if (!std::getline(std::cin, plate)) {
        std::cout << "Khong doc duoc bien so xe.\n";
        return;
    }

    const std::time_t entryTime = std::time(nullptr);
    if (entryTime == static_cast<std::time_t>(-1)) {
        std::cout << "Khong the lay thoi gian hien tai. Vui long thu lai.\n";
        return;
    }

    std::string message;
    const ParkStatus status = parkVehicle(
        lot,
        table,
        plate,
        entryTime,
        message
    );

    printParkResult(status, message);
}

void Menu::handleReturnVehicle() {
    std::string plate;
    std::cout << "Nhap bien so xe can tra: ";

    if (!std::getline(std::cin, plate)) {
        std::cout << "Khong doc duoc bien so xe.\n";
        return;
    }

    const std::time_t exitTime = std::time(nullptr);
    if (exitTime == static_cast<std::time_t>(-1)) {
        std::cout << "Khong the lay thoi gian hien tai. Vui long thu lai.\n";
        return;
    }

    PaymentReceipt receipt{};
    std::string message;

    const ReturnStatus status = returnVehicle(
        lot,
        table,
        plate,
        exitTime,
        receipt,
        message
    );

    printReturnResult(status, message);

    if (status == ReturnStatus::SUCCESS) {
        printReceipt(receipt);
    }
}

void Menu::handleAvailability() {
    // Menu chi goi dich vu; viec duyet Linked list nam trong AvailabilityService.
    displayAvailableSpots(lot, std::cout);
    displayParkingSummary(lot, std::cout);
}

void Menu::run() {
    std::string input;

    while (true) {
        showMenu();

        if (!std::getline(std::cin, input)) {
            std::cout << "\nKet thuc chuong trinh.\n";
            return;
        }

        int choice = -1;
        if (!parseInteger(input, choice)) {
            std::cout << "Lua chon khong hop le. Vui long nhap so nguyen.\n";
            continue;
        }

        switch (choice) {
        case 1:
            handleParkVehicle();
            break;
        case 2:
            handleReturnVehicle();
            break;
        case 3:
            handleAvailability();
            break;
        case 0:
            std::cout << "Da thoat chuong trinh.\n";
            return;
        default:
            std::cout << "Lua chon khong ton tai. Vui long chon lai.\n";
            break;
        }
    }
}
