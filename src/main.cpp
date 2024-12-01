#include "Storage.cpp"
using namespace std;

const int STORAGE_COLUMM = 6;
const int FRESH_FOOD_COLUMM = 7;

int main(int argc, char** argv)
{
    Storage storage("storage.csv");
    bool isPeak = true; // 피크타임 여부 체크하는 변수
    storage.PrintDatabase();
    bool flag = true;
    int menu;
    while(flag)
    {
        cout << "Item to display on the stand" << endl;
        if(isPeak == true)
            cout << "Peaktime = true" << endl;
        else
            cout << "Peaktime = false" << endl;

        for(int i = 0; i < storage.GetSize(); i++)
        {
            // line번째 아이템들의 값을 저장
            // 추후 각 아이템의 정보를 vector<string>으로 저장하는 것이 아닌 구조체를 만들어 저장하는 것으로 class 정의 업데이트 예정
            Item item = storage.GetItem(i);
            int cur_stand_qnt =  item.cur_stand_qnt;
            int max_stand_qnt = item.max_stand_qnt;
            int storage_qnt = item.inventory;
            
            if(isPeak == true) // 피크타임일 경우 매대에 넣을 수 있는 공간이 있기만 하면 해당 상품 및 추가 진열 수량 제시
            {
                if(cur_stand_qnt < max_stand_qnt && storage_qnt >= (max_stand_qnt - cur_stand_qnt))
                {
                    cout.width(13);
                    cout << std::right << item.code;
                    cout.width(25);
                    cout << std::right << item.name << " ---> " << max_stand_qnt - cur_stand_qnt << endl;
                }
            } else // 피크타임이 아닐 경우
            {
                // 매대 최대 수량의 40%보다 적은 수량이 매대에 있을 경우 우선도가 높은 진열 품목으로 제시
                if(cur_stand_qnt < (int)(max_stand_qnt * 0.4) && storage_qnt >= ((int)(max_stand_qnt * 0.4) - cur_stand_qnt))
                {
                    cout.width(13);
                    cout << std::right << item.code;
                    cout.width(25);
                    cout << std::right << item.name << " ---> " << max_stand_qnt - cur_stand_qnt << "  Urgent" << endl;
                } 
                // 매대 최대 수량의 40%이상 60% 미만의 수량이 매대에 있을 경우 우선도가 낮은 진열 품목으로 제시
                else if((cur_stand_qnt >= (int)(max_stand_qnt * 0.4) && cur_stand_qnt < (int)(max_stand_qnt * 0.6))
                 && storage_qnt >= ((int)(max_stand_qnt * 0.6) - cur_stand_qnt))
                {
                    cout.width(13);
                    cout << std::right << item.code;
                    cout.width(25);
                    cout << std::right << item.name << " ---> " << max_stand_qnt - cur_stand_qnt << "  Not Urgent" << endl;
                }
            }
        }
        // 메뉴 선택
        cout << "1. Sell item  2. New item  3. Place item on stand  4. Add item in storage " << endl;
        cout << "5. Remove item 6. Save and Exit" << endl;

        cin >> menu;
        switch(menu){
            case 1: // Sell item
            {   
                // 판매할 상품 code와 수량 입력
                string sell_item_code;
                int sell_qnt;
                cout << "Item Code: ";
                cin >> sell_item_code;
                cout << "Quantity: ";
                cin >> sell_qnt;

                // 해당 상품 code로 검색해서 없을 경우 에러 메시지 출력 후 break
                Item item = storage.GetItem(sell_item_code); // 검색되지 않았을 경우 오류 메시지 출력
                // 상품이 검색되지 않았을 경우 반환받은 객체 검사
                if(item.was_found == false)
                {
                    break;
                }

                // 상품 판매 처리
                storage.ItemSold(sell_item_code, sell_qnt);
                storage.PrintDatabase();
                break;
            }
            case 2: // New item
            {
                // 상품 정보 입력
                string item_code;
                string item_name;
                string stand;
                string cur_stand_qnt;
                string max_stand_qnt;
                string inventory;

                cout << "Item Code: ";
                cin >> item_code;
                cout << "Item Name: ";
                cin >> item_name;
                cout << "Stand that Item belongs: ";
                cin >> stand;
                cout << "Current Qunatity in Stand: ";
                cin >> cur_stand_qnt;
                cout << "Max Qunatity of Stand: ";
                cin >> max_stand_qnt;
                cout << "Qunatity in Storage: ";
                cin >> inventory;

                // 새로운 상품 storage에 추가
                Item newItem;
                newItem.code = item_code;
                newItem.name = item_name;
                newItem.stand = stand;
                newItem.cur_stand_qnt = stoi(cur_stand_qnt);
                newItem.max_stand_qnt = stoi(max_stand_qnt);
                newItem.inventory = stoi(inventory);

                storage.AddLine(newItem);
                storage.PrintDatabase();
                break;
            }
            case 3: // Place item on stand
            {
                // 상품 코드, 수량 입력
                string item_code;
                string qnt;

                cout << "Item Code: ";
                cin >> item_code;
                cout << "Quantity: ";
                cin >> qnt;
                
                // 상품 진열 처리
                storage.ItemToStand(item_code, stoi(qnt));
                storage.PrintDatabase();
                break;
            }
            case 4: // Add item in storage
            {
                // 상품 코드, 수량 입력
                string item_code;
                string qnt;

                cout << "Item Code: ";
                cin >> item_code;
                cout << "Quantity: ";
                cin >> qnt;

                // 재고 추가 처리
                storage.ItemStore(item_code, stoi(qnt));
                storage.PrintDatabase();
                break;
            }
            case 5: // Remove item
            {
                string item_code;
                cout << "Item Code: ";
                cin >> item_code;

                // 상품 삭제 처리
                storage.RemoveItem(item_code);
                storage.PrintDatabase();
                break;
            }
            case 6:
            {
                // flag를 false로 바꿔 무한 반복문 탈출
                flag = false;
                break;
            }
        }
        
    }
    return 0;
}
