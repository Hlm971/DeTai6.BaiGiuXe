
#ifndef VEHICLEHASHTABLE_H
#define VEHICLEHASHTABLE_H

#include <string>
#include <ctime>

// Khai bao truoc SpotNode
struct SpotNode;

// Cau truc luu thong tin xe
struct VehicleRecord {
    std::string licensePlate;
    std::time_t entryTime;
    SpotNode* spot;
    VehicleRecord* next;
};

// Lop quan ly bang bam
class VehicleHashTable {
private:
    VehicleRecord** buckets;
    int bucketCount;
    int size;

public:
    VehicleHashTable();
    ~VehicleHashTable();

    // Khong cho phep sao chep bang bam
    VehicleHashTable(const VehicleHashTable&) = delete;
    VehicleHashTable& operator=(const VehicleHashTable&) = delete;

    int hashFunction(const std::string& plate) const;

    bool insertVehicle(
        const std::string& plate,
        std::time_t entryTime,
        SpotNode* spot
    );

    VehicleRecord* findVehicle(const std::string& plate);

    bool removeVehicle(const std::string& plate);

    int getSize() const;
    int getBucketCount() const;

    void clear();
};

// Ham chuan hoa bien so xe
std::string normalizeLicensePlate(const std::string& plate);

#endif
