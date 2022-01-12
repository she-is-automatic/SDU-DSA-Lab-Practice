//输入 第一行是一个整数 n (2<=n<=10)，表示需排列的数的个数。接下来一行是 n 个整数，数的范围是 0 到 100000，每两个相邻数据间用一个空格分隔。
//输出 一个整数，代表所有排列价值的或。

#include <iostream>

int ret = 0;
using namespace std;

template<typename T>
int permutations(T *a, int n, int cur) {
    int value = 0;
    if (cur == n - 1) {
        value = 0;
        for (int i = 0; i < n; ++i) {
            value += a[i] ^ (i + 1);
        }
        ret |= value;
    } else {
        for (int i = cur; i < n; ++i) {
            swap(a[cur], a[i]);
            permutations(a, n, cur + 1);
            swap(a[cur], a[i]);
        }
    }
    return ret;
}

int main() {
    int n;
    cin >> n;
    int *a = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    cout << permutations(a, n, 0);
    return 0;
}
