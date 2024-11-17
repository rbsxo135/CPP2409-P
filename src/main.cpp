#include "Storage.cpp"
using namespace std;

const int STORAGE_COLUMM = 6;
const int FRESH_FOOD_COLUMM = 7;

int main(int argc, char** argv)
{
    // 기능 테스트
    Storage storage("storage.csv");
    storage.removeCode("1000001");
    storage.printDatabase();

    storage.itemSold("1000002", 1);
    storage.printDatabase();

    storage.itemToStand("1000002", 2);
    storage.printDatabase();

    storage.itemToStand("1000002", 2);
    storage.printDatabase();

    storage.itemToStand("1000004", 2);
    storage.printDatabase();

    storage.itemStore("1000002", 20);
    storage.printDatabase();
    
    return 0;
}
