#include <iostream>
#include<iterator>

using namespace std;

void subset(bool *a, int n, int cur) {
    if (cur == n) {
        //终止条件：数组中的所有元素均取了0/1
        copy(a, a + n, ostream_iterator<bool>(cout, " "));
        cout << endl;
    } else {
        //递归部分：a[cur]分别取0/1，与后边部分的subset取并
        a[cur] = 0;
        subset(a, n, cur + 1);
        a[cur] = 1;
        subset(a, n, cur + 1);
    }
}

int main() {
    bool a[5]={1};
    subset(a, 5, 0);
    return 0;
}