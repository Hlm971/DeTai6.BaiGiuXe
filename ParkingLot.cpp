#include "ParkingLot.h"
#include <new>
using namespace std;

// khoi tao bai xe rong gom rows hang, moi hang co spots vi tri
ParkingLot::ParkingLot(int rows, int spots) {
    
    // chi co 26 hang (A - Z), moi hang can it nhat 1 vi tri
    if (rows < 1) rows = 1;
    if (rows > 26) rows = 26;
    if (spots < 1) spots = 1;
 
    head = nullptr;
    rowCount = rows;
    spotsPerRow = spots;
    totalSpots = 0;
    occupiedCount = 0;
 
    initializeLot();
}

// huy danh sach va giai phong bo nho
ParkingLot::~ParkingLot() {
    clear();
}

// tao dslk theo thu tu A1, A2, ... Am, B1, B2, ... 
SpotNode* ParkingLot::initializeLot() {
    clear(); // bo danh sach cu neu co, tranh ro ri bo nho
    
    SpotNode* tail = nullptr;

    for (int i = 0; i < rowCount; i++) {  // hang
        for (int j = 1; j <= spotsPerRow; j++) { // vi tri
            SpotNode* node = new(nothrow) SpotNode;
            if (node == nullptr) { // het bo nho, don phan da tao
                clear();
                return nullptr;
            }
            node->row = static_cast<char>('A' + i);
            node->number = j;
            node->occupied = false;
            node->licensePlate = "";
            node->next = nullptr;

            if (head == nullptr)
                head = node;
            else 
                tail->next = node;
            tail = node;
        }
    }
    totalSpots = rowCount * spotsPerRow;
    occupiedCount = 0;
    return head;
}

// tim cho trong dau tien theo thu tu hang va vi tri
SpotNode* ParkingLot::findFirstFreeSpot() const {
    SpotNode* cur = head;
    while (cur != nullptr) {
        if (!cur->occupied)
            return cur;
        cur = cur->next;
    }
    return nullptr; // bai da day
}

// danh dau cho da co xe
bool ParkingLot::occupySpot(SpotNode* spot, const string& licensePlate) {
    if (spot == nullptr || spot->occupied) 
        return false;
    if (occupiedCount >= totalSpots) // bo dem khong the vuot tong so cho
        return false;
    spot->occupied = true;
    spot->licensePlate = licensePlate;
    occupiedCount++;
    return true;
}

// giai phong cho dau
bool ParkingLot::releaseSpot(SpotNode* spot) {
    if (spot == nullptr || !spot->occupied) 
        return false;
    if (occupiedCount <= 0) // bo dem khong the am
        return false;
    spot->occupied = false;
    spot->licensePlate = "";
    occupiedCount--;
    return true;
}

// lay node dau tien de duyet
SpotNode* ParkingLot::getHead() const {
    return head;
}

// lay tong so cho
int ParkingLot::getTotalSpots() const {
    return totalSpots;
}

// lay so cho da su dung
int ParkingLot::getOccupiedCount() const {
    return occupiedCount;
}

// lay so cho con trong
int ParkingLot::getAvailableCount() const {
    return totalSpots - occupiedCount;
}

// xoa toan bo danh sach va dat lai cac bo dem
void ParkingLot::clear() {
    SpotNode* cur = head;
    while (cur != nullptr) {
        SpotNode* nextNode = cur->next;
        delete cur;
        cur = nextNode;
    }
    head = nullptr;
    occupiedCount = 0;
    totalSpots = 0;
}
