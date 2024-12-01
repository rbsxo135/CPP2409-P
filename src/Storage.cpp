#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "storage.h"

Storage::Storage(string filename)
{   
    string str_buf;
    this->storage_name = filename;
    this->fs.open(filename, ios::in);
    
    getline(this->fs, str_buf,'\n'); // 목록 부분을 제외하기 위해 한 줄을 받기만 하는 작업
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
}

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

// 소멸하면서 데이터베이스의 내용을 같은 이름의 새로운 csv파일로 만들어 저장한다
Storage::~Storage()
{
    ofstream outfile(this->storage_name);
    for(int i = 0; i < this->database.size(); i++)
    {
        outfile << this->database[i].code << ",";
        outfile << this->database[i].name << ","; 
        outfile << this->database[i].stand << ","; 
        outfile << this->database[i].cur_stand_qnt << ","; 
        outfile << this->database[i].max_stand_qnt << ","; 
        outfile << this->database[i].inventory << endl;
    }
}

Item Storage::GetItem(string code) const
{  
    // code로 해당 item 검색
    for(Item i : this->database)
    {
        if(i.code == code)
        {
            return i; // database에 해당 item이 있을 경우 반환
        }
    }
    cout << "Wrong item code" << endl; // 검색되지 않았을 경우 에러 메시지
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
    // 검색되지 않았을 경우 에러 메시지
    cout << "There is no item that has code " << code << endl;
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
    for(item i : this->database)
    {
        cout.width(13);
        cout << right << i.code << " ";
        cout.width(25);
        cout << right << i.name << " ";
        cout. width(13);
        cout << right << i.stand << " " << i.cur_stand_qnt << " " 
                << i.max_stand_qnt << " " << i.inventory << endl;
    }
}
