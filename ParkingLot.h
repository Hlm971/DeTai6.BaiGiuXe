#ifndef PARKINGLOT_H
#define PARKINGLOT_H

#include <string>

struct SpotNode {  // node luu thong tin tai mot vi tri do xe
    char row;  // A - Z
    int number;
    bool occupied; // vi tri co xe
    std::string licensePlate; // bien so xe
    SpotNode* next;
};

class ParkingLot {
    private:
        SpotNode* head;
        int rowCount; // so hang
        int spotsPerRow; // so vi tri moi hang
        int totalSpots; // tong so vi tri
        int occupiedCount; // so vi tri dang co xe
    
    public:
        ParkingLot(int rows, int spots); // khoi tao bai xe rong
        ~ParkingLot(); // huy danh sach va giai phong bo nho

        // khong cho copy de tranh loi double delete
        ParkingLot(const ParkingLot&) = delete;
        ParkingLot& operator=(const ParkingLot&) = delete;
        
        SpotNode* initializeLot(); // tao danh sach vi tri
        SpotNode* findFirstFreeSpot() const; // tim cho trong dau tien
        bool occupySpot(SpotNode* spot, const std::string& licensePlate); // danh dau cho da co xe
        bool releaseSpot(SpotNode* spot); // giai phong cho dau
        
        SpotNode* getHead() const; // lay node dau tien de duyet
        int getTotalSpots() const; // lay tong so cho
        int getOccupiedCount() const; // lay so cho da su dung
        int getAvailableCount() const; // lay so cho con trong
        
        void clear(); // xoa toan bo danh sach
};

#endif
