#include "AvailabilityService.h"
#include <iomanip>
#include <string>

void displayAvailableSpots(const ParkingLot& lot, std::ostream& out) {
    int freeCount = 0;
    out << "DANH SACH VI TRI TRONG\n";

    // Duyet linked list tu head; thu tu node = A1, A2, ..., B1, B2, ...
    for (const SpotNode* p = lot.getHead(); p != nullptr; p = p->next) {
        if (!p->occupied) {
            out << "  Hang " << p->row << " - Vi tri " << p->number << "\n";
            ++freeCount;
        }
    }

    if (freeCount == 0) {
        out << "Bai xe da het cho trong!\n";
    }
}

static void printLine(std::ostream& out, const std::string& label, int value) {
    const int WIDTH = 24;  // tong do rong nhan + so, canh phai theo mau de bai
    out << label << std::setw(WIDTH - static_cast<int>(label.size())) << value << "\n";
}

void displayParkingSummary(const ParkingLot& lot, std::ostream& out) {
    out << "================================\n"
        << "         THONG KE BAI XE\n"
        << "================================\n";
    printLine(out, "Tong so vi tri:",    lot.getTotalSpots());
    printLine(out, "Vi tri da su dung:", lot.getOccupiedCount());
    printLine(out, "Vi tri con trong:",  lot.getAvailableCount());
    out << "================================\n";
}
