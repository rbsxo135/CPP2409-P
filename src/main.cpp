#include "Storage.cpp"
#include "PeakTime.cpp"
#include <time.h>
using namespace std;

const int STORAGE_COLUMM = 6;
const int FRESH_FOOD_COLUMM = 7;

int main(int argc, char** argv)
{
    PeakTime pt;
    pt.ModelUpdate(LEARNING_RATE, EPOCH); // 프로그램 시작할 때 피크타임 계산 모델 업데이트
    
    Storage storage;
    storage.Open("storage.csv");
    StorageRef storage_ref;
    storage_ref.Open("storage_ref.csv");
    bool isPeak = false; // 피크타임 여부 체크하는 변수
    // 이후 다른 종류의 storage가 추가됐을 경우의 확장성 고려한 코드
    Storage *storages[] = {&storage, &storage_ref}; // 다른 storage가 추가되어도 배열에 추가만 하면 된다

   // 품목 출력
    for(auto *s : storages)
    {
        s->PrintDatabase();
        cout << "---------------------------------------------------------------------------" << endl;
    }

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
        cout << "---------------------------------------------------------------------------" << endl;

        // 현재 시각 표시해주기
        time_t raw_time = time(NULL);
        struct tm* time_info = localtime(&raw_time); // 현재 시간정보를 가지고 있는 time_info 구조체 생성
        cout << "Current Time: " << time_info->tm_hour << "h " << time_info->tm_min << "m" << endl; // 현재 시각 출력
        cout << "After " << time_info->tm_hour + 1 << ":00, Update Peaktime" << endl; // 수동 업데이트 해야 할 시각 알려주기

        // 메뉴 선택
        cout << "1. Sell item  2. New item  3. Place item on stand  4. Add item in storage " << endl;
        cout << "5. Remove item 6. Save and Exit 7. Update Peaktime" << endl;

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

                Item item;
                bool item_found = false; // 상품을 찾았는지 검사하는 변수
                int cnt = 0;
                for(auto *s : storages)
                {
                    cnt++;
                    // 해당 상품 code로 검색해서 없을 경우 NULL 구조체 저장된다
                    item = s->GetItem(sell_item_code);
                    // NULL 구조체인지 검사
                    if(item.was_found == true) // NULL 구조체가 아닐경우, 상품을 찾은 것이므로 반복문 종료
                    {
                        item_found = true;
                        break;
                    }
                    // NULL 구조체일 경우, 상품을 찾지 못한 것이므로 다른 storage에서 상품 찾기 위해 반복문 진행
                }
                cout << cnt << endl;
                
                if (item_found == false) // 모든 storage를 검색해도 상품을 찾지 못한경우
                {
                    cout << "There is no item code " << sell_item_code << endl;
                    break; // case 종료
                }

                // 상품 판매 처리
                for(auto *s : storages)
                {
                    s->ItemSold(sell_item_code, sell_qnt);
                }
                
                // 품목 출력
                for(auto *s : storages)
                {
                    s->PrintDatabase();
                    cout << "---------------------------------------------------------------------------" << endl;
                }
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
                string item_length;

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
                cout << "Length of Item(Put 0 if it isn't Refrigerated Food): ";
                cin >> item_length;

                // 새로운 상품 생성
                Item new_item;
                new_item.code = item_code;
                new_item.name = item_name;
                new_item.stand = stand;
                new_item.cur_stand_qnt = stoi(cur_stand_qnt);
                new_item.max_stand_qnt = stoi(max_stand_qnt);
                new_item.inventory = stoi(inventory);
                new_item.item_length = stod(item_length);
                new_item.was_found = true;

                // storage 종류 입력받기
                int storage_type;
                cout << "Enter storge type: 0. Normal, 1. Refrigerated Food";
                cin >> storage_type;

                switch(storage_type)
                {
                    case 0:
                        // 상품 추가
                        storage.AddLine(new_item);
                        break;
                    case 1:
                    {
                        // 매대 이름 입력 받기
                        string stand_name;
                        cout << "Enter stand name: ";
                        cin >> stand_name;

                        // 상품 추가
                        storage_ref.AddLine(stand_name, new_item);
                        break;
                    }

                }
                
                // 품목 출력
                for(auto *s : storages)
                {
                    s->PrintDatabase();
                    cout << "---------------------------------------------------------------------------" << endl;
                }
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
                for(auto *s : storages)
                {
                    s->ItemToStand(item_code, stoi(qnt));
                }

                // 품목 출력
                for(auto *s : storages)
                {
                    s->PrintDatabase();
                    cout << "---------------------------------------------------------------------------" << endl;
                }
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
                for(auto *s : storages)
                {
                    s->ItemStore(item_code, stoi(qnt));
                }

                // 품목 출력
                for(auto *s : storages)
                {
                    s->PrintDatabase();
                    cout << "---------------------------------------------------------------------------" << endl;
                }
                break;
            }
            case 5: // Remove item
            {
                string item_code;
                cout << "Item Code: ";
                cin >> item_code;

                // 상품 삭제 처리
                for(auto *s : storages)
                {
                    s->RemoveItem(item_code);
                }
                
                // 품목 출력
                for(auto *s : storages)
                {
                    s->PrintDatabase();
                    cout << "---------------------------------------------------------------------------" << endl;
                }
                break;
            }
            case 6:
            {
                // flag를 false로 바꿔 무한 반복문 탈출
                flag = false;
                break;
            }
            case 7:
            {
                int customer_num; // 이전 시간대 손님 수
                cout << "Number of customers in previous time: ";
                cin >> customer_num; // 손님 수 입력
                isPeak = pt.Predict(customer_num); // 손님 수를 바탕으로 모델을 통해 이전 시간대가 피크타임이었는지 예측
            }
        }
    }
    storage.Close();
    storage_ref.Close();
    return 0;
}
