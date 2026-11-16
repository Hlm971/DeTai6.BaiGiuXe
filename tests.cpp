// Chi kiem thu qua giao dien cong khai cua cac module khac.
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ParkingLot.h"
#include "VehicleHashTable.h"
#include "AvailabilityService.h"

#include "ParkingService.h"
#include "PaymentService.h"

// ================= Mini framework =================
struct Result { std::string group, name, expected, actual; bool pass; };
static std::vector<Result> g_results;
static std::string g_group;

static void checkBool(const std::string& name, bool cond) {
    g_results.push_back({g_group, name, "dung", cond ? "dung" : "sai", cond});
}
template <typename A, typename E>
static void checkEq(const std::string& name, const A& actual, const E& expected) {
    std::ostringstream a, e;
    a << actual; e << expected;
    g_results.push_back({g_group, name, e.str(), a.str(), actual == expected});
}
#define CHECK(name, cond) checkBool(name, (cond))
#define CHECK_EQ(name, actual, expected) checkEq(name, (actual), (expected))

static const std::time_t T0 = 1700000000;

// ---------- Ham phu (chi doc qua giao dien cong khai) ----------
static int countFreeByTraversal(const ParkingLot& lot) {
    int n = 0;
    for (const SpotNode* p = lot.getHead(); p; p = p->next) if (!p->occupied) ++n;
    return n;
}
static SpotNode* nodeAt(const ParkingLot& lot, char row, int number) {
    for (SpotNode* p = lot.getHead(); p; p = p->next)
        if (p->row == row && p->number == number) return p;
    return nullptr;
}
static int countSubstr(const std::string& s, const std::string& sub) {
    int n = 0;
    for (std::size_t pos = s.find(sub); pos != std::string::npos; pos = s.find(sub, pos + 1)) ++n;
    return n;
}
static std::string makePlate(int i) { return "51F" + std::to_string(10000 + i); }

// ====================== ParkingLot ======================
void testParkingLot() {
    g_group = "ParkingLot";
    ParkingLot lot(3, 4);                                    // 12 vi tri

    CHECK_EQ("Khoi tao 3x4: tong so vi tri", lot.getTotalSpots(), 12);
    CHECK("Ban dau: 12 trong, 0 da dung",
          lot.getAvailableCount() == 12 && lot.getOccupiedCount() == 0);

    std::string order;
    int nodes = 0;
    for (const SpotNode* p = lot.getHead(); p; p = p->next) {
        order += p->row + std::to_string(p->number) + " ";
        ++nodes;
    }
    CHECK("Thu tu khoi tao A1 A2 A3 A4 B1", order.find("A1 A2 A3 A4 B1 ") == 0);
    CHECK_EQ("Dung 12 node", nodes, 12);
    CHECK("Node cuoi la C4", order.size() >= 3 && order.compare(order.size() - 3, 3, "C4 ") == 0);

    SpotNode* s = lot.findFirstFreeSpot();
    CHECK("Cho trong dau tien la A1", s != nullptr && s->row == 'A' && s->number == 1);
    CHECK("Chiem cho A1 thanh cong", s != nullptr && lot.occupySpot(s, "TEST-1"));
    CHECK("Sau khi chiem: 1 da dung, 11 trong",
          lot.getOccupiedCount() == 1 && lot.getAvailableCount() == 11);
    SpotNode* s2 = lot.findFirstFreeSpot();
    CHECK("Cho trong tiep theo la A2", s2 != nullptr && s2->row == 'A' && s2->number == 2);

    CHECK("occupySpot(nullptr) that bai", !lot.occupySpot(nullptr, "X"));
    CHECK("Chiem lai cho da co xe that bai", s != nullptr && !lot.occupySpot(s, "TEST-2"));
    CHECK_EQ("Chiem that bai khong doi so cho da dung", lot.getOccupiedCount(), 1);

    CHECK("Giai phong A1 thanh cong", s != nullptr && lot.releaseSpot(s));
    CHECK("Giai phong cho trong that bai", s != nullptr && !lot.releaseSpot(s));
    CHECK("releaseSpot(nullptr) that bai", !lot.releaseSpot(nullptr));
    CHECK("Sau giai phong: 0 da dung, 12 trong",
          lot.getOccupiedCount() == 0 && lot.getAvailableCount() == 12);
    SpotNode* s3 = lot.findFirstFreeSpot();
    CHECK("A1 duoc tim lai dau tien", s3 != nullptr && s3->row == 'A' && s3->number == 1);

    // Tham so khong hop le duoc constructor chinh lai
    ParkingLot tiny(0, 0);
    CHECK_EQ("rows, spots < 1 -> chinh ve 1 cho", tiny.getTotalSpots(), 1);
    ParkingLot big(30, 2);
    CHECK_EQ("rows > 26 -> chan o 26 hang (52 cho)", big.getTotalSpots(), 52);

    // Lap day bai
    for (int i = 0; i < 20; ++i) {                           // gioi han vong lap de tranh treo
        SpotNode* f = lot.findFirstFreeSpot();
        if (!f || !lot.occupySpot(f, "FULL-" + std::to_string(i))) break;
    }
    CHECK("Bai day: khong con cho trong",
          lot.findFirstFreeSpot() == nullptr && lot.getOccupiedCount() == 12
          && lot.getAvailableCount() == 0);
}

