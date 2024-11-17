#include "Storage.cpp"
using namespace std;

const int STORAGE_COLUMM = 6;
const int FRESH_FOOD_COLUMM = 7;

int main(int argc, char** argv)
{
    Storage storage("storage.csv");
    bool isPeak = false; // 피크타임 여부 체크하는 변수
    storage.printDatabase();
    bool flag = true;
    int menu;
    while(flag)
    {
        cout << "Item to display on the stand" << endl;
        if(isPeak == true)
            cout << "Peaktime = true" << endl;
        else
            cout << "Peaktime = false" << endl;

        for(int i = 0; i < storage.size(); i++)
        {
            // line번째 아이템들의 값을 저장
            // 추후 각 아이템의 정보를 vector<string>으로 저장하는 것이 아닌 구조체를 만들어 저장하는 것으로 class 정의 업데이트 예정
            vector<string> item = storage.getItem(i);
            int cur_stand_qnt =  stoi(item[3]);
            int max_stand_qnt = stoi(item[4]);
            int storage_qnt = stoi(item[5]);
            
            if(isPeak == true) // 피크타임일 경우 매대에 넣을 수 있는 공간이 있기만 하면 해당 상품 및 추가 진열 수량 제시
            {
                if(cur_stand_qnt < max_stand_qnt && storage_qnt >= (max_stand_qnt - cur_stand_qnt))
                {
                    cout.width(13);
                    cout << std::right << item[0];
                    cout.width(25);
                    cout << std::right << item[1] << " ---> " << max_stand_qnt - cur_stand_qnt << endl;
                }
            } else // 피크타임이 아닐 경우
            {
                // 매대 최대 수량의 40%보다 적은 수량이 매대에 있을 경우 우선도가 높은 진열 품목으로 제시
                if(cur_stand_qnt < (int)(max_stand_qnt * 0.4) && storage_qnt >= ((int)(max_stand_qnt * 0.4) - cur_stand_qnt))
                {
                    cout.width(13);
                    cout << std::right << item[0];
                    cout.width(25);
                    cout << std::right << item[1] << " ---> " << max_stand_qnt - cur_stand_qnt << "  Urgent" << endl;
                } 
                // 매대 최대 수량의 40%이상 60% 미만의 수량이 매대에 있을 경우 우선도가 낮은 진열 품목으로 제시
                else if((cur_stand_qnt >= (int)(max_stand_qnt * 0.4) && cur_stand_qnt < (int)(max_stand_qnt * 0.6))
                 && storage_qnt >= ((int)(max_stand_qnt * 0.6) - cur_stand_qnt))
                {
                    cout.width(13);
                    cout << std::right << item[0];
                    cout.width(25);
                    cout << std::right << item[1] << " ---> " << max_stand_qnt - cur_stand_qnt << "  Not Urgent" << endl;
                }
            }
        }
        // 메뉴 선택
        cout << "1. Sell item  2. Add item  3. Save and Exit" << endl;
        cin >> menu;
        switch(menu){
            case 1:
            {
                string sellItemCode;
                int sellQnt;
                cout << "Item Code: ";
                cin >> sellItemCode;
                cout << "Quantity: ";
                cin >> sellQnt;
                if(stoi(storage.getItem(sellItemCode)[3]) < sellQnt)
                {
                    cout << "Wrong quantity!";
                    break;
                }
                storage.itemSold(sellItemCode, sellQnt);
                break;
            }
            case 2:
            {
                string itemCode;
                string itemName;
                string stand;
                string curStandQnt;
                string maxStandQnt;
                string qntStorage;

                cout << "Item Code: ";
                cin >> itemCode;
                cout << "Item Name: ";
                cin >> itemName;
                cout << "Stand that Item belongs: ";
                cin >> stand;
                cout << "Current Qunatity in Stand: ";
                cin >> curStandQnt;
                cout << "Max Qunatity of Stand: ";
                cin >> maxStandQnt;
                cout << "Qunatity in Storage";
                cin >> qntStorage;

                vector<string> newItem;
                newItem.push_back(itemCode);
                newItem.push_back(itemName);
                newItem.push_back(stand);
                newItem.push_back(curStandQnt);
                newItem.push_back(maxStandQnt);
                newItem.push_back(qntStorage);
                break;
            }
            case 3:
            {
                flag = false;
                break;
            }
        }
        
    }
    return 0;
}
