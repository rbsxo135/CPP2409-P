#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "storage.h"

/*
    Storage 클래스
*/

Storage::Storage(const Storage &other)
{
    this->fs.open(this->storage_name);
    this->storage_name = other.storage_name;
    // 복사할 Storage 객체의 database에 기존의 database 복사
    for(Item i : other.database)
    {
        AddLine(i);
    }
}

void Storage::Open(string filename)
{
    string str_buf;
    this->storage_name = filename;
    this->fs.open(filename, ios::in);
    
    while(!fs.eof())
    {
        vector<string> tmp; // 파일을 읽기 위한 임시 벡터, 한 줄을 저장
        getline(this->fs, str_buf,'\n');

        // string 형태로 읽어진 csv파일 한 줄을 읽어 tmp에 저장한다
        int position;
        int cur_position = 0;
        while((position = str_buf.find(",", cur_position)) != string::npos)
        {
            int len = position - cur_position;
            tmp.push_back(str_buf.substr(cur_position, len));
            cur_position = position + 1;
        }
        tmp.push_back(str_buf.substr(cur_position));

        // tmp의 string 값을 자료형에 맞게 Item 구조체에 저장
        Item data_extracted;

        data_extracted.code = tmp[0];
        data_extracted.name = tmp[1];
        data_extracted.stand = tmp[2];
        data_extracted.cur_stand_qnt = stoi(tmp[3]);
        data_extracted.max_stand_qnt = stoi(tmp[4]);
        data_extracted.inventory = stoi(tmp[5]);
        data_extracted.was_found = true;

        // 추출한 Item을 database에 저장한다
        AddLine(data_extracted);
    }
    fs.close();
}

Item Storage::GetItem(string code)
{  
    // code로 해당 item 검색
    for(Item i : this->database)
    {
        if(i.code == code)
        {
            return i; // database에 해당 item이 있을 경우 반환
        }
    }
    // 검색되지 않을 경우에 반환하는 was_found 값이 false인 NULL 구조체
    Item null_item;
    null_item.was_found = false;
    return null_item;
}

Item Storage::GetItem(int line) const
{
    return this->database[line]; // line번째 Item 반환
}

int Storage::GetSize() const
{
    return this->database.size(); // database의 목록부분을 제외한 사이즈
}

void Storage::AddLine(Item new_line)
{
    this->database.push_back(new_line); // database에 item 추가
}

void Storage::RemoveItem(string code)
{
    // code로 해당 item 검색
    for(int i = 0; i < this->database.size(); i++)
    {  
        // 검색되었을 경우 해당 item 삭제
        if(this->database[i].code == code)
        {
            this->database.erase(this->database.begin() + i);
            return;
        }
    }
}

void Storage::ItemSold(string code, int qnt)
{
    // code로 해당 item 검색
    for(int i = 0; i < this->database.size(); i++)
    {   
        // item이 검색 되었을 경우
        if(this->database[i].code == code)
        {
            int qnt_result = database[i].cur_stand_qnt - qnt; // 판매 후 진열된 수량
            // 진열된 수량이 판매 수량보다 적은 경우 에러 메시지
            if(qnt_result < 0)
            {
                cout << "Not enough items on the stand. Check quantity of the sold item" << endl;
                return;
            }
            // 아닐 경우 판매 수량만큼 진열된 수량 감소
            else 
            {
                this->database[i].cur_stand_qnt  = qnt_result;
            }
        }
    }
}

void Storage::ItemToStand(string code, int qnt)
{
    // code로 해당 item 검색
    for(int i = 0; i < this->database.size(); i++)
    {   
        if(this->database[i].code == code)
        {
            // 재고가 부족할 경우 에러 메시지
            if(qnt > database[i].inventory)
            {
                cout << "Not enough items in the inventory. Check quantity of the item" << endl;
                return;
            }

            int qnt_result_cur = database[i].cur_stand_qnt + qnt; // 매대에 진열될 총 개수
            // 진열할 수 있는 최대치를 넘어서는 경우 에러 메시지
            if(qnt_result_cur > database[i].max_stand_qnt)
            {
                cout << "Not enough space for the item. Check quantity of the item" << endl;
                return;
            }
            this->database[i].cur_stand_qnt = qnt_result_cur; // 매대 진열 개수 변경
            this->database[i].inventory -= qnt; // 재고 수 변경
        }
    }
}

