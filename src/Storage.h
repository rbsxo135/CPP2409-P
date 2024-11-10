#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// 데이터 저장공간 클래스
class Storage{
private:
    vector<vector<string>> database; // 2차원 벡터를 활용한 동적 배열
public:
    Storage(string filename);
    void addLine(vector<string> new_line); // database 행 추가
    void removeCode(string code); // code에 해당하는 제품 삭제
    void itemSold(string code, int qnt); // code에 해당하는 제품 판매
    void itemToStand(string code, int qnt); // code에 해당하는 제품 qnt만큼 매대에 진열
    void itemStore(string code, int qnt); // code에 해당하는 제품 qnt만큼 재고수 추가
    void printDatabase(); // database 출력
};