// ====================== VehicleHashTable ======================
void testVehicleHashTable() {
    g_group = "VehicleHashTable";
    VehicleHashTable t;
    SpotNode dummy{'A', 1, false, "", nullptr};
    const std::string plate = normalizeLicensePlate("59A12345");

    CHECK_EQ("So bucket mac dinh", t.getBucketCount(), 101);
    CHECK("normalize on dinh (chuan hoa 2 lan khong doi)", normalizeLicensePlate(plate) == plate);
    CHECK("normalize: bo dau cach/gach/cham, doi chu hoa",
          normalizeLicensePlate("59a-123.45 ") == "59A12345");

    CHECK("Them xe moi", t.insertVehicle(plate, T0, &dummy));
    CHECK_EQ("Size sau khi them", t.getSize(), 1);
    VehicleRecord* r = t.findVehicle(plate);
    CHECK("Tim thay xe vua them", r != nullptr && r->licensePlate == plate);
    CHECK("Luu dung thoi gian vao va vi tri", r != nullptr && r->entryTime == T0 && r->spot == &dummy);
    CHECK("Tim xe khong ton tai tra ve nullptr", t.findVehicle("99Z99999") == nullptr);
    CHECK("Them trung bien so bi tu choi", !t.insertVehicle(plate, T0 + 1, &dummy));
    CHECK("Them bien so rong bi tu choi", !t.insertVehicle("", T0, &dummy));

    CHECK("Xoa xe ton tai", t.removeVehicle(plate));
    CHECK("Sau khi xoa: khong tim thay, size = 0", t.findVehicle(plate) == nullptr && t.getSize() == 0);
    CHECK("Xoa xe da xoa that bai", !t.removeVehicle(plate));

    // Dung do: so xe > so bucket => theo nguyen ly Dirichlet chac chan co va cham
    const int n = t.getBucketCount() + 50;
    bool allInserted = true, allFound = true;
    for (int i = 0; i < n; ++i)
        allInserted = t.insertVehicle(makePlate(i), T0 + i, &dummy) && allInserted;
    CHECK("Them n xe (n > so bucket) deu thanh cong", allInserted);
    CHECK_EQ("Size = n sau khi them", t.getSize(), n);
    for (int i = 0; i < n; ++i)
        if (t.findVehicle(makePlate(i)) == nullptr) allFound = false;
    CHECK("Tim du n xe ke ca xe bi dung do", allFound);

    const int mid = n / 2;
    CHECK("Xoa 1 xe giua chuoi: cac xe khac van con",
          t.removeVehicle(makePlate(mid))
          && t.findVehicle(makePlate(mid)) == nullptr
          && t.findVehicle(makePlate(mid - 1)) != nullptr
          && t.findVehicle(makePlate(mid + 1)) != nullptr);
    CHECK_EQ("Size = n - 1 sau khi xoa", t.getSize(), n - 1);

    t.clear();
    CHECK("clear(): size = 0 va khong con xe",
          t.getSize() == 0 && t.findVehicle(makePlate(0)) == nullptr);
}

