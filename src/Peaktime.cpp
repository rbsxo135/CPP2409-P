#include "Peaktime.h"

// 시그모이드 함수
double PeakTime::Sigmoid(double z)
{
    return (1 / (1 + exp(-z)));
}

// X에는 시간대별 고객 수가 저장된 1차원 벡터 대입
// Y에는 시간대 중 고객수가 많은 최상위 5개의 시간대를 피크타임이라고 판단해 1, 나머지는 0인 1차원 벡터 대입
void PeakTime::GradientDescent(vector<int>& X, vector<double>& Y, double & w, double& b, double learning_rate)
{  
    const int m = X.size(); // 벡터 데이터의 사이즈, X, Y사이즈가 동일
    double loss = 0, dw = 0, db = 0;

    // cost function, dw, db값 계산
    for(int i = 0; i < m; i++)
    {
        double hyphthesis = Sigmoid(w * X[i] + b);

        dw += (hyphthesis - Y[i]) * X[i];
        db += hyphthesis - Y[i];
    }

    // weigth, bias 값 업데이트
    this->w -= learning_rate * dw / m;
    this->b -= learning_rate * db / m;
}

void PeakTime::ModelUpdate(double learning_rate, int epoch)
{  
    // 파일 읽고 쓰기 위한 fstream 객체, 문자열 버퍼 생성
    ifstream ifs;
    string str_buf;

    ifs.open("param.csv", ios::in); // 파라미터 저장된 csv 파일 읽기 모드로 오픈

    // w, b값 가져오기
    getline(ifs, str_buf, ',');
    this->w = stod(str_buf);
    getline(ifs, str_buf);
    this->b = stod(str_buf);

    ifs.close(); // 파일 종료

    vector<vector<int>> X; // 각 시간대별 판매량이 저장된 X 벡터
    vector<vector<double>> Y; // 각 시간대별 피크타임 여부가 저장될 12*n Y 행렬

    // 각 시간대별 판매 고객 수가 저장된 12*n 행렬이 저장된 csv 파일 읽기 모드로 오픈
    ifs.open("customer_per_hour.csv", ios::in);
    
    while(!ifs.eof())
    {   
        vector<int> tmp; // X 행렬의 각 행을 임시 저장하기 위한 벡터
        getline(ifs, str_buf, '\n');

        // string 형태로 읽어진 csv파일 한 줄을 읽어 tmp에 저장한다
        int position;
        int cur_position = 0;
        while((position = str_buf.find(",", cur_position)) != string::npos)
        {
            int len = position - cur_position;
            tmp.push_back(stod(str_buf.substr(cur_position, len)));
            cur_position = position + 1;
        }
        tmp.push_back(stoi(str_buf.substr(cur_position)));

        X.push_back(tmp); // tmp행을 X 행렬에 추가
    }
    ifs.close(); // 파일 종료

    // 각 시간대별 피크타임 여부가 저장된 12*n 행렬이 저장된 csv 파일 읽기 모드로 오픈
    ifs.open("is_peak.csv", ios::in);
    
    while(!ifs.eof())
    {   
        vector<double> tmp; // Y 행렬의 각 행을 임시 저장하기 위한 벡터
        getline(ifs, str_buf, '\n');

        // string 형태로 읽어진 csv파일 한 줄을 읽어 tmp에 저장한다
        int position;
        int cur_position = 0;
        while((position = str_buf.find(",", cur_position)) != string::npos)
        {
            int len = position - cur_position;
            tmp.push_back(stod(str_buf.substr(cur_position, len)));
            cur_position = position + 1;
        }
        tmp.push_back(stod(str_buf.substr(cur_position)));

        Y.push_back(tmp); // tmp행을 Y 행렬에 추가
    }
    ifs.close(); // 파일 종료
    
    // 로지스틱 선형 회귀 모델 학습, epoch만큼 학습한다
    for(int k = 0; k < epoch; k++)
    {
        for(int i = 0; i < X.size(); i++)
        {
            GradientDescent(X[i], Y[i], w, b, learning_rate);
        }
    }


    cout << w << " " << b << endl;
}

bool PeakTime::Predict(int prev_sales_vol)
{
    return (Sigmoid(this->w * prev_sales_vol + this->b) < 0.5) ? false : true; // 이전 시간대가 피크 타임이었을 경우 true 아닐 경우 false 반환
}
