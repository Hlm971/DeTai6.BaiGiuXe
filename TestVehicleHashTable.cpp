
#include "VehicleHashTable.h"
#include <iostream>
#include <cassert>
#include <string>

int main() {
    VehicleHashTable table;

    std::cout << "===== KIEM TRA HASH TABLE =====\n";

    // Kiem tra so bucket ban dau
    assert(table.getBucketCount() == 101);
    assert(table.getSize() == 0);

    std::cout << "1. Khoi tao bang bam: PASS\n";

    // Kiem tra them xe
    bool result1 = table.insertVehicle(
        "59A1-123.45", 1000, nullptr
    );

    bool result2 = table.insertVehicle(
        "51B2-678.90", 2000, nullptr
    );

    assert(result1 == true);
    assert(result2 == true);
    assert(table.getSize() == 2);

    std::cout << "2. Them xe: PASS\n";

    // Kiem tra bien so trung sau khi chuan hoa
    bool duplicate = table.insertVehicle(
        "59a1 12345", 3000, nullptr
    );

    assert(duplicate == false);
    assert(table.getSize() == 2);

    std::cout << "3. Kiem tra bien so trung: PASS\n";

    // Kiem tra tim xe
    VehicleRecord* vehicle =
        table.findVehicle("59a1-123.45");

    assert(vehicle != nullptr);
    assert(vehicle->licensePlate == "59A112345");

    std::cout << "4. Tim xe ton tai: PASS\n";

    // Kiem tra tim xe khong ton tai
    VehicleRecord* notFound =
        table.findVehicle("99Z9-999.99");

    assert(notFound == nullptr);

    std::cout << "5. Tim xe khong ton tai: PASS\n";

    // Kiem tra xoa xe
    bool removed = table.removeVehicle("59A1-123.45");

    assert(removed == true);
    assert(table.getSize() == 1);
    assert(table.findVehicle("59A1-123.45") == nullptr);

    std::cout << "6. Xoa xe: PASS\n";

    // Kiem tra xoa xe khong ton tai
    bool removeNotFound =
        table.removeVehicle("99Z9-999.99");

    assert(removeNotFound == false);
    assert(table.getSize() == 1);

    std::cout << "7. Xoa xe khong ton tai: PASS\n";

    // Kiem tra va cham
    int index1 = table.hashFunction("51A0050");
    int index2 = table.hashFunction("51A0088");
    int index3 = table.hashFunction("51A0102");

    assert(index1 == index2);
    assert(index2 == index3);

    table.insertVehicle("51A0050", 4000, nullptr);
    table.insertVehicle("51A0088", 5000, nullptr);
    table.insertVehicle("51A0102", 6000, nullptr);

    assert(table.getSize() == 4);

    std::cout << "8. Xu ly va cham: PASS\n";

    // Xoa node dau tien
    assert(table.removeVehicle("51A0102"));
    assert(table.findVehicle("51A0102") == nullptr);
    assert(table.findVehicle("51A0088") != nullptr);

    std::cout << "9. Xoa node dau: PASS\n";

    // Xoa node o giua
    assert(table.removeVehicle("51A0088"));
    assert(table.findVehicle("51A0088") == nullptr);
    assert(table.findVehicle("51A0050") != nullptr);

    std::cout << "10. Xoa node giua: PASS\n";

    // Xoa node cuoi
    assert(table.removeVehicle("51A0050"));
    assert(table.findVehicle("51A0050") == nullptr);

    std::cout << "11. Xoa node cuoi: PASS\n";

    // Kiem tra xoa toan bo bang
    table.clear();

    assert(table.getSize() == 0);
    assert(table.findVehicle("51B2-678.90") == nullptr);

    std::cout << "12. Xoa toan bo bang: PASS\n";

    std::cout << "\n===== TAT CA KIEM TRA THANH CONG =====\n";

    return 0;
}