void Storage::ItemStore(string code, int qnt)
{
    // code로 해당 item 검색
    for(int i = 0; i < this->database.size(); i++)
    {
        if(this->database[i].code == code)
        {
            this->database[i].inventory += qnt; // 재고 수 증가
        }
    }
}

void Storage::PrintDatabase()
{
    cout << "         code                      name         stand" << endl;
    for(item i : this->database)
    {
        cout.width(13);
        cout << right << i.code << " ";
        cout.width(25);
        cout << right << i.name << " ";
        cout. width(13);
        cout << right << i.stand << "  " << i.cur_stand_qnt << "/" 
                << i.max_stand_qnt << "/" << i.inventory << endl;
    }
}

void Storage::Close()
{
    cout << storage_name << " closed" << endl;
    ofstream outfile(this->storage_name);
    for(int i = 0; i < this->database.size(); i++)
    {
        outfile << this->database[i].code << ",";
        outfile << this->database[i].name << ","; 
        outfile << this->database[i].stand << ","; 
        outfile << this->database[i].cur_stand_qnt << ","; 
        outfile << this->database[i].max_stand_qnt << ",";
        outfile << this->database[i].inventory;
        
        if(i != this->database.size() - 1)
            outfile << endl;
    }
    outfile.close();
}

/*********************************************************************************************************************************** */


/*
    StorageRef 클래스
*/

void StorageRef::Combination(vector<Item> arr, vector<Item> comb, vector<vector<Item>> &result, int r, int index, int depth)
{
    if (r == 0)
    {
        vector<Item> tmp;
        for(Item i : comb)
        {   
            tmp.push_back(i);
        }
        result.push_back(tmp);
    }
    else if (depth == arr.size())  // depth == n // 계속 안뽑다가 r 개를 채우지 못한 경우는 이 곳에 걸려야 한다.
    {
        return;
    }
    else
    {
        // arr[depth] 를 뽑은 경우
        comb[index] = arr[depth];
        Combination(arr, comb, result, r - 1, index + 1, depth + 1);
        
        // arr[depth] 를 뽑지 않은 경우
        Combination(arr, comb, result, r, index, depth + 1);
    }
}

// 냉장식품 Storage는 기본 Storage와 자료의 형태가 다르므로 함수를 재정의하여 형식에 맞게 읽을 수 있도록 한다
void StorageRef::Open(string filename)
{
    string str_buf;
    this->storage_name = filename;
    this->fs.open(filename, ios::in);
    int stand_idx = -1;

    while(!fs.eof())
    {
        int position;
        int cur_position = 0;

        vector<string> tmp; // 파일을 읽기 위한 임시 벡터, 한 줄을 저장
        getline(this->fs, str_buf,'\n');

        if(str_buf == "/") // 매대 정보 구분자를 읽을 경우
        {
            getline(this->fs, str_buf, '\n'); // 매대 정보 한 줄을 저장
            // string 형태로 읽어진 csv파일 한 줄을 읽어 tmp에 저장한다
            while((position = str_buf.find(",", cur_position)) != string::npos)
            {
                int len = position - cur_position;
                tmp.push_back(str_buf.substr(cur_position, len));
                cur_position = position + 1;
            }
            tmp.push_back(str_buf.substr(cur_position));

            // tmp의 string 값을 자료형에 맞게 Stand 구조체에 저장
            Stand data_extracted;
            data_extracted.name = tmp[0];
            data_extracted.max_stand_qnt = stoi(tmp[1]);
            data_extracted.stand_length = stod(tmp[2]);
            if(tmp[3] == "Fix")
            {
                data_extracted.stand_type = Fix;
            } 
            else if(tmp[3] == "Oneline")
            {
                data_extracted.stand_type = Oneline;
            }
            else
            {
                data_extracted.stand_type = Free;
            }
            data_extracted.was_found = true;

            this->stands.push_back(data_extracted); // Stand 구조체 동적 배열에 저장
            stand_idx++; // 다음 읽어지는 상품 정보들이 현재 읽은 매대에 등록되도록 현재 매대의 인덱스 값 증가
            continue;
        }

        // string 형태로 읽어진 csv파일 한 줄을 읽어 tmp에 저장한다
        while((position = str_buf.find(",", cur_position)) != string::npos)
        {
            int len = position - cur_position;
            tmp.push_back(str_buf.substr(cur_position, len));
            cur_position = position + 1;
        }
        tmp.push_back(str_buf.substr(cur_position));

        // tmp의 string 값을 자료형에 맞게 Item 구조체에 저장
        Item data_extracted;

        data_extracted.code = tmp[0];
        data_extracted.name = tmp[1];
        data_extracted.stand = tmp[2];
        data_extracted.cur_stand_qnt = stoi(tmp[3]);
        data_extracted.max_stand_qnt = stoi(tmp[4]);
        data_extracted.inventory = stoi(tmp[5]);
        data_extracted.item_length = stod(tmp[6]); // 냉장식품의 경우 자료의 종류가 하나 더 있기 때문에 추가로 처리해준다
        data_extracted.was_found = true;

        // 추출한 Item을 특정 stand의 database에 저장한다
        this->stands[stand_idx].database.push_back(data_extracted);
    }
    fs.close();
}