// ====================== ParkingService ======================
void testParkingService() {
    g_group = "ParkingService";
    ParkingLot lot(1, 2);                                    // 2 vi tri
    VehicleHashTable table;
    std::string msg;

    CHECK("Bien so rong -> INVALID_PLATE",
          parkVehicle(lot, table, "", T0, msg) == ParkStatus::INVALID_PLATE);
    CHECK("Bien so rong khong doi du lieu", lot.getOccupiedCount() == 0 && table.getSize() == 0);

    msg.clear();
    CHECK("Gui xe thanh cong", parkVehicle(lot, table, "59A12345", T0, msg) == ParkStatus::SUCCESS);
    CHECK("Co thong bao ket qua", !msg.empty());
    CHECK("Sau khi gui: 1 cho da dung, 1 xe trong bang bam",
          lot.getOccupiedCount() == 1 && table.getSize() == 1);
    VehicleRecord* r = table.findVehicle("59A12345");
    CHECK("Xe dau tien duoc cap cho A1",
          r != nullptr && r->spot != nullptr && r->spot->row == 'A' && r->spot->number == 1);

    CHECK("Bien so trung -> DUPLICATE_PLATE",
          parkVehicle(lot, table, "59A12345", T0 + 60, msg) == ParkStatus::DUPLICATE_PLATE);
    CHECK("Bien so trung khac dinh dang (59a-123.45) cung bi tu choi",
          parkVehicle(lot, table, "59a-123.45", T0 + 61, msg) == ParkStatus::DUPLICATE_PLATE);
    CHECK("Gui trung khong chiem them cho", lot.getOccupiedCount() == 1 && table.getSize() == 1);

    CHECK("Gui xe thu 2 thanh cong",
          parkVehicle(lot, table, "51F11111", T0 + 120, msg) == ParkStatus::SUCCESS);
    VehicleRecord* r2 = table.findVehicle("51F11111");
    CHECK("Xe thu 2 duoc cap cho A2",
          r2 != nullptr && r2->spot != nullptr && r2->spot->row == 'A' && r2->spot->number == 2);

    CHECK("Bai day -> LOT_FULL",
          parkVehicle(lot, table, "30G99999", T0 + 180, msg) == ParkStatus::LOT_FULL);
    CHECK("Bai day: khong nhan them xe", lot.getOccupiedCount() == 2 && table.getSize() == 2);
}

// ====================== PaymentService ======================
void testPaymentService() {
    g_group = "PaymentService";
    long long hours = -1, fee = -1;
    CHECK("Tinh phi 3 gio tron = 3 x don gia",
          calculateFee(T0, T0 + 3 * 3600, hours, fee) && hours == 3 && fee == 3 * kFeePerStartedHourVND);
    CHECK("Gio ra < gio vao bi tu choi", !calculateFee(T0 + 100, T0, hours, fee));
    CHECK("Gio ra = gio vao bi tu choi", !calculateFee(T0, T0, hours, fee));

    ParkingLot lot(1, 2);
    VehicleHashTable table;
    std::string msg;
    PaymentReceipt rc{};
    parkVehicle(lot, table, "51F11111", T0, msg);

    CHECK("Tra xe thanh cong",
          returnVehicle(lot, table, "51F11111", T0 + 2 * 3600, rc, msg) == ReturnStatus::SUCCESS);
    CHECK("Hoa don: dung bien so", rc.licensePlate == normalizeLicensePlate("51F11111"));
    CHECK("Hoa don: dung hang va vi tri", rc.row == 'A' && rc.spotNumber == 1);
    CHECK("Hoa don: dung gio vao / gio ra", rc.entryTime == T0 && rc.exitTime == T0 + 2 * 3600);
    CHECK("Hoa don: 2 gio, 10000 VND", rc.billedHours == 2 && rc.totalFeeVND == 2 * kFeePerStartedHourVND);
    CHECK("Sau tra xe: cho duoc giai phong", lot.getOccupiedCount() == 0 && lot.getAvailableCount() == 2);
    CHECK("Sau tra xe: xe bi xoa khoi bang bam",
          table.findVehicle("51F11111") == nullptr && table.getSize() == 0);

    CHECK("Xe khong ton tai -> VEHICLE_NOT_FOUND",
          returnVehicle(lot, table, "30G99999", T0 + 5000, rc, msg) == ReturnStatus::VEHICLE_NOT_FOUND);
    CHECK("Bien so rong -> INVALID_PLATE",
          returnVehicle(lot, table, "", T0 + 5000, rc, msg) == ReturnStatus::INVALID_PLATE);

    parkVehicle(lot, table, "51F22222", T0 + 10000, msg);
    CHECK("Gio ra < gio vao -> INVALID_EXIT_TIME",
          returnVehicle(lot, table, "51F22222", T0, rc, msg) == ReturnStatus::INVALID_EXIT_TIME);
    CHECK("Loi thoi gian: xe van con trong bai",
          lot.getOccupiedCount() == 1 && table.findVehicle("51F22222") != nullptr);
}

