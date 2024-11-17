#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "storage.h"

Storage::Storage(string filename)
{   
    this->storage_name = filename;
    this->fs.open(filename, ios::in);
    while(!fs.eof())
    {
        vector<string> tmp;
        getline(this->fs,this->str_buf,'\n');

        int position;
        int cur_position = 0;
        while((position = this->str_buf.find(",", cur_position)) != string::npos)
        {
            int len = position - cur_position;
            tmp.push_back(str_buf.substr(cur_position, len));
            cur_position = position + 1;
        }
        tmp.push_back(str_buf.substr(cur_position));
        addLine(tmp);
    }
}

// 소멸하면서 데이터베이스의 내용을 같은 이름의 새로운 csv파일로 만들어 저장한다
Storage::~Storage()
{
    ofstream outfile(this->storage_name);
    for(int i = 0; i < this->database.size(); i++)
    {
        for(int j = 0; j < this->database[i].size(); j++)
        {
            outfile << this->database[i][j];
            if(j != this->database[i].size() - 1)
                outfile << ",";
        }
        if(i != this->database.size() - 1)
            outfile << endl;
    }
}

void Storage::addLine(vector<string> new_line)
{
    this->database.push_back(new_line);
}

void Storage::removeCode(string code)
{
    for(int i = 0; i < this->database.size(); i++)
    {
        if(this->database[i][0] == code)
        {
            this->database.erase(this->database.begin() + i);
        }
    }
}

void Storage::itemSold(string code, int qnt)
{
    for(int i = 0; i < this->database.size(); i++)
    {   
        if(this->database[i][0] == code)
        {
            int tmp = stoi(database[i][3]) - qnt;
            if(tmp < 0)
            {
                cout << "Not enough items on the stand. Check quantity of the sold item" << endl;
                return;
            }
            this->database[i].erase(this->database[i].begin() + 3);
            this->database[i].insert(this->database[i].begin() + 3, to_string(tmp));
        }
    }
}

void Storage::itemToStand(string code, int qnt)
{
    for(int i = 0; i < this->database.size(); i++)
    {   
        if(this->database[i][0] == code)
        {
            if(qnt > stoi(database[i][5]))
            {
                cout << "Not enough items in the inventory. Check quantity of the item" << endl;
                return;
            }

            int tmp = stoi(database[i][3]) + qnt;
            if(tmp > stoi(database[i][4]))
            {
                cout << "Not enough space for the item. Check quantity of the item" << endl;
                return;
            }
            this->database[i].erase(this->database[i].begin() + 3);
            this->database[i].insert(this->database[i].begin() + 3, to_string(tmp));

            tmp = stoi(database[i][5]) - qnt;
            this->database[i].erase(this->database[i].begin() + 5);
            this->database[i].insert(this->database[i].begin() + 5, to_string(tmp));
        }
    }
}

void Storage::itemStore(string code, int qnt)
{
    for(int i = 0; i < this->database.size(); i++)
    {
        if(this->database[i][0] == code)
        {
            int tmp = stoi(database[i][5]) + qnt;
            this->database[i].erase(this->database[i].begin() + 5);
            this->database[i].insert(this->database[i].begin() + 5, to_string(tmp));
        }
    }
}

void Storage::printDatabase()
{
    for(vector<string> v : this->database)
    {
        int ctr = 0;
        for(string s : v)
        {
            if(ctr++ != 1)
                cout.width(13);
            else  
                cout.width(25);
            cout << std::right << s << " ";
        }
        cout << endl;
    }
}
