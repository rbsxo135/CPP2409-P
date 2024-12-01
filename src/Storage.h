#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

struct item
{
    string code;
    string name;
    string stand;
    int cur_stand_qnt;
    int max_stand_qnt;
    int inventory;
} typedef Item;

// 데이터 저장공간 클래스
class Storage{
private:
    fstream fs;
    string storage_name;
    vector<Item> database; // 벡터를 활용한 동적 배열
public:
    Storage(string filename);
    Storage(const Storage& other); // database 멤버 변수를 깊은 복사하기 위한 복사생성자
    ~Storage(); // 소멸시 변경된 내용을 저장하는 소멸자

    Item getItem(string code) const; // code에 해당하는 상품을 vector로 반환
    Item getItem(int line) const; // line번째 상품을 벡터로 반환
    int size() const; // database의 크기 반환, 목록 부분을 제외한 사이즈이다

    void addLine(Item new_line); // database 행 추가
    void removeItem(string code); // code에 해당하는 제품 삭제
    void itemSold(string code, int qnt); // code에 해당하는 제품 판매
    void itemToStand(string code, int qnt); // code에 해당하는 제품 qnt만큼 매대에 진열
    void itemStore(string code, int qnt); // code에 해당하는 제품 qnt만큼 재고수 추가
    void printDatabase(); // database 출력
};
