#ifndef MENU_H
#define MENU_H

class ParkingLot;
class VehicleHashTable;

class Menu {
private:
    ParkingLot& lot;
    VehicleHashTable& table;

    void showMenu() const;
    void handleParkVehicle();
    void handleReturnVehicle();
    void handleAvailability();

public:
    Menu(ParkingLot& lot, VehicleHashTable& table);

    void run();
};

#endif // MENU_H
