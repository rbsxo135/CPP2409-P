#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// 상품 구조체
struct item
{
    string code; // 상품 코드
    string name; // 상품명
    string stand; // 상품이 진열될 매대
    int cur_stand_qnt; // 매대에 진열된 개수
    int max_stand_qnt; // 최대 진열 가능 개수
    int inventory; // 재고
    double item_length = 0; // 진열할 상품의 길이, 냉장 식품 매대 전용
    bool was_found; // 기본적으로 true, 상품이 검색되지 않았을 때 이 값이 false인 구조체 반환
} typedef Item;

// 냉장 매대 구조체
struct stand
{
    string name; // 매대 이름
    int max_stand_qnt; // 세로로 최대 진열 개수
    double stand_length; // 매대의 길이
    string stand_type; // 매대의 타입, Fix, Free, Oneline이 있다
    string item_type; // 진열될 상품의 타입, Free 매대의 경우 Free
    vector<Item> database; // 매대에 등록된 상품들의 동적 배열
    bool was_found; // 기본적으로 true, 매대가 검색되지 않았을 때 이 값이 false인 구조체 반환
} typedef Stand;

// 데이터 저장공간 클래스
class Storage
{
protected:
    fstream fs;
    string storage_name;
    vector<Item> database; // 벡터를 활용한 동적 배열
public:
    Storage(){}
    Storage(const Storage& other); // database 멤버 변수를 깊은 복사하기 위한 복사생성자
    void Open(string filename); // 파일 여는 함수

    virtual Item GetItem(string code); // code에 해당하는 상품을 vector로 반환
    Item GetItem(int line) const; // line번째 상품을 벡터로 반환
    int GetSize() const; // database의 크기 반환, 목록 부분을 제외한 사이즈이다

    void AddLine(Item new_line); // database 행 추가
    virtual void RemoveItem(string code); // code에 해당하는 제품 삭제
    virtual void ItemSold(string code, int qnt); // code에 해당하는 제품 판매
    virtual void ItemToStand(string code, int qnt); // code에 해당하는 제품 qnt만큼 매대에 진열
    virtual void ItemStore(string code, int qnt); // code에 해당하는 제품 qnt만큼 재고수 추가
    virtual void PrintDatabase(); // database 출력

    void Close(); // 변경된 내용을 저장
};

// 냉장 식품 매대 코너
class StorageRef : public Storage
{
protected:
    vector<Stand> stands; // 매대를 저장하는 동적 배열
public:
    void Open(string filename); // 파일 여는 함수 재정의

    // 각 매대에 저장된 database에 대해 작동할 수 있도록 함수 재정의
    Item GetItem(string code);
    
    void AddLine(string name, Item new_line);
    void RemoveItem(string code);
    void ItemSold(string code, int qnt);
    void ItemToStand(string code, int qnt);
    void ItemStore(string code, int qnt);
    void PrintDatabase();
    
    void Close(); // 파일 저장 함수 재정의
};