// ====================== AvailabilityService ======================
static std::string expectedSummary(int total, int used) {
    std::ostringstream o;
    o << "================================\n"
      << "         THONG KE BAI XE\n"
      << "================================\n"
      << "Tong so vi tri:" << std::setw(9) << total << "\n"
      << "Vi tri da su dung:" << std::setw(6) << used << "\n"
      << "Vi tri con trong:" << std::setw(7) << (total - used) << "\n"
      << "================================\n";
    return o.str();
}

void testAvailabilityService() {
    g_group = "AvailabilityService";
    ParkingLot lot(3, 4);                                    // 12 vi tri

    const char rows[5] = {'A', 'A', 'B', 'C', 'C'};
    const int  nums[5] = { 1,   3,   2,   4,   1 };          // 5 vi tri da dung
    for (int i = 0; i < 5; ++i)
        lot.occupySpot(nodeAt(lot, rows[i], nums[i]), "T" + std::to_string(i));
    CHECK_EQ("Chuan bi: so cho da co xe", lot.getOccupiedCount(), 5);

    std::ostringstream o1;
    displayAvailableSpots(lot, o1);
    const std::string s = o1.str();

    bool noOccupied = true;
    for (int i = 0; i < 5; ++i) {
        std::string line = std::string("Hang ") + rows[i] + " - Vi tri " + std::to_string(nums[i]) + "\n";
        if (s.find(line) != std::string::npos) noOccupied = false;
    }
    CHECK("Khong hien thi cho da co xe", noOccupied);

    const std::vector<std::string> freeLines = {
        "Hang A - Vi tri 2", "Hang A - Vi tri 4", "Hang B - Vi tri 1", "Hang B - Vi tri 3",
        "Hang B - Vi tri 4", "Hang C - Vi tri 2", "Hang C - Vi tri 3"};
    bool allShown = true, ordered = true;
    std::size_t last = 0;
    for (const auto& line : freeLines) {
        std::size_t pos = s.find(line);
        if (pos == std::string::npos) { allShown = false; ordered = false; continue; }
        if (pos < last) ordered = false;
        last = pos;
    }
    CHECK("Hien thi du 7 cho trong", allShown);
    CHECK("Dung thu tu hang va so vi tri", ordered);
    CHECK_EQ("So dong vi tri trong", countSubstr(s, "Hang "), 7);
    CHECK("Con cho: khong co thong bao het cho", s.find("het cho trong") == std::string::npos);

    std::ostringstream o2;
    displayParkingSummary(lot, o2);
    CHECK("Thong ke 12/5/7 khop chinh xac mau de bai", o2.str() == expectedSummary(12, 5));
    CHECK("getAvailableCount khop voi duyet danh sach",
          lot.getAvailableCount() == countFreeByTraversal(lot));

    // Giai phong 1 cho -> hien lai trong danh sach, thong ke cap nhat
    lot.releaseSpot(nodeAt(lot, 'A', 1));
    std::ostringstream o2b, o2c;
    displayAvailableSpots(lot, o2b);
    displayParkingSummary(lot, o2c);
    CHECK("Giai phong A1 -> A1 hien lai, dung vi tri dau tien",
          o2b.str().find("Hang A - Vi tri 1") != std::string::npos
          && o2b.str().find("Hang A - Vi tri 1") < o2b.str().find("Hang A - Vi tri 2"));
    CHECK("Thong ke sau giai phong: 12/4/8", o2c.str() == expectedSummary(12, 4));

    // Bai day
    ParkingLot full(1, 2);
    full.occupySpot(nodeAt(full, 'A', 1), "F0");
    full.occupySpot(nodeAt(full, 'A', 2), "F1");
    std::ostringstream o3, o3b;
    displayAvailableSpots(full, o3);
    displayParkingSummary(full, o3b);
    CHECK("Bai day: co thong bao het cho", o3.str().find("het cho trong") != std::string::npos);
    CHECK_EQ("Bai day: so vi tri duoc liet ke", countSubstr(o3.str(), "Hang "), 0);
    CHECK("Bai day: thong ke 2/2/0", o3b.str() == expectedSummary(2, 2));

    // Bai trong
    ParkingLot empty(3, 4);
    std::ostringstream o4, o4b;
    displayAvailableSpots(empty, o4);
    displayParkingSummary(empty, o4b);
    CHECK_EQ("Bai trong: so vi tri duoc liet ke", countSubstr(o4.str(), "Hang "), 12);
    CHECK("Bai trong: thong ke 12/0/12", o4b.str() == expectedSummary(12, 0));

    // Cac quy mo khac nhau
    ParkingLot one(1, 1);
    std::ostringstream q1, q1b;
    displayAvailableSpots(one, q1);
    CHECK_EQ("Quy mo 1x1 (trong): so vi tri duoc liet ke", countSubstr(q1.str(), "Hang "), 1);
    one.occupySpot(nodeAt(one, 'A', 1), "Q");
    displayAvailableSpots(one, q1b);
    CHECK("Quy mo 1x1 (day): thong bao het cho", q1b.str().find("het cho trong") != std::string::npos);

    ParkingLot wide(1, 100);
    std::ostringstream q2;
    displayAvailableSpots(wide, q2);
    CHECK_EQ("Quy mo 1x100: so vi tri duoc liet ke", countSubstr(q2.str(), "Hang A - Vi tri "), 100);

    ParkingLot huge(26, 50);                                 // 1300 vi tri, A..Z
    for (SpotNode* p = huge.getHead(); p; p = p->next)
        if (p->number % 2 == 0) huge.occupySpot(p, "H");     // chiem cac vi tri chan
    std::ostringstream q3, q3b;
    displayAvailableSpots(huge, q3);
    displayParkingSummary(huge, q3b);
    CHECK_EQ("Quy mo 26x50, chiem so chan: so vi tri trong", countSubstr(q3.str(), "Hang "), 650);
    CHECK("Quy mo 26x50: co hang Z, khong co vi tri chan",
          q3.str().find("Hang Z - Vi tri 49") != std::string::npos
          && q3.str().find("Hang Z - Vi tri 50") == std::string::npos);
    CHECK("Quy mo 26x50: thong ke 1300/650/650", q3b.str() == expectedSummary(1300, 650));

    ParkingLot clamp(0, 0);                                  // ParkingLot chinh ve 1x1
    std::ostringstream q4;
    displayAvailableSpots(clamp, q4);
    CHECK_EQ("Tham so 0x0 (bi chinh ve 1x1): so vi tri duoc liet ke", countSubstr(q4.str(), "Hang "), 1);
}

// ====================== Chay tat ca ======================
void runAllTests() {
    g_results.clear();
    testParkingLot();
    testVehicleHashTable();
    testParkingService();
    testPaymentService();
    testAvailabilityService();

    std::cout << "\n=============== BANG KET QUA KIEM THU ===============\n";
    int pass = 0;
    for (const auto& r : g_results) {
        std::cout << std::left << std::setw(22) << r.group
                  << std::setw(62) << r.name
                  << (r.pass ? "PASS" : "FAIL");
        if (!r.pass) std::cout << "   (mong doi: " << r.expected << ", thuc te: " << r.actual << ")";
        std::cout << "\n";
        if (r.pass) ++pass;
    }
    std::cout << "-----------------------------------------------------\n"
              << "Tong: " << g_results.size() << "   Dat: " << pass
              << "   Loi: " << g_results.size() - pass << "\n";
}

void runTests() { runAllTests(); }   // ten ham theo bang quy uoc giao tiep

#ifdef RUN_TESTS
int main() {
    runAllTests();
    return 0;
}
#endif
