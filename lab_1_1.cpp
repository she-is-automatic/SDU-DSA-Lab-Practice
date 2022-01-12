//输入：第一行，一个整数n，接下来一行有n个非负整数：a1,a2,⋯,an
//输出：一个整数，表示所有子集价值的异或和。

#include <iostream>
using namespace std;

int ret;
int xor_sum(int *a, int *x, int n, int cur);

int main() {
    int n;
    cin >> n;
    int *a = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    int *x = new int[n];
    cout << xor_sum(a, x, n, 0);

    return 0;
}

int xor_sum(int *a, int *x, int n, int cur) {
    if (cur == n) {//终止条件：数组中的所有元素均取了0/1
        int value = 0,count = 0;
        for (int i = 0; i < n; ++i) {
            if (x[i] != 0) {
                count++;
                value += a[i] * count;   //求子集的价值
            }
        }
        ret ^= value;
    }
    else {
        //递归部分：a[cur]分别取0/1，与后边部分的subset取并
        x[cur] = 0;
        xor_sum(a, x, n, cur + 1);
        x[cur] = 1;
        xor_sum(a, x, n, cur + 1);
    }
    return ret;
}