Item StorageRef::GetItem(string code)
{  

    Item tmp;
    for(Stand s : stands)
    {
        this->database = s.database; // 현재 database에 매대의 database 임시 할당
        tmp = Storage::GetItem(code); // 부모 함수 재사용
        if(tmp.was_found == true)
        cout << "item found" << endl;
            break;
    }
    return tmp;
}

void StorageRef::AddLine(string name, Item new_line)
{
    Stand* tmp;
    bool was_found = false;

    // name으로 stand 검색
    for(Stand &s : this->stands)
    {
        if(s.name == name)
        {
            tmp = &s; // 해당 stand가 있을 경우 반환
            was_found = true;
        }
    }
    if(was_found == false) // 찾지 못 한 경우 함수 종료
        return;

    // 정해진 종류에 맞게 등록해야 하는데 그러지 않은 경우 에러 발생
    if(tmp->stand_type != Free && !tmp->item_type.compare(new_line.stand))
    {
        cout << "Type is not matching!" << endl; 
        return;
    }
    tmp->database.push_back(new_line);
}

void StorageRef::RemoveItem(string code)
{
    for(Stand &s : stands)
    {
        this->database = s.database; // 현재 database에 매대의 database 임시 할당
        Storage::RemoveItem(code); // 부모 함수 재사용
        s.database = this->database;
    }
}

void StorageRef::ItemSold(string code, int qnt)
{
    for(Stand &s : stands)
    {
        this->database = s.database; // 현재 database에 매대의 database 임시 할당
        Storage::ItemSold(code, qnt); // 부모 함수 재사용
        s.database = this->database;
    }
}

void StorageRef::ItemToStand(string code, int qnt)
{
    for(Stand &s : stands)
    {
        this->database = s.database; // 현재 database에 매대의 database 임시 할당
        Storage::ItemToStand(code, qnt); // 부모 함수 재사용
        s.database = this->database;
    }
}

void StorageRef::ItemStore(string code, int qnt)
{
    for(Stand &s : stands)
    {
        this->database = s.database; // 현재 database에 매대의 database 임시 할당
        Storage::ItemStore(code, qnt); // 부모 함수 재사용
        s.database = this->database;
    }
}

void StorageRef::PrintDatabase()
{   
    for(Stand s : stands)
    {   
        cout << "Stand: " << s.name << endl;
        cout << "Type: ";
            if(s.stand_type == Fix)
                cout << "Fix" << endl;
            else if(s.stand_type == Free)
                cout << "Free" << endl;
            else  
                cout << "Oneline" << endl;
        this->database = s.database; // 현재 database에 매대의 database 임시 할당
        Storage::PrintDatabase(); // 부모 함수 재사용
    }
}

void StorageRef::NewStand(Stand new_stand)
{
    this->stands.push_back(new_stand);
}

void StorageRef::RemoveStand(string name)
{
    for(int i = 0; i < this->stands.size(); i++)
    {  
        // 검색되었을 경우 해당 stand 삭제
        if(this->stands[i].name == name)
        {
            this->stands.erase(this->stands.begin() + i);
            return;
        }
    }
}

