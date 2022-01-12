#include <iostream>

using namespace std;

template<typename T>
bool belong(const T *a, int len, int cur, const T &x) {
    if (cur == len - 1) {
        return a[cur] == x;   //基础部分：只剩最后一个元素，若相等，则在集合中，否则不在
    } else {
        //递归部分：要想在集合中，val要么与a[cur]相等，要么在a[cur]之后
        return a[cur] == x || belong(a, len, cur + 1, x);
    }
}

int main() {
    int a[5] = {1, 2, 3, 4, 5};
    cout << belong(a, 5, 0, 5);
    string b[3] = {"afega", "egs", " "};
    cout << belong<string>(b, 3, 0, " ");

    return 0;
}