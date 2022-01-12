#include <iostream>
using namespace std;

template<class T>
T** lowerMultiUpper(T *lower, T *upper,  int n);

int main() {
    int lower[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int upper[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int **ans = lowerMultiUpper(lower,upper,4);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << ans[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}

template<class T>
T** lowerMultiUpper(T *lower, T *upper,  int n){
    //分配结果数组的空间并初始化每个位置为0
    T **ans = new T *[n];
    for (int i = 0; i < n; ++i) {
        ans[i] = new T[n]{0};
    }

    // 计算第i行第j列的结果
    int ta = 0, tb = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            ta = i * (i - 1) / 2;   //第i行的起始索引
            tb = j - 1;             //第j列的起始索引
            int num = min(i, j);    //循环次数
            for (int k = 1; k <= num; ++k) {
                ans[i-1][j-1] += lower[ta] * upper[tb];
                ++ta;               //ta移向同一行下一列
                tb = tb + n - k;    //tb移向同一列下一行
            }
        }
    }

    return ans;
}
