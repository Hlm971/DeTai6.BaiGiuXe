
#include "VehicleHashTable.h"
#include <cctype>

// Chuan hoa bien so xe
std::string normalizeLicensePlate(const std::string& plate) {
    std::string result = "";

    for (char c : plate) {
        // Bo qua khoang trang, dau gach ngang va dau cham
        if (c == ' ' || c == '-' || c == '.') {
            continue;
        }

        // Chuyen chu thuong thanh chu hoa
        result += static_cast<char>(
            std::toupper(static_cast<unsigned char>(c))
        );
    }

    return result;
}

// Ham khoi tao bang bam
VehicleHashTable::VehicleHashTable()
    : buckets(nullptr), bucketCount(101), size(0)
{
    // Tao mang 101 bucket, cac bucket ban dau la nullptr
    buckets = new VehicleRecord*[bucketCount]();
}

// Ham huy
VehicleHashTable::~VehicleHashTable() {
    clear();

    // Giai phong mang bucket
    delete[] buckets;
}

// Ham bam bien so xe
int VehicleHashTable::hashFunction(
    const std::string& plate
) const {
    std::string normalized = normalizeLicensePlate(plate);

    unsigned long long hash = 0;

    for (char c : normalized) {
        hash = (hash * 31 +
                static_cast<unsigned char>(c))
                % bucketCount;
    }

    return static_cast<int>(hash);
}

// Ham them xe vao bang bam
bool VehicleHashTable::insertVehicle(
    const std::string& plate,
    std::time_t entryTime,
    SpotNode* spot
) {
    // Chuan hoa bien so xe
    std::string normalized = normalizeLicensePlate(plate);

    // Khong them neu bien so rong
    if (normalized.empty()) {
        return false;
    }

    // Tinh vi tri bucket
    int index = hashFunction(normalized);

    // Kiem tra bien so da ton tai chua
    VehicleRecord* current = buckets[index];

    while (current != nullptr) {
        if (current->licensePlate == normalized) {
            return false;
        }

        current = current->next;
    }

    // Tao node moi
    VehicleRecord* newNode = new VehicleRecord{
        normalized,
        entryTime,
        spot,
        buckets[index]
    };

    // Chen node vao dau danh sach lien ket
    buckets[index] = newNode;

    // Tang so luong xe
    size++;

    return true;
}

// Ham tim xe theo bien so
VehicleRecord* VehicleHashTable::findVehicle(
    const std::string& plate
) {
    // Chuan hoa bien so xe
    std::string normalized = normalizeLicensePlate(plate);

    // Khong tim neu bien so rong
    if (normalized.empty()) {
        return nullptr;
    }

    // Tinh vi tri bucket
    int index = hashFunction(normalized);

    // Duyet danh sach lien ket
    VehicleRecord* current = buckets[index];

    while (current != nullptr) {
        if (current->licensePlate == normalized) {
            return current;
        }

        current = current->next;
    }

    // Khong tim thay xe
    return nullptr;
}

// Ham xoa xe khoi bang bam
bool VehicleHashTable::removeVehicle(
    const std::string& plate
) {
    // Chuan hoa bien so xe
    std::string normalized = normalizeLicensePlate(plate);

    // Khong xoa neu bien so rong
    if (normalized.empty()) {
        return false;
    }

    // Tinh vi tri bucket
    int index = hashFunction(normalized);

    // Khoi tao hai con tro
    VehicleRecord* current = buckets[index];
    VehicleRecord* previous = nullptr;

    // Duyet danh sach lien ket
    while (current != nullptr) {
        if (current->licensePlate == normalized) {

            // Neu node can xoa la node dau tien
            if (previous == nullptr) {
                buckets[index] = current->next;
            }
            else {
                previous->next = current->next;
            }

            // Giai phong bo nho node
            delete current;

            // Giam so luong xe
            size--;

            return true;
        }

        // Di chuyen den node tiep theo
        previous = current;
        current = current->next;
    }

    // Khong tim thay xe
    return false;
}

// Ham lay so luong xe hien co
int VehicleHashTable::getSize() const {
    return size;
}

// Ham lay so luong bucket
int VehicleHashTable::getBucketCount() const {
    return bucketCount;
}

// Ham xoa toan bo du lieu trong bang bam
void VehicleHashTable::clear() {
    for (int i = 0; i < bucketCount; i++) {
        VehicleRecord* current = buckets[i];

        // Xoa tung node trong danh sach lien ket
        while (current != nullptr) {
            VehicleRecord* temp = current;
            current = current->next;

            delete temp;
        }

        // Dua bucket ve trang thai rong
        buckets[i] = nullptr;
    }

    // Dat lai so luong xe
    size = 0;
}
