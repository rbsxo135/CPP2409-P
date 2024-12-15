#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
using namespace std;

const double LEARNING_RATE = 0.1;
const int EPOCH = 1000;
class PeakTime
{
private:
    double w = 0.5; // weight 값
    double b = 10; // bias 값
    double Sigmoid(double z); // 시그모이드 함수
    // 학습데이터의 X, Y값, weight, bias 값을 인자로 받아 경사하강법을 수행하는 함수
    void GradientDescent(vector<int>& X, vector<double>& Y, double& w, double& b, double learning_rate);
public:
    void ModelUpdate(double learning_rate, int epoch); // 회귀분석을 통해 w, b값을 업데이트
    bool Predict(int X); // 이전 시간대 판매량이 주어지면 이전에 피크타임이었는지 모델에 기반해 예측
};