vector<Item> StorageRef::UpdateItemList()
{
    vector<Item> result;
    vector<string> code_list; // 진열 필요한 상품 목록
    vector<string> qnt_list; // 진열 개수

    for (Stand s: this->stands)
    {
        StandType type = s.stand_type;
        switch(type)
        {
            case(Fix):
            case(Free):
            {
                double stand_length = s.stand_length;
                double cur_length = 0;
                vector<Item> zero_item; // 현재 매대에 진열된 상품이 없는 상품의 코드 리스트
                vector<vector<Item>> comb_zero_item; // zero_item의 값들의 조합 결과, nC1, nC2...nCn 저장장
                
                // database에 저장된 item의 현황 파악
                for(Item i : s.database)
                {
                    // 현재 매대에 진열된 상품이 있을 경우 공간을 어느정도 차지하는 지 합산
                    if (i.cur_stand_qnt > 0)
                    {
                        cur_length += i.item_length;
                    }
                    // 현재 매대에 진열된 상품이 없을 경우 해당 아이템 코드를 리스트에 저장
                    else if(i.inventory > 0)
                    {
                        zero_item.push_back(i);
                    }
                }
                
                // 
                if(zero_item.size() > 0)
                {
                    for(int r = 1; r <= zero_item.size(); r++)
                    {
                        vector<Item> comb(r); // 조합 계산을 위한 임시 벡터, 크기 r
                        Combination(zero_item, comb, comb_zero_item, r, 0, 0);
                    }
                }
                
                // 각 조합으로부터 조합내의 상품의 길이의 합이 냉장 매대의 빈 공간보다 작은 합중 최대 합을 가진 조합 선별
                double max_sum = 0;
                vector<Item> result_comb;
                for(vector<Item> v : comb_zero_item)
                {  
                    double tmp_sum = 0;
                    for(Item i : v)
                    {
                        tmp_sum += i.item_length;
                    }
                    if(tmp_sum < stand_length - cur_length && tmp_sum > max_sum)
                    {
                        max_sum = tmp_sum;
                        result_comb = v;
                    }
                }

                // 최적 조합내의 상품의 코드와 진열해야 하는 갯수 저장
                for(Item i : result_comb)
                {
                    result.push_back(i);
                    break;
                }
            }
            case(Oneline):
            {
                int cur_stand; // 현재 Oneline 매대에 있는 상품의 총 개수
                for(Item i : s.database)
                {
                    cur_stand += i.cur_stand_qnt;
                }
                vector<Item> zero_item; // 현재 매대에 진열된 상품이 없는 상품의 코드 리스트
                for(Item i : s.database)
                {
                    if(i.cur_stand_qnt == 0)
                    {
                        zero_item.push_back(i);
                    }
                    if(zero_item.size() == s.max_stand_qnt  - cur_stand) // Oneline 매대에 최대로 넣을 수 있는 수량을 선정했을 경우 반복문 종료료
                        break;
                }
                for(Item i : zero_item)
                    result.push_back(i);
                break;
            }
        }
    }
    return result;
}

void StorageRef::Close()
{
    cout << storage_name << " closed" << endl;
    ofstream outfile(this->storage_name);
    for(int i = 0; i < this->stands.size(); i++)
    {
        outfile << "/" << endl;
        outfile << this->stands[i].name << ",";
        outfile << this->stands[i].max_stand_qnt << ",";
        outfile << this->stands[i].stand_length << ",";
        outfile << this->stands[i].stand_type << endl;

        for(int k = 0; k < this->stands[i].database.size(); k++)
        {
            outfile << this->stands[i].database[k].code << ",";
            outfile << this->stands[i].database[k].name << ","; 
            outfile << this->stands[i].database[k].stand << ","; 
            outfile << this->stands[i].database[k].cur_stand_qnt << ","; 
            outfile << this->stands[i].database[k].max_stand_qnt << ",";
            outfile << this->stands[i].database[k].inventory << "," ;
            outfile << this->stands[i].database[k].item_length;
            if(i != this->stands.size() - 1 || k != this->stands[i].database.size() - 1)
                outfile << endl;
        }
    }
    outfile.close();
}
