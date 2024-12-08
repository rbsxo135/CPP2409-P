#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

struct item
{
    string code; // 상품 코드
    string name; // 상품명
    string stand; // 상품이 진열될 매대
    int cur_stand_qnt; // 매대에 진열된 개수
    int max_stand_qnt; // 최대 진열 가능 개수
    int inventory; // 재고
    bool was_found; // 기본적으로 true, 상품이 검색되지 않았을 때 이 값이 false인 구조체 반환
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
    static Storage not_found;
    Item GetItem(string code) const; // code에 해당하는 상품을 vector로 반환
    Item GetItem(int line) const; // line번째 상품을 벡터로 반환
    int GetSize() const; // database의 크기 반환, 목록 부분을 제외한 사이즈이다

    void AddLine(Item new_line); // database 행 추가
    void RemoveItem(string code); // code에 해당하는 제품 삭제
    void ItemSold(string code, int qnt); // code에 해당하는 제품 판매
    void ItemToStand(string code, int qnt); // code에 해당하는 제품 qnt만큼 매대에 진열
    void ItemStore(string code, int qnt); // code에 해당하는 제품 qnt만큼 재고수 추가
    void PrintDatabase(); // database 출력